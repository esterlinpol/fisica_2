/*------------------------------------------------------------------------------
  20/02/2020
  Author: Esterlin Polanco
  Platforms: ESP8266
  Language: C++/Arduino
  File: fisica_2.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for Water control system, built for physics 2 class at Dominico Americano
  University, Santo Domingo D.R.
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
ESP8266WebServer server;
//Define WiFi Name and password:
char* ssid = "Flow";
char* password = "12345678";

//Set IP address GW and subnet:
IPAddress local_ip(192,168,10,4);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

//Define DNS Server:
const byte DNS_PORT = 53;
DNSServer dnsServer;

//Define HTTP Server:
ESP8266WebServer webServer(80);

String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
                      "be redirected here.</p></body></html>";

void setup() {


//Open Serial Port:
Serial.begin(115200);

Serial.println();
//Set Settings:
Serial.print("Setting soft-AP configuration ... ");

Serial.println(WiFi.softAPConfig(local_ip, gateway, subnet) ? "Ready" : "Failed!");

//Start WiFi in AP mode:
Serial.print("Setting soft-AP ... ");

Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

Serial.print("Soft-AP IP address = ");

Serial.println(WiFi.softAPIP());

//Captive portal control:
dnsServer.start(DNS_PORT, "*", local_ip);

//Reply all request with same page:
webServer.onNotFound([]() {
  webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();


}

void loop() {
dnsServer.processNextRequest();
webServer.handleClient();
}
