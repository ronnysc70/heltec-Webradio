
//show Standby Anzeige
void showStandby()
{
  time(&now); // read the current time
  localtime_r(&now, &tm);             // update the structure tm with the current time
  
  u8g2.clearBuffer();       //Heltec
  u8g2.setCursor(35,20);
  sprintf(ausgabe, "%02d:%02d", tm.tm_hour, tm.tm_min);
  u8g2.print(ausgabe);
  u8g2.setCursor(15,40);
  sprintf(ausgabe, "%02d.%02d.%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  u8g2.print(ausgabe);
  u8g2.sendBuffer();
}
//show name of current station on LCD display
//show the speaker symbol in front if current station = active station
void showStation() 
{
  u8g2.clearBuffer();
  if (btnStation)
  {
    u8g2.setCursor(0,20);
    u8g2.print(curStation + 1);
    u8g2.print(":");
  }
  else
  {
    if (!(streamReady))
    {
      u8g2.setCursor(0,20);
    }
    else
    {
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_siji_t_6x10);
      u8g2.drawGlyph(0,20, 0xe05d);
      u8g2.setFont(u8g2_font_courB10_tf);
      u8g2.setCursor(15,20);
    }
  }
 
  String name = String(stationlist[curStation].name);
  if (name.length() < 12)
  {
    u8g2.print(name);
  }
  else 
  {
    uint8_t p = name.lastIndexOf(" ",13); //if name does not fit, split line on space
    u8g2.print(name.substring(0,p));
    u8g2.setCursor(0,40);
    u8g2.print(name.substring(p+1,p+17));
  }
  u8g2.sendBuffer();
 
}
