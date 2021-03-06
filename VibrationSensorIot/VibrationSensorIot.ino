// Code by Khandwani Mohd Zaid <-- 20-03-2017 -->

#include <SoftwareSerial.h>

#define sensor A0
float val;
#define _SS_MAX_RX_BUFF 256
#define TRIGPIN 12                       //Pin on which Trig Pin of HC-SR04 is connected
#define ECHOPIN 13                       //Pin on which Echo Pin of HC-SR04 is connected
#define EspRxPIN 8                       //Pin on which Rx of ESP Module is connected
#define EspTxPIN 9                       //Pin on which Tx of ESP Module is connected
#define EspBaudRate 9600                 //Baud rate of communication with ESP Module
String domain = "api.thingspeak.com";
String url = "/update?";
String apiKey = "6GMY86Q8TU9KQP55";



SoftwareSerial mySerial(EspTxPIN, EspRxPIN); 

void setup() {
  Serial.begin(9600);
  Serial.println("Debug Mode");
  mySerial.begin(EspBaudRate);
  mySerial.println("AT+CWJAP=\"Mhsscoe\",\"Mhsscoe@1\"");
  pinMode(sensor,INPUT);
}

void loop() {
  delay(10000);
  
 val=analogRead(sensor);
val= 500-val;
val= val/5;
  Serial.print("Vibration: ");
  Serial.print(val);
  Serial.println(" %");
  Serial.print("Sending : ");
  Serial.println("AT+CIPCLOSE");
  mySerial.println("AT+CIPCLOSE");
  delay(500);
  reader();

  String startString = "AT+CIPSTART=\"TCP\",\"";
  startString += domain;
  startString += "\",80";
  
  Serial.print("Sending : ");
  Serial.println(startString);
  mySerial.println(startString);
  delay(1500);
  reader();

  String msg = "GET ";
  msg += url + "api_key="  + apiKey + "&field1=" + String(val);    // Attach your data here to publish on net
  int len = msg.length();
  len += 2;

  Serial.print("Sending : ");
  Serial.println("AT+CIPSEND=" + String(len));
  mySerial.println("AT+CIPSEND=" + String(len));
  delay(500);
  reader();
  
  Serial.print("Sending : ");
  Serial.println(msg);
  mySerial.println(msg);
  delay(500);
  reader();
  long tym = millis();
  while(!mySerial.available())
  {
    if((millis() - tym) >5000)
    {
        Serial.println("Timeout");
        break;
    }
  }
  if(mySerial.available())
  {
    while(mySerial.available())
    {
      delay(1);
      Serial.write(mySerial.read());
    }
  }
  
  Serial.println();
}

void reader()
{
  delay(500);
  if(mySerial.available()>0)
  {
      //Serial.println("Available");
      while(mySerial.available())
      {
        delay(20);
        Serial.write(mySerial.read());
      }
  }
}

