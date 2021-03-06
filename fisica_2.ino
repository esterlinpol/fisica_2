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
#include <Wire.h>

#include "index.h"  //Web page header file

//Define WiFi Name and password for WIFI Client:
const char* ssid = "JOSEPOLANCO";
const char* password = "contrawifi";

//Define WiFi Name and password for AP:
const char* ssid1 = "Flow";
const char* password1 = "SafePass123";

//Set IP address GW and subnet:
IPAddress local_ip(192,168,10,4);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

//Define DNS Server for captive portal:
const byte DNS_PORT = 53;
DNSServer dnsServer;

//Define HTTP Server:
ESP8266WebServer webServer(80);

//Define Relay pin for pump:
#define pumppin 14
bool pumppinstate = false;

//Sonar 2 pump activation value:
int s2min = 20;
int s2max = 90;

//Flow sensor data variable:
String flowlm;

//i2c slave address with flow sensor:
const byte SlaveDeviceId = 1;

/*------------------------------------------------------------------------------
  Main Website, set content, reply:
------------------------------------------------------------------------------*/

  void handleRoot() {
    webServer.send(200, "text/html", MAIN_page); //Send web page
  }
  
  void handleNotFound(){
    webServer.send(200, "text/html", MAIN_page);
  }

/*------------------------------------------------------------------------------
  Sonar 1 and 2 configuration:
------------------------------------------------------------------------------*/

  //Define Pins for sonas module 1:
  #define trigPin1 0
  #define echoPin1 2
  
  //Define Pins for sonas module 2:
  #define trigPin2 12
  #define echoPin2 13
  

/*------------------------------------------------------------------------------
  Sonar 1 code, data colector:
------------------------------------------------------------------------------*/
  void sonar1(){
  //Set variables:
  int heightTank1=20; //Set Height of the container
  int deviation1=6; //Set Distance from the maximun height of the liquid
  
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
  Sonar 2 code, data colector:
------------------------------------------------------------------------------*/
  void sonar2(){
  //Set variables:
  int heightTank2=10; //Set Height of the container
  int deviation2=5; //Set Distance from the maximun height of the liquid
  
  //Initializa calculation variables:
  int duration2,distance2,percentage2;
  
  //Pulse and Receive:
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin2,LOW);
  duration2=pulseIn(echoPin2,HIGH);
  
  //Calculate distance and percentage:
  distance2=(duration2/2)/29.1;
  percentage2=100-(((distance2-deviation2)*100)/heightTank2);
  
  //Convert result to String:
  String amount2 = String(percentage2);
  
  //Send sonar 1 value only to client ajax request
  webServer.send(200, "text/plane", amount2); 

  //Activate pump on s2min water percentage:
  if (percentage2<=s2min) {
    digitalWrite(pumppin, HIGH);
    }
  else if (percentage2==s2max){
    digitalWrite(pumppin, LOW);
  }   
  
  //Wait 1 second to re calculate:
  delay(1000);

 }  


/*------------------------------------------------------------------------------
  Flow sensor, data colector:
------------------------------------------------------------------------------*/
  //If pump pin is HIGH, Reads serial information from arduino nano on Serial1:
  void flow(){
    if (pumppinstate==true) {
      
      // Request data from slave.
      Wire.beginTransmission(SlaveDeviceId);
      int available = Wire.requestFrom(SlaveDeviceId, (uint8_t)2);
      
      if(available == 2)
      {
        int receivedValue = Wire.read() << 8 | Wire.read(); 
        flowlm = receivedValue/10000;
        Serial.println(flowlm); 
      }
      else
      {
        Serial.print("Unexpected number of bytes received: ");
        Serial.println(available);
      }

      int result = Wire.endTransmission();
      if(result)
      {
        Serial.print("Unexpected endTransmission result: ");
        Serial.println(result);
      }

      webServer.send(200, "text/plane", flowlm);
    }
    else {
      webServer.send(200, "text/plane", "0");
    }
    
  } 

/*------------------------------------------------------------------------------
  WIFI AP Configuration:
------------------------------------------------------------------------------*/
  void startAP(){
    Serial.println();
    //Set Settings:
    Serial.print("Setting soft-AP configuration ... ");

    Serial.println(WiFi.softAPConfig(local_ip, gateway, subnet) ? "Ready" : "Failed!");

    //Start WiFi in AP mode:
    Serial.print("Setting soft-AP ... ");

    Serial.println(WiFi.softAP(ssid1, password1) ? "Ready" : "Failed!");

    Serial.print("Soft-AP IP address = ");

    Serial.println(WiFi.softAPIP());
  }

/*------------------------------------------------------------------------------
  WIFI Client Configuration:
------------------------------------------------------------------------------*/
  
  void wificl(){

    //Connect to WIFI
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
  }

/*------------------------------------------------------------------------------
  Startup code:
------------------------------------------------------------------------------*/

void setup() {
  
  
  //Set Pump pin as an output and turns it off:
  pinMode(pumppin,OUTPUT); 
  digitalWrite(pumppin, LOW);
  
  //Set sonar 1 pins:
  pinMode(trigPin1,OUTPUT); 
  pinMode(echoPin1,INPUT); 
  
  //Set sonar 2 pins:
  pinMode(trigPin2,OUTPUT); 
  pinMode(echoPin2,INPUT); 
    
  //Open Serial Port for pc logs:
  Serial.begin(115200);

  //Open Serial Port for flow sensor:
  Serial1.begin(115200);
  
  //Start Wifi AP:
  startAP();
  
  //Connect to WIFI as client:
  wificl();
  dnsServer.start(DNS_PORT, "*", local_ip);

  // Start i2c bus as master
  Wire.begin(); 

/*------------------------------------------------------------------------------
  Start Web Server:
------------------------------------------------------------------------------*/
  
  //Display page:
  webServer.on("/", handleRoot);
 
  //To get update of Sonar1:      
  webServer.on("/sonar1", sonar1);

  //To get update of Sonar1:      
  webServer.on("/sonar2", sonar2);

  //To get update of Sonar1:      
  webServer.on("/flow", flow);
  
  //Try to handle not found:
  webServer.onNotFound(handleNotFound); 

  //Start server:
  webServer.begin();                  
  Serial.println("HTTP server started");

}
/*------------------------------------------------------------------------------
  Loop code:
------------------------------------------------------------------------------*/

void loop() {
  
  //WEB Server handler:
  webServer.handleClient(); 
  delay(1);
  
  
  
  //dnsServer.processNextRequest();
}
