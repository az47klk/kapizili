#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <bitwise.hpp>
#include <button.h>
#include <SoftwareSerial.h>

// Status Datatypes
#define SIGOK   0b00
#define SIGON   0b01
#define SIGOFF  0b10
#define SIGOFFREQ 0b11

#define id 0U
#define led 5

button offButton(A0);
SoftwareSerial ses(6,7);
RF24 radio(9,10);

const byte address[6] = "azk11";

bool reading = true;
unsigned long readingTimer = 0;

bool testPress = 0;
unsigned long testTimer = 0;

unsigned long repeatTime = 0;

void writeData(uint8_t);
uint8_t getDataType(uint8_t);
void sesTanima();

bool ledStatus = 0;

// Setup Section
void setup() {

  pinMode(led,OUTPUT);
  Serial.begin(9600);
  ses.begin(9600);

  if (!radio.begin()) // Check communication module. If it is not working, begin loop.
  {
    while (true){
      Serial.println("Communication Module Error!");
      delay(1000);
      if (radio.begin()) break;
    }
  }
  Serial.println("Module Connected!");
   
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);
  
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  ses.write(0xAA);
  ses.write(0x37);
  delay(1000);
  ses.write(0xAA);
  ses.write(0x21);
  delay(1000);
}

void loop() {
  offButton.loopActivator();
  sesTanima();

  // read all packets
  if (reading){
  if (radio.available()){
    int buff;
    radio.read(&buff, sizeof(buff));

    int data = getDataType(buff);
    Serial.println("Msg: " + String(data));
    if (data == SIGOFFREQ){
      delay(80);
      writeData(SIGOFF);
      writeData(SIGOFF);
      writeData(SIGOFF);
      
      ledStatus = 0;
    }
  }
  }

 
  /*
   if (millis() - repeatTime > 5000){
    if (ledStatus == 1){
      writeData(SIGON);
    }
    else {
      writeData(SIGOFF);
    }
    Serial.println("echo");
    repeatTime = millis();
  }
  
  */
  if (!reading){
    if (millis() - readingTimer > 80){
      reading = true;
      Serial.println("startRead");
      radio.startListening();
    }
  }

  //test mode
  if (testPress){
    if (millis() - testTimer > 2000){
      writeData(SIGON);
      ledStatus = 1;
      Serial.println("testMode");
      testPress = 0;
    }
  }
}


void writeData(uint8_t _datatype){
  uint8_t buff = 0;
  buff |= (_datatype << 0);
  radio.stopListening();
  Serial.print("Write: ");
  Serial.println(radio.write(&buff, sizeof(buff)));
  reading = false;
  readingTimer = millis();
}

uint8_t getDataType(uint8_t data){
  uint8_t buff = 0;
  buff |= (getbit(data,0) << 0);
  buff |= (getbit(data, 1) << 1);
  return buff;
}


void button::onPressed(){
  writeData(SIGOFF);
  ledStatus = 0;
  testTimer = millis();
  testPress = 1;
}

void button::onPressedOut(){
  testPress = 0;
}

void sesTanima(){
  if (ses.available() > 0)
    {
        int komut = ses.read();
        switch (komut)
        {
            case 0x11:
                Serial.println("Sound 0x11");
                ledStatus = 1;
                writeData(SIGON);
            break;
            case 0x12:
                Serial.println("Sound 0x12");
                ledStatus = 1;
                writeData(SIGON);
            break;
            case 0x13:
                Serial.println("Sound 0x13");
                ledStatus = 1;
                writeData(SIGON);
            break;
            case 0x14:
                Serial.println("Sound 0x14");
                ledStatus = 1;
                writeData(SIGON);
            break;
            case 0x15:
                Serial.println("Sound 0x15");
                ledStatus = 1;
                writeData(SIGON);
            break;
        }
    }
}
