#include <Ticker.h>
Ticker flip;
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "data.h"
#include <time.h>
void configModeCallback(WiFiManager *myWiFiManager)
{
  display.clear();
  display.drawString(0, 0, "Connection failed !!!");
  display.drawString(0, 10, "connect to wifi");
  display.drawString(0, 20, myWiFiManager->getConfigPortalSSID());
  display.drawString(0, 30, "to config new wifi");
  display.display();
}
void setup()
{
  Serial.begin(115200);
  pinMode(reset, INPUT_PULLUP);
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Waiting for wifi");
  display.display();

  wifiManager.setAPCallback(configModeCallback);
  if (!wifiManager.autoConnect())
  {
    ESP.reset();
    delay(1000);
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
