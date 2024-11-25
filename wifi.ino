//init WiFi Verbindung
int setup_wifi()
{       
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
    WiFi.begin(SSID, PSK);
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
    return(true);   // when connection ok, stop here and return positive
  }
  return(false);
}


void wifi_loop()
{
  
    if ((millis() - delayTimeRefresh) > 2000)       //aller 2 Sekunden Überprüfung WLAN und Internet Verbindung
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
          Serial.println("Internet erreichbar");
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
