#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define button A0

RF24 radio(9, 10); // CE, CSN

int gelen;

const byte addresses[][6] = {"1Node", "2Node"};

unsigned long timr2 = 0;

bool buttArc = false;

void setup() {
    pinMode(A0, INPUT_PULLUP);
    pinMode(5, OUTPUT);
    Serial.begin(9600);
    if (!radio.begin())
    {
        Serial.println("Hw not working!");
        while (true){
            digitalWrite(5, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            delay(1900);
        }
    }
    else Serial.println("Connected.");
    radio.openReadingPipe(1, addresses[0]);
    radio.openWritingPipe(addresses[1]);
    radio.setPALevel(RF24_PA_MIN);
  
    digitalWrite(5, LOW);
}

void loop() {
    radio.startListening();
    if (radio.available())
    {
        while(radio.available())
        {
            radio.read(&gelen, sizeof(gelen));
            Serial.println(gelen);
            if (gelen == 12)
            {
                digitalWrite(5, HIGH);
            }
            else if (gelen == 11)
            {
                digitalWrite(5, LOW);
            }
        }
    }
    if (!digitalRead(A0) && !buttArc)
    {
        buttArc = true;
        radio.stopListening();
        int buff = 10;
        if (radio.write(&buff, sizeof(buff)))
        {
            Serial.println("10");
        }
        else Serial.println("ERR");
        radio.startListening();
    }
    if (buttArc)
    {
        Serial.print("BTN: ");
        Serial.println((millis() - timr2));
    }
    if (buttArc && ((millis() - timr2) >= 450))
    {
        Serial.println("BTN: OK");
        buttArc = false;
        timr2 = millis();
    }
}
