//Program: Sending SMS with the SIM808 Arduino module

#include <LCD_I2C.h>
#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
#include "testSIM808.h"
#include "sim.h"
#include "station.h"
#define SIM808_RX 2
#define SIM808_TX 3
#define SIM808_RST 4

//number of pin 
const int buzzer = 6;
const int buttonPin = 8;
const int ledPin = 10;


//-------------------
//SOME VARIABLE 
int buttonState = 0;
int valueOfNumber = 0;
char callNumber[11];
int value = 0;

#if defined(__AVR__)

//Define some helper macro here 
#ifndef __HAVE_SIM808_H 
#define __HAVE_SIM808_H 

#endif 
// Maybe some additional features here 
#endif

char replybuffer[255];

SoftwareSerial SIM808SS = SoftwareSerial(SIM808_TX, SIM808_RX);
SoftwareSerial *SIM808Serial = &SIM808SS;
SIM808 Sim808 = SIM808(SIM808Serial);
char * sendto = "0335784621";
char * message = "Test sim808";

//VARIABLE FOR CALL AND SMS
int call;
char bufferSMS[32];
char sender[32];
uint16_t *readlen;
uint16_t *senderlen;
char *phonenumberIn;
unsigned long timer;
int CallStatus;


//VARIABLE FOR THE GPS
int8_t gpsStatus;
float lat, lon, speed, heading, altitude;
double gpslat;
double gpslon;

void  setup() 
{
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.println(F("Basic test of the SIM808 module"));
  Serial.println(F("Starting... (may take a few seconds)"));
  Serial.begin(9600);
  SIM808Serial->begin(9600);
  Sim808.begin(*SIM808Serial);
  Serial.println("Originated module");
  Sim808.print("AT+CPIN?\r\n");
  //Sim808.enableGPS(true);
  //Sim808.print("AT+BTPOWER=1\r\n");
  //Sim808.print("AT+CREG?\r\n");
  //Sim808.print("AT+CSQ\r\n");
  //Sim808.print("AT+CLVL=85\r\n"); //volume of the speaker
  //Sim808.print("AT+CHFA=0\r\n");
  //Sim808.print("ATE1\r\n");
}
void  loop() 
{
valueOfNumber = 0;

/*if(value >= 0 && value < 10)
  {
    while( millis() - timer < 5000)
    {
      buttonState = digitalRead(buttonPin);
      if(buttonState == HIGH)
      {
        digitalWrite(ledPin, HIGH);
        delay(250);
        valueOfNumber += 1;
        Serial.println(valueOfNumber);
      }else
      {
        digitalWrite(ledPin, LOW);
      }
    }
  }
  Serial.println("nouveau chiffre....");
  
  //if(millis() - timer > 10000)
  //{
    timer = millis();
    callNumber[value] = valueOfNumber + '0';
    value += 1;
  //}
  
  if((value == 10) && (millis() - timer < 10000))
  {
    callNumber[value] = '\0';
    Serial.println(F("le numero est : "));
    Serial.println(callNumber);
    Serial.println("press the button to call.");
    while(value == 10)
    {
        Serial.println("press the button to call.");
        if(buttonState == HIGH)
        {
          Sim808.makeCall(callNumber, 5000);
          break;
        }
    }
    Serial.println("call ended.");
    value = -1;
  }*/

  //Sim808.makeCall(sendto,5000);
  //Sim808.getNumSMS();


  /*Sim808.readSMS(1, bufferSMS, 2048, readlen);
  Sim808.getSMSSender(1,sender,senderlen);
  lcd.print(bufferSMS);
  lcd.setCursor(0, 1);
  lcd.print("par:");
  lcd.setCursor(5, 1);
  lcd.print(sender);*/

  //Serial.println("Calling mode");
  //delay(1000);
    CallStatus = Sim808.getCallStatus();
    switch(CallStatus)
    {
      case 3:
        Serial.println("Status : 3 (Ringing)");
        delay(500);
        Sim808.pickUp();
        delay(10000);
        Sim808.hangUp();
      break;

      case 0:
        Serial.println("Status : 0 (Ready)");
      break;

      case 1:
        Serial.println("Status : 1 (Failure)");
      break;

      case 2:
        Serial.println("Status : 2 (Unknown)");
      break;
      case 4:
        Serial.println("Status : 4 (Ongoing call)");
      break;
      default:
        Serial.println(" Status : Unknown");
    }
    /*
  Serial.println("Waiting for GPS fix...");
  while (true) {
    gpsStatus = Sim808.GPSstatus();
    if (gpsStatus == 1) {
      Serial.println("GPS fix acquired!");
      break;
    } else {
      Serial.println("No fix yet...");
      delay(2000);
    }
  }

  Sim808.getGPS(&lat, &lon, &speed, &heading, &altitude);
  gpslat = lat;
  gpslon = lon;

  Station nearest = findNearest(gpslat, gpslon);

  Serial.print("Station la plus proche: ");
  Serial.println(nearest.nom);
  Serial.print("Coordonnées: ");
  afficherStation(nearest);
  Serial.print(nearest.lat); Serial.print(", ");
  Serial.println(nearest.lon);
*/
delay(3000);
}

