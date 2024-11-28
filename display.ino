
//show Standby Anzeige
void showStandby()
{
  time(&now); // read the current time
  localtime_r(&now, &tm);             // update the structure tm with the current time
  
  u8g2.clearBuffer();       //Heltec
  u8g2.setCursor(35,20);
  u8g2.setFont(u8g2_font_courB10_tf);
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
  int yLine = 0;
  u8g2.clearBuffer();
  String name = String(stationlist[curStation].name);
  if (btnStation)
  {
    u8g2.setCursor(0,12);
    u8g2.setFont(u8g2_font_courB10_tf);
    u8g2.print(curStation + 1);
    u8g2.print(":");
  }
  else
  {
    if (name.length() < 12)
    {
      yLine = 28;
    }
    else
    {
      yLine = 12;
    }
    if (!(streamReady))
    {
      u8g2.setCursor(0,yLine);
    }
    else
    {
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_siji_t_6x10);
      u8g2.drawGlyph(0,yLine, 0xe05d);
      u8g2.setFont(u8g2_font_courB10_tf);
      u8g2.setCursor(15,yLine);
    }
  }
 
  if (name.length() < 12)
  {
    u8g2.print(name);
  }
  else 
  {
    uint8_t p = name.lastIndexOf(" ",13); //if name does not fit, split line on space
    u8g2.print(name.substring(0,p));
    u8g2.setCursor(0,32);
    u8g2.print(name.substring(p+1,p+17));
  }
  u8g2.sendBuffer();
 
}
void display_loop()
{
  if ( (isSongText) && (!(btnStation)) )
  { 
    int len = songText.length()+1;        //Länge des Strings ermitteln
    char displayText[len];                //Array anlegen
    songText.toCharArray(displayText, len);   //String ins Array schreiben mit Nullterminierung
    u8g2.clearBuffer();            // clear the complete internal memory
    // draw the scrolling text at current offset
    u8g2.setFont(u8g2_font_courB10_tf);  // set the target font for the text width calculation
    u8g2.drawUTF8(
    pixel_area_x_pos-width+offset, 
    pixel_area_y_pos+pixel_area_height+u8g2.getDescent()-1, 
    displayText);                // draw the scolling text
  
    // now only update the selected area, the rest of the display content is not changed
    u8g2.updateDisplayArea(tile_area_x_pos, tile_area_y_pos, tile_area_width, tile_area_height);  
    offset--;               // scroll by one pixel
    if ( offset == 0 ) 
    { 
      offset = width+pixel_area_width;      // start over again
    }
    delay(10);
  }
}
