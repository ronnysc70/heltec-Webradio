
void pressOne()
{
  
  if (!(btnStandby))
  {
    if (btnStation)       //2. kurzer Druck -> Station wird gewählt
    {
        rotaryVol = true;
        btnStation = false;
       //set current station as active station and start streaming
        actStation = curStation;
        Serial.printf("Active station %s\n",stationlist[actStation].name);
        pref.putUShort("station",curStation);
        startUrl();
      //call show station to display the speaker symbol
        showStation();
        rotaryEncoder.setBoundaries(0, maxVol, false); //minValue, maxValue,
        rotaryEncoder.setEncoderValue(curVol);
    }
    else                    //erster kurzer Druck -> Stationsauswahl
    {
      btnStation = true;
      rotaryVol = false;
      rotaryEncoder.setBoundaries(0, STATIONS, true); //minValue, maxValue,
      rotaryEncoder.setEncoderValue(actStation);
      showStation();
      lastchange = millis();
    }
  }
  else              //im Standby durch kurzen Druck wieder zurück zum Radio
  {
    rotaryVol = true;
    startUrl();
    //call show station to display the speaker symbol
    showStation();
    rotaryEncoder.setBoundaries(0, maxVol, false); //minValue, maxValue,
    rotaryEncoder.setEncoderValue(curVol);
    btnStandby = false;
    pref.putBool("standby", btnStandby);
  }
}
void pressTwice()           //Doppelklick -> Anzeige IP-Adresse
{
  if (!(btnStandby))
  {
    btnMode = true;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_courB10_tf);
    u8g2.setCursor(0,12);
    u8g2.print("IP-Adresse: ");
    u8g2.setCursor(0,32);
    u8g2.print(WiFi.localIP().toString().c_str());
    u8g2.sendBuffer();
    lastchange = millis();
  }
}
void onPressLong()          //lange Drücken -> Standby
{
   //Standby-Taster auswerten
   if (!(btnStandby))
   {
      audio.stopSong();
      u8g2.clearBuffer();       //Heltec
      u8g2.setFont(u8g2_font_courB10_tf);
      u8g2.setCursor(20,30);
      u8g2.print("Standby");
      u8g2.sendBuffer();
      delay(2000);
      showStandby();
      btnStandby = true;
      Serial.println("standby");
      pref.putBool("standby", btnStandby);
   }
  
}

void onPressShort()
{
  if (clicks == 0)
  {
    doublePressLimit = millis() + doublePressMax;
    clicks = 1;
    Serial.println("kurzer Tastendruck");
    pressOne();
  }
  else if (clicks == 1 && millis() < doublePressLimit)
  {
    Serial.println("DoppelKlick");
    pressTwice();
    clicks = 0;
    doublePressLimit = 0;
  }
}

void handle_rotary_button()
{
  static unsigned long lastTimeButtonDown = 0;
  static bool wasButtonDown = false;
  bool isEncoderBtnDown = rotaryEncoder.isEncoderButtonDown();

  if (isEncoderBtnDown)
  {
    //Serial.print("*");
    if (!wasButtonDown)
    {
      lastTimeButtonDown = millis();
    }
    wasButtonDown = true;
    return;
  }
  if (wasButtonDown)  //Button up
  {
    //Serial.print("_");
    if (millis() - lastTimeButtonDown > longPressTime)
    {
      onPressLong();
    }
    else if (millis() - lastTimeButtonDown > shortPressTime)
    {
      onPressShort();
    }
  }
  wasButtonDown = false;
  //Zeitlimit abgelaufen, rücksetzen der Variablen
  if (clicks == 1 &&  doublePressLimit != 0 && millis() > doublePressLimit)
  {
    Serial.println("zeit doppelklick abgelaufen");
    doublePressLimit=0;
    clicks=0;
  }
  
  if (!(btnStandby))
  {
   //if no change station happened within 5s set active station as current station
    if (btnStation && (millis() - lastchange) > 5000)
    {
      curStation = actStation;
      lastchange = 0;
      rotaryVol = true;
      btnStation = false;
      showStation();
    }
  //nach 3 sek. Anzeige IP-Adresse zurück zur Stationsanzeige 
    if (btnMode && (millis() - lastchange) > 3000)
    {
      curStation = actStation;
      lastchange = 0;
      rotaryVol = true;
      btnMode = false;
      showStation();
    }
  }
}
