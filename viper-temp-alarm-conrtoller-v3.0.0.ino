/*
Ownership: Virtual Incision Corp
Last Modified: 8/13/2019

Author: David Ryan
  E: davidryn6@gmail.com
  C: (402)-499-8715

++Permanent Temperature Alarm++

OM-THA2-U Temperature/Humidity/Dewpoint Alarm's (120-000060-000) NO (normaly open) relay output 
connected to reset pin of WeMos D1 Mini ESP8266 chip. When tripped opens curcit and resets 
D1 Mini to run this program.

D1 Mini driver:
https://wiki.wemos.cc/downloads
board Manager:
http://arduino.esp8266.com/stable/package_esp8266com_index.json


setup:
Connects to Internal WiFi and triggers an event on ifttt. ifttt sends email to a
dummy Virtual Incision google account (details below). Sets keepAlivePin to HIGH to
prevent reset upon Alarm reset. Blinks LED light and waits for a button input to 
break loop.  Sets keepAlivePin to low to allow reset.

loop:
na

Virtual Incision dummy google account info:
vipermtempalarm@gmail.com
password: EtGuUU++vNN@#4GZ
password will work with anything connected to this account including ifttt.
Email can be forwarded to any account. 

Serial prints have been commented out to optimize code, uncomment for debug.

*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#ifndef STASSID  //define SSID and Password if not
#define STASSID "VirtualIncision-Internal"  //Internal Network SSID
#define STAPSK  "R0b0tsCutY0u"    //Internal Network Password
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "maker.ifttt.com"; //host
const int httpsPort = 443;   //https port

const char fingerprint[] PROGMEM = "AA 75 CB 41 2E D5 F9 97 FF 5D A0 8B 7D AC 12 21 08 4B 00 8C";   //ifttt trigger SHA1 fingerprint

int ledPin = D7; // pin for the LED
int keepAlivePin = D6; // input for keep alive transistor
int inPin = D5;   // input pin (for reset button)


void setup() {
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode (keepAlivePin, OUTPUT);  //declare keep alive pin as output
  pinMode(inPin, INPUT);    // declare pushbutton as input

  
  Serial.begin(115200);  //begin serial connection at 115200 baud
  Serial.println(); //new line to get past beginning junk
  Serial.print("connecting to ");  
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connect to network using ssid & password
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i <= 33) { //wait until connection established or number of atempts has exceeded The mAgIc NuMbEr
    delay(500);
    i++;
    Serial.print(".");
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    WiFiClientSecure client;  // use WiFiClientSecure class to create TLS connection
    Serial.print("connecting to ");
    Serial.println(host); //host

    Serial.printf("Using fingerprint '%s'\n", fingerprint);
    client.setFingerprint(fingerprint); //assign SHA1 fingerprint

    client.connect(host, httpsPort); //make https request

    String url = "/trigger/tempTrigger/with/key/bv-RJOueif_Hj6XFHWTDS3jpAQgtLWh-9XTzxiiIc7w"; //trigger request extension
    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +   
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");  //assemble URL

    Serial.println("request sent");  
    while (client.connected()) { 
      String line = client.readStringUntil('\n');//read in output
      if (line == "\r") {
        Serial.println("headers received");
        break;
        }
      }
      String line = client.readStringUntil('\n');

    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line); //print result
    Serial.println("==========");
   } else {
     Serial.println("WiFi connection failed");
  }

digitalWrite(keepAlivePin, LOW);  // lock reset line

while (digitalRead(inPin) == HIGH) {  // read input value
  // D1-mini chip io pins can draw more then they can 
  // output so switched wiring in order to make led brighter
    Serial.println("LED blinking...");
    digitalWrite(ledPin, LOW);  // turn LED ON
    delay(500);
    digitalWrite(ledPin, HIGH);  // turn LED OFF
    delay(200);
    digitalWrite(ledPin, LOW);  // turn LED ON
    delay(500);
    digitalWrite(ledPin, HIGH);  // turn LED OFF
    delay(1200);
  }
  Serial.println("loop broken, waiting for reset.");
  delay(1200);
  digitalWrite(ledPin, LOW);   // turn LED ON
  delay(2400);
  digitalWrite(ledPin, HIGH);   // turn LED OFF
  delay(500);
  digitalWrite(keepAlivePin, HIGH);   //enable reset line

// -eof
}

void loop() {
}
