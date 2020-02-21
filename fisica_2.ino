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
#include <HCSR04.h>
ESP8266WebServer server;
/*------------------------------------------------------------------------------
  Set Website content:
------------------------------------------------------------------------------*/
const char responseHTML[] PROGMEM={"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"
"<title>Distribucion de Fluidos</title>\n"
"<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style>\n"
"</head>\n"
"<body>\n"
"<div id=\"webpage\">\n"
"<h1>Sistema de Monitoreo</h1>\n"
"</div>\n"
"</body>\n"
"</html>\n"
};

/*------------------------------------------------------------------------------
  Network and Web server configuration:
------------------------------------------------------------------------------*/

//Define WiFi Name and password:
const char* ssid = "Flow";
const char* password = "SafePass123";

//Set IP address GW and subnet:
IPAddress local_ip(192,168,10,4);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

//Define DNS Server:
const byte DNS_PORT = 53;
DNSServer dnsServer;

//Define HTTP Server:
ESP8266WebServer webServer(80);

/*------------------------------------------------------------------------------
  Sonar 1 configuration:
------------------------------------------------------------------------------*/

//Define Pins for sonas module 1:
#define trigPin1 5
#define echoPin1 4
int heightTank1=10; //Set Height of the container
int deviation1=2; //Set Distance from the maximun height of the liquid
int duration1,distance1,percentage1; 

/*------------------------------------------------------------------------------
  Startup code:
------------------------------------------------------------------------------*/

void setup() {

  pinMode(trigPin1,OUTPUT); //Sets the trigPin1 as an Output
  pinMode(echoPin1,INPUT); //Sets the echoPin1 as an Input

  //Open Serial Port:
  Serial.begin(115200);

/*------------------------------------------------------------------------------
  WIFI AP Configuration:
------------------------------------------------------------------------------*/

  Serial.println();
  //Set Settings:
  Serial.print("Setting soft-AP configuration ... ");

  Serial.println(WiFi.softAPConfig(local_ip, gateway, subnet) ? "Ready" : "Failed!");

  //Start WiFi in AP mode:
  Serial.print("Setting soft-AP ... ");

  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");

  Serial.println(WiFi.softAPIP());

/*------------------------------------------------------------------------------
  Captive portal and Web server start:
------------------------------------------------------------------------------*/

  //Captive portal control:
  dnsServer.start(DNS_PORT, "*", local_ip);

  //Reply all request with same page:
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
    });
  webServer.begin();


}

/*------------------------------------------------------------------------------
  Loop code:
------------------------------------------------------------------------------*/

void loop() {

  dnsServer.processNextRequest(); //DNS Server handler
  webServer.handleClient(); //WEB Server handler
  sonar1(); //Sonar 1 value reporter
}

/*------------------------------------------------------------------------------
  Sonar 1 code, data colector:
------------------------------------------------------------------------------*/
void sonar1(){
  //Pulse and Receive:
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin1,LOW);
  duration1=pulseIn(echoPin1,HIGH);
  //Calculate distance and percentage:
  distance1=(duration1/2)/29.1;
  percentage1=100-(((distance1-deviation1)*100)/heightTank1);
  //Print results:
  Serial.println(distance1);
  Serial.println(percentage1);
  delay(1000);

}  
