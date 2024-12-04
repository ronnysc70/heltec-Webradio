//init WiFi Verbindung
boolean initWiFi(String ssid, String pkey)
{       
  connected = false;
  Serial.println("Connecting to WiFi");
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  if (tryCount < MAXWLANTRY) {
    // Mit Wi-Fi verbinden
    Serial.println("...Connecting to WiFi");
    u8g2.clearBuffer();                           //Heltec
    u8g2.setCursor(0,20);
    u8g2.print("suche WLAN...");
    u8g2.sendBuffer();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(),pkey.c_str());
     while ((WiFi.status() != WL_CONNECTED) && (tryCount<MAXWLANTRY)) 
     {
       Serial.print(".");
       delay(1000);
       tryCount++;
    }
    IPAddress lip(0,0,0,0);
    if (WiFi.localIP() == lip) {
      tryCount = MAXWLANTRY;
    }
  }
  if (tryCount < MAXWLANTRY) {
    Serial.print(F("IP-Adresse per DHCP ist "));
    Serial.println(WiFi.localIP());
    connected = true;
  }
  //we start an access point to allow configuration
  if (!connected) {
     Serial.println("Keine Verbindung! \nStarte Access-Point.");
     WiFi.mode(WIFI_AP);
     WiFi.softAP("webradioconf","");
  }
  return connected;
}


void wifi_loop()
{
  
    if ((millis() - delayTimeRefresh) > 10000)       //aller 10 Sekunden Überprüfung WLAN und Internet Verbindung
    {
      int retry = 0;
      delayTimeRefresh = millis();
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("reconnect WIFI");
        u8g2.clearBuffer();                   //Heltec
        u8g2.setCursor(0,20);
        u8g2.print("kein WLAN..");
        u8g2.setCursor(0,40);
        u8g2.print("..verbinde neu");
        u8g2.sendBuffer();
        WiFi.disconnect();
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(SSID, PSK);
        while (WiFi.status() != WL_CONNECTED)
        {
          Serial.print(".");
          delay(100);
        }
        if (btnStandby)
        {
          showStandby();
        }
        else
        {
          showStation();
        }
      }
      // teste ob eine Verbindung zum Internet besteht wg. capitive Portal
 
      else
      {
        while((!httpsClient.connect("www.google.de", 443, 2000)) && (retry < 3))  //3. Wert ist timeout der Verbindung in ms
        {
          delay(100);
          Serial.print("<*>");
          retry++;   
        }
        if (retry==3)
        {
          Serial.println("keine Verbindung");
          internetLost = true;
        }
        else
        {
          //Serial.println("Internet erreichbar");
          if (internetLost)
          {
            internetLost = false;
            Serial.println("Internet wieder erreichbar, starte Stream neu");
            if (btnStandby)
            {
              showStandby();
            }
            else
            {
              startUrl();
            }
          }
        }
        httpsClient.stop();
      }
    }
}

void stationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
        u8g2.clearBuffer();                   //Heltec
        u8g2.setCursor(0,15);
        u8g2.print("im Browser");
        u8g2.setCursor(0,30);
        u8g2.print("192.168.4.1");
        u8g2.setCursor(0,45);
        u8g2.print("eingeben");
        u8g2.sendBuffer();
}
