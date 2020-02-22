/*------------------------------------------------------------------------------
  20/02/2020
  Author: Esterlin Polanco
  Platforms: ESP8266
  Language: C#/Arduino
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

#include "index.h"  //Web page header file

//Define WiFi Name and password for WIFI Client:
const char* ssid = "New Line";
const char* password = "cinema00";

//Define WiFi Name and password for AP:
const char* ssid1 = "Flow";
const char* password1 = "SafePass123";

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
  Main Website, set content, reply:
------------------------------------------------------------------------------*/

void handleRoot() {
 webServer.send(200, "text/html", MAIN_page); //Send web page
}

/*------------------------------------------------------------------------------
  Sonar 1 configuration:
------------------------------------------------------------------------------*/

//Define Pins for sonas module 1:
#define trigPin1 5
#define echoPin1 4

/*------------------------------------------------------------------------------
  Sonar 1 code, data colector:
------------------------------------------------------------------------------*/
void sonar1(){
  //Set variables:
  int heightTank1=10; //Set Height of the container
  int deviation1=2; //Set Distance from the maximun height of the liquid
  
  //Initializa calculation variables:
  int duration1,distance1,percentage1;
  
  //Pulse and Receive:
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin1,LOW);
  duration1=pulseIn(echoPin1,HIGH);
  
  //Calculate distance and percentage:
  distance1=(duration1/2)/29.1;
  percentage1=100-(((distance1-deviation1)*100)/heightTank1);
  
  //Convert result to String:
  String amount1 = String(percentage1);
  
  //Send sonar 1 value only to client ajax request
  webServer.send(200, "text/plane", amount1); 
  
  //Wait 1 second to re calculate:
  delay(1000);

}  


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

  Serial.println(WiFi.softAP(ssid1, password1) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");

  Serial.println(WiFi.softAPIP());
  

/*------------------------------------------------------------------------------
  WIFI Client Configuration:
  ------------------------------------------------------------------------------
  //Connect to WIFI
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");

  //Show WIFI Client IP:
  Serial.println(WiFi.localIP());  
*/

/*------------------------------------------------------------------------------
  Start Web Server:
  ------------------------------------------------------------------------------*/
  
  //Display page:
  webServer.on("/", handleRoot);
  
  //Captive portal control:
  dnsServer.start(DNS_PORT, "*", local_ip);
  
  //Reply as captive portal:
  webServer.onNotFound(handleRoot);

  //To get update of Sonar1:      
  webServer.on("/sonar1", sonar1);
  
  //Start server:
  webServer.begin();                  
  Serial.println("HTTP server started");

}
/*------------------------------------------------------------------------------
  Loop code:
------------------------------------------------------------------------------*/

void loop() {
  //captive portal
  dnsServer.processNextRequest();
  //WEB Server handler:
  webServer.handleClient(); 
  delay(1);
}

