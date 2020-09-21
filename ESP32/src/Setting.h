#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

float volt = 0.00;
int Blever = 0;
int BatteryPIN = 36;

int OpticalPIN = 39;
int OpticalLever;

int ONE_WIRE_BUS = 32;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temp;

void Battery()
{
        int raw = analogRead(BatteryPIN);
        Blever = map(raw, 2500, 3350, 0, 100);
        if (Blever > 100)
        {
            Blever = 100;
        }
        else if (Blever < 0)
        {
            Blever = 0;
        }
}
void Optical()
{
        int raw = analogRead(OpticalPIN);
        OpticalLever = map(raw, 150, 4095, 0, 100);
        if (OpticalLever > 100)
        {
            OpticalLever = 100;
        }
        else if (OpticalLever < 0)
        {
            OpticalLever = 10;
        }
}
void ReadTemp(){
    sensors.requestTemperatures();
    temp=sensors.getTempCByIndex(0);
}

