#include <Ticker.h>
Ticker flip;
Ticker tick;
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "data.h"
#include <time.h>
void setup()
{
  Serial.begin(115200);
  pinMode(reset, INPUT_PULLUP);
  display.init();
  display.flipScreenVertically();
  wifiManager.setTimeout(30);
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Connecting to wifi");
  display.display();
  if (!wifiManager.autoConnect("MCU"))
  {
    display.clear();
    display.drawString(0, 10, "Restart esp and");
    display.drawString(0, 20, "connect to wiFi MCU");
    display.drawString(0, 30, "to config new wiFi");
    
    display.display();
  }
  
  server.begin();
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  while (nam.toInt() < 2000)
  { 
    capNhatThoiGian();
    delay(50);
  }
  flip.attach(1, ngat);
}
void loop()
{
  display.clear();

  if (ketNoiApp)
  {
    getData();
  }
  else
  {
    if (!client.connected())
    {
      client = server.available();
      wifi();
    }
    else
    {
      display.clear();
      ketNoiApp = true;
      lanDauMoApp = true;
    }
  }
  display.display();
  delay(1000);
}
