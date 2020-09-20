#include "Font.h"
unsigned long time1 = 0;
String cpuName, cpuLoad, cpuTemp, ramLoad, gpuName, gpuLoad, gpuTemp, net;
String oldCPULoad, oldCPUTemp, oldRAMLoad, oldGPULoad, oldGPUTemp, oldNet;
String ngayTrongTuan, thang, ngay, gio, phut, giay, nam;
int hour, minute, sec;
int timer = 0, demFlash, matKetNoiWiFi;
bool lanDauMoApp = true, ketNoiApp, doiWiFi;
WiFiServer server(80);
WiFiManager wifiManager;
WiFiClient client;
#ifdefined(ESP8266)
SSD1306Wire display(0x3C, D2, D1);
const int reset = D3;
#else
SSD1306Wire display(0x3C, 22, 21);
const int reset = 10;
int getBarsSignal(long rssi)
{
    int bars;
    if (rssi > -55)
    {
        bars = 5;
    }
    else if (rssi<-55 & rssi> - 65)
    {
        bars = 4;
    }
    else if (rssi<-65 & rssi> - 75)
    {
        bars = 3;
    }
    else if (rssi<-75 & rssi> - 85)
    {
        bars = 2;
    }
    else if (rssi<-85 & rssi> - 96)
    {
        bars = 1;
    }
    else
    {
        bars = 0;
    }
    return bars;
}
void wifi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        display.drawLine(0, 52, 128, 52);
        String ip = WiFi.localIP().toString();
        display.setFont(ArialMT_Plain_10);
        display.drawString(0, 54, ip);
        long rssi = WiFi.RSSI();
        int bars = getBarsSignal(rssi);
        display.drawLine(0, 52, 128, 52);
        for (int i = 0; i <= bars; i++)
        {
            display.fillRect(117 + (i * 2), 64 - (i * 2), 1, i * 2);
        }
        display.setFont(DSEG7_Classic_Bold_22);
        String h, m, s;
        if (hour < 10)
        {
            h = "0" + String(hour);
        }
        else
        {
            h = String(hour);
        }
        if (minute < 10)
        {
            m = "0" + String(minute);
        }
        else
        {
            m = String(minute);
        }
        if (sec < 10)
        {
            s = "0" + String(sec);
        }
        else
        {
            s = String(sec);
        }
        String sp = h + ":" + m + ":" + s;
        display.drawString(2, 0, sp);
        display.setFont(ArialMT_Plain_16);
        String st = ngayTrongTuan + " " + ngay + " / " + thang + " / " + String(nam);
        display.drawString(0, 30, st);
    }
    else
    {
        display.drawString(0, 54, "There is no connection!");
    }
    if (digitalRead(reset) == LOW)
    {
        wifiManager.resetSettings();
        ESP.restart();
    }
}
void border()
{
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "CPU:");
    display.drawString(0, 17, "GPU:");
    display.drawString(0, 34, "NET:");
    display.drawString(117, 0, "C");
    display.drawString(117, 18, "C");
    display.setFont(ArialMT_Plain_10);
    display.drawString(110, 0, "O");
    display.drawString(110, 18, "O");

    display.drawLine(0, 52, 128, 52);
    String ip = WiFi.localIP().toString();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 54, ip);
    long rssi = WiFi.RSSI();
    int bars = getBarsSignal(rssi);
    display.drawLine(0, 52, 128, 52);
    for (int i = 0; i <= bars; i++)
    {
        display.fillRect(117 + (i * 2), 64 - (i * 2), 1, i * 2);
    }
}
void getData()
{
    if (client.connected())
    {
        if (client.available() > 0)
        {
            String data = client.readStringUntil('\n');
            Serial.println(data);
            DynamicJsonDocument root(300);
            deserializeJson(root, (char *)data.c_str());
            cpuName = root["CPU"]["Name"].as<String>();
            cpuLoad = root["CPU"]["Load"].as<String>();
            cpuTemp = root["CPU"]["Temp"].as<String>();
            gpuName = root["GPU"]["Name"].as<String>();
            gpuLoad = root["GPU"]["Load"].as<String>();
            gpuTemp = root["GPU"]["Temp"].as<String>();
            net = root["Net"]["Speed"].as<String>();
            if (cpuLoad.length() > 5)
                cpuLoad = cpuLoad.substring(0, 5);
            if (lanDauMoApp)
            {
                //border();
                lanDauMoApp = false;
            }
            else
            {
                border();
                display.setFont(ArialMT_Plain_16);
                display.drawString(37, 0, cpuLoad + "%");
                display.setFont(ArialMT_Plain_16);
                display.drawString(85, 0, cpuTemp);
                display.setFont(ArialMT_Plain_16);
                display.drawString(37, 18, gpuLoad + "%");
                display.setFont(ArialMT_Plain_16);
                display.drawString(85, 18, gpuTemp);
                display.setFont(ArialMT_Plain_16);
                display.drawString(37, 34, net);
            }
        }
    }
    else
    {
        display.clear();
        ketNoiApp = false;
    }
}
void capNhatThoiGian()
{
    time_t now = time(nullptr);
    String data = ctime(&now); // Lưu chuỗi nhận được
    ngayTrongTuan = data.substring(0, 3);
    String month = data.substring(4, 7);
    ngay = data.substring(8, 10);  // Ngày để hiển thị
    gio = data.substring(11, 13);  // Giờ để hiển thị
    phut = data.substring(14, 16); // Phút để hiển thị
    giay = data.substring(17, 19); // Giây để hiển thị
    nam = data.substring(20, 24);  // Năm để hiển thị
    ngayTrongTuan.toUpperCase();
    ngay.trim();
    if (ngay.toInt() < 10)
    {
        ngay = "0" + ngay;
    }
    if (month == "Jan")
        thang = "01";
    else if (month == "Feb")
        thang = "02";
    else if (month == "Mar")
        thang = "03";
    else if (month == "Apr")
        thang = "04";
    else if (month == "May")
        thang = "05";
    else if (month == "Jun")
        thang = "06";
    else if (month == "Jul")
        thang = "07";
    else if (month == "Aug")
        thang = "08";
    else if (month == "Sep")
        thang = "09";
    else if (month == "Oct")
        thang = "10";
    else if (month == "Nov")
        thang = "11";
    else if (month == "Dec")
        thang = "12";
    hour = gio.toInt();
    minute = phut.toInt();
    sec = giay.toInt();
}
void ngat()
{
    sec++;
    if (sec == 60)
    {
        minute++;
        sec = 0;
    }
    if (minute == 60)
    {
        hour++;
        minute = 0;
    }
    if (hour == 24)
    { // Sau 1 ngày cập nhật thời gian 1 lần
        hour = 0;
        capNhatThoiGian();
    }
    if (WiFi.status() != WL_CONNECTED)
        WiFi.reconnect();
}