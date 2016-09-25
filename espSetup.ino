/*
  ESP8266 mDNS responder sample

  This is an example of an HTTP server that is accessible
  via http://esp8266.local URL thanks to mDNS responder.

  Instructions:
  - Update WiFi SSID and password as necessary.
  - Flash the sketch to the ESP8266 board
  - Install host software:
    - For Linux, install Avahi (http://avahi.org/).
    - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
    - For Mac OSX and iOS support is built in through Bonjour already.
  - Point your browser to http://esp8266.local, you should see a response.

 */


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include"wifiSetupMenu.h"

const char* ssid = "............";
const char* password = "..............";

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);

void setup(void)
{
  Serial.begin(115200);
  initWifiSetupMenu();
  Serial.println("Started");
}


void loop(void)
{
  runWifiSetupMenu();
}
