
//handle events from rotary encoder
void rotary_loop()
{
   //process rotary encoder change
    if (rotaryEncoder.encoderChanged())
    {
      uint16_t v = rotaryEncoder.readEncoder();
      if (!(rotaryVol))
      {
        Serial.printf("Station: %i\n",v);
        uint8_t cnt = 0; //overflow counter, prevents endless loop if no station is enabled
        while ((!stationlist[v].enabled) && (cnt < 2))
        {
          v++;
          if (v >= STATIONS)
          {
            v=0;
            cnt++;
          }
        }
        //set new currtent station and show its name
        if (v < STATIONS) 
        {
          curStation = v;
          showStation();
          lastchange = millis();
        }
      }
      else
      {
        
        u8g2.clearBuffer();             //Heltec
        u8g2.setCursor(0,30);
        sprintf(ausgabe,"Volume: %d", (v*2));
        u8g2.print(ausgabe);
        u8g2.print("%");
        u8g2.sendBuffer();
        
        audio.setVolume(v); // default 0...21
        curVol = v;
        lastchange = millis();
      }
    }

  //if no change volume happened within 2s show Stationname
    if (rotaryVol && lastchange > 0 && (millis() - lastchange) > 2000)
    {
      pref.putUShort("volume",curVol);
      lastchange = 0;
      showStation();
    }
 
}

//interrupt handling for rotary encoder
void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();
}

void setup_rotary()
{
  //start rotary encoder instance
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, maxVol, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.setEncoderValue(curVol);
  rotaryEncoder.disableAcceleration();  
}
