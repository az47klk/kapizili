#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

#define button A0

SoftwareSerial ses(6,7);
RF24 radio(9, 10); // CE, CSN

const byte addresses[][6] = {"1Node", "2Node"};
bool listening = true;
unsigned long ledDurumSure;

// prototypes of functions
void ledDurumDevriye();
int _ledStatusRead();


void setup() 
{
    pinMode(5, OUTPUT);
    pinMode(A0, INPUT_PULLUP);
    Serial.begin(9600);
    ses.begin(9600);

    // kablosuz iletisim modulu kontrolu
    if (!radio.begin())
    {
        Serial.println("Communication module error.");
        while (true)
        {
            digitalWrite(5, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            delay(1900);
        }
    }
    else Serial.println("Connected.");

    radio.openReadingPipe(1, addresses[1]);
    radio.openWritingPipe(addresses[0]);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();

    // ses modulunu dinleme moduna alma
    ses.write(0xAA);
    ses.write(0x37);
    delay(1000);
    ses.write(0xAA);
    ses.write(0x21);
    delay(1000);
}
void loop() 
{
    if (ses.available() > 0 && listening)
    {
        int komut = ses.read();
        switch (komut)
        {
            case 0x11:
                Serial.println("0x11 geldi");
                listening = false;
            break;
            case 0x12:
                Serial.println("0x12 geldi");
                listening = false;
            break;
            case 0x13:
                Serial.println("0x13 geldi");
                listening = false;
            break;
            case 0x14:
                Serial.println("0x14 geldi");
                listening = false;
            break;
            case 0x15:
                Serial.println("0x15 geldi");
                listening = false;
            break;
        }
    }
    if (millis() - ledDurumSure > 200)
    {
        Serial.println((listening ? "Durum: dogru" : "Durum: yanlis"));
        ledDurumDevriye();
        ledDurumSure = millis();
    }

    if (!listening) _ledStatusRead(); // client side'dan kapatma
    if (!listening && !digitalRead(A0)) listening = true; // uzerindeki buttondan kapatma
}

void ledDurumDevriye()
{
    if (listening == false)
    {
        radio.stopListening();
        int buff = 12;
        if (radio.write(&buff, sizeof(buff)))
            Serial.println("ledDurumDevriye: listening = false radio success.");
        else
            Serial.println("ledDurumDevriye: listening = false radio fail.");
        
        radio.startListening();
    }
    else 
    {
        radio.stopListening();
        int buff = 11;
        if (radio.write(&buff, sizeof(buff)))
        {
            Serial.println("ledDurumDevriye: listening = true radio success.");
        }
        else
        {
            Serial.println("ledDurumDevriye: listening = true radio fail.");
        }
        radio.startListening();
    }
}

int _ledStatusRead()
{
    int rapor = 0;
    while (radio.available())
    {
        radio.read(&rapor, sizeof(rapor));
        Serial.print("_ledStatusRead while radio available read rapor: ");
        Serial.println(rapor);
        if (rapor == 10) listening = true;
    }
    return rapor;
}