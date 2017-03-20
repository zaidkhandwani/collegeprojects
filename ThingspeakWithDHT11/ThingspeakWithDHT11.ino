#include "DHT.h"
#include <SoftwareSerial.h>

#define _SS_MAX_RX_BUFF 256
#define DHTPIN 2          //Arduino Pin on which Pin2 of DHT sensor is connected
#define DHTTYPE DHT11     //Type of DHT Sensor connected
#define EspRxPIN 8        //Pin on which Rx of ESP Module is connected
#define EspTxPIN 9        //Pin on which Tx of ESP Module is connected
#define EspBaudRate 9600  //Baud rate of communication with ESP Module
String domain = "api.thingspeak.com";
String url = "/update?api_key=6GMY86Q8TU9KQP55&field1=";

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(EspTxPIN, EspRxPIN); 

void setup() {
  Serial.begin(9600);
  Serial.println("Data Center ==> Debug Mode");
  mySerial.begin(EspBaudRate);
  dht.begin();
}

void loop() {
  delay(7000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  /*if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }*/

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  Serial.println();
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
  msg += url + "15";//String(t,0);
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
      while(mySerial.available())
      {
        delay(20);
        Serial.write(mySerial.read());
      }
  }
}

