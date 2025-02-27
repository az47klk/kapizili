#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <bitwise.hpp>
#include <button.h>

#define SIGOK   0b00
#define SIGON   0b01
#define SIGOFF  0b10
#define SIGOFFREQ 0b11

#define led 5

button offButton(A0);
RF24 radio(9,10);
const byte address[6] = "azk11";

bool reading = true;
unsigned long readingTimer = 0;


void writeData(uint8_t);
uint8_t getDataType(uint8_t);
void ledBlinkFunc();

// longs for timer funcs
bool ledStatus = 0;
bool ledTempStatus = 0;
unsigned long ledTimer = 0;


// Setup Section
void setup() {

  pinMode(led,OUTPUT);
  Serial.begin(9600);

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
}

void loop() {
  offButton.loopActivator();
  ledBlinkFunc();
    
  if (reading){
  if (radio.available()){
    int buff;
    radio.read(&buff, sizeof(buff));
    
    int data = getDataType(buff);
    Serial.println(" Msg: " + String(data));

    if (data == SIGOFF){
      ledStatus = 0;
    }
    else if (data == SIGON){
      ledStatus = 1;
    }
    writeData(data);
  }
  }

  if (!reading){
    if (millis() - readingTimer > 80){
      reading = true;
      radio.startListening();
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

void ledBlinkFunc(){
 if (ledStatus){
  if (millis() - ledTimer > 500){
    if (ledTempStatus){
    ledTempStatus = 0;
    analogWrite(led, 0);
    
  }
  else{
    ledTempStatus = 1;
    analogWrite(led, 255);
  }
  ledTimer = millis();
  }
  
 }
 else {
  analogWrite(led, 0);
 }
}

void button::onPressed(){
  writeData(SIGOFFREQ);
  ledStatus = 0;
}

void button::onPressedOut(){
  
}