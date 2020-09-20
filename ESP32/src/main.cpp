#include <WiFi.h>
#include <Arduino.h>
#include <time.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <Ticker.h>
#include "Timer.h"
#include "Weather.h"
#include "Setting.h"
Ticker flip;
const char *ssid = "ThanhHien";
const char *password = "1234qwer";
WiFiClient client;
unsigned long time1 = 0;
unsigned long time2 = 0;
TaskHandle_t Tack1;
TaskHandle_t Tack0;
void codeForTask1(void *parameter)
{
  for (;;)
  {
    delay(1000);
  }
}
void codeForTask0( void * parameter )
{
  for (;;) {
    delay(1000);
  }
}
void setup()
{
  Serial.begin(9600);
  sensors.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  while (nam.toInt() < 2000)
  {
    capNhatThoiGian();
    delay(50);
  }
  flip.attach(1, ngat);
  getWeatherForecastData();

  xTaskCreatePinnedToCore(codeForTask1,"led1Task", 1000, NULL,1,&Tack1,1);
  delay(500);
  xTaskCreatePinnedToCore(codeForTask0,"led2Task",1000,NULL,1,&Tack0,0);
}
void loop()
{
  if ((unsigned long)(millis() - time1) > 20000)
  {
    getWeatherForecastData();
    time1 = millis();
  }
  if ((unsigned long)(millis() - time2) > 1000)
  {
    String st = ngayTrongTuan + "/" + ngay + "/" + thang + "/" + String(nam);
    String sp = String(hour) + "/" + String(minute) + "/" + String(sec);
    time2 = millis();
  }
}