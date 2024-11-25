
//start playing a stream from current active station
void startUrl()
{
  audio.connecttohost(stationlist[actStation].url);
  streamReady = false;
}

// to be called in 'setup()'
void setup_audio()
{
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolumeSteps(55); // max 255, derzeit Vorgabe 55, Einstellung mit Encoder nur bis 50
  audio.setVolume(curVol); // lade gespeicherte Lautstärke
}
void audio_loop()
{
   audio.loop();
}

//**************************************************************************************************
//                                           E V E N T S                                           *
//**************************************************************************************************

void audio_info(const char *info)
{
    Serial.print("audio_info: "); 
    Serial.println(info);
    if (strstr(info, "stream ready") != NULL)     //sucht danach
    {
      streamReady = true;
      showStation();
    }
}
