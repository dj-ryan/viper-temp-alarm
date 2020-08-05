# viper-temp-alarm
A short Arduino script that sends an email when reset (by relay) to any specified client using IFTTT

OM-THA2-U Temperature/Humidity/Dewpoint Alarm's (120-000060-000) NO (normaly open) relay output 
connected to reset pin of WeMos D1 Mini ESP8266 chip. When tripped opens curcit and resets 
D1 Mini to run this program.

D1 Mini driver:
https://wiki.wemos.cc/downloads

Board Manager:
http://arduino.esp8266.com/stable/package_esp8266com_index.json


setup:
Connects to Internal WiFi and triggers an event on ifttt. ifttt sends email to a
dummy google account. Sets keepAlivePin to HIGH to
prevent reset upon Alarm reset. Blinks LED light and waits for a button input to 
break loop.  Sets keepAlivePin to low to allow reset.

loop:
na

Serial prints have been commented out to optimize code, uncomment for debug.

