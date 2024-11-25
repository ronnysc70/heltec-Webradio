//default stationlist
#define DEFAULTSTATIONS 10

Station defstations[DEFAULTSTATIONS] PROGMEM = {
{"https://mdr-284290-2.sslcast.mdr.de/mdr/284290/2/mp3/high/stream.mp3","MDR Sachsen Anhalt",1},
{"http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3","WDR1",1},
{"http://st01.sslstream.dlf.de/dlf/01/128/mp3/stream.mp3","Deutschlandfunk",1},
{"http://stream.89.0rtl.de/live/mp3-256/", "89,0 RTL",1},
{"http://dispatcher.rndfnk.com/br/br3/live/mp3/low","Bayern3",1},
{"https://absolut-relax.live-sm.absolutradio.de/absolut-relax/stream/mp3", "Absolut Relax",1},
{"http://rnrw.cast.addradio.de/rnrw-0182/deinschlager/low/stream.mp3","NRW Schlagerradio",1},
{"http://mdr-284331-2.sslcast.mdr.de/mdr/284331/2/mp3/high/stream.mp3","MDR Sputnik in the mix",1},
{"http://stream.89.0rtl.de/mix/mp3-256/play.m3u", "89,0 RTL in the Mix",1}, 
{"http://stream.sunshine-live.de/live/mp3-192","Sunshine Live",1}
};


void setup_senderList() {
  char nkey[4];
  char ukey[4];
  char fkey[4];
  for (uint8_t i = 0; i<STATIONS; i++) {
    sprintf(nkey,"n%i",i);
    sprintf(ukey,"u%i",i);
    sprintf(fkey,"f%i",i);
    if (i<DEFAULTSTATIONS) {
      strlcpy(stationlist[i].name,sender.getString(nkey,defstations[i].name).c_str(),32);
      strlcpy(stationlist[i].url,sender.getString(ukey,defstations[i].url).c_str(),150);
      stationlist[i].enabled = sender.getUChar(fkey,defstations[i].enabled);
    } else {
      strlcpy(stationlist[i].name,sender.getString(nkey,nkey).c_str(),32);
      strlcpy(stationlist[i].url,sender.getString(ukey,"").c_str(),150);
      stationlist[i].enabled = sender.getUChar(fkey,0);
    }
  }
}

void restore() {
  char nkey[4];
  char ukey[4];
  char fkey[4];
  for (uint8_t i = 0; i<STATIONS; i++) {
    sender.clear();
    sprintf(nkey,"n%i",i);
    sprintf(ukey,"u%i",i);
    sprintf(fkey,"f%i",i);
    if (i<DEFAULTSTATIONS) {
      strlcpy(stationlist[i].name,defstations[i].name,32);
      sender.putString(nkey,defstations[i].name);
      strlcpy(stationlist[i].url,defstations[i].url,150);
      sender.putString(ukey,defstations[i].url);
      stationlist[i].enabled = defstations[i].enabled;
      sender.putUChar(fkey,defstations[i].enabled);
    } else {
      strlcpy(stationlist[i].name,nkey,32);
      sender.putString(nkey,nkey);
      strlcpy(stationlist[i].url,"",150);
      sender.putString(ukey,"");
      stationlist[i].enabled = 0;
      sender.putUChar(fkey,0);
    }
  }
}

void saveList() {
  char nkey[4];
  char ukey[4];
  char fkey[4];
  for (uint8_t i = 0; i<STATIONS; i++) {
    sprintf(nkey,"n%i",i);
    sprintf(ukey,"u%i",i);
    sprintf(fkey,"f%i",i);
    sender.putString(nkey,stationlist[i].name);
    sender.putString(ukey,stationlist[i].url);
    sender.putUChar(fkey,stationlist[i].enabled);
  }
  Serial.println("Liste gespeichert");
}

void reorder(uint8_t oldpos, uint8_t newpos) {
  Station temp;
  if ((oldpos < STATIONS) && (newpos  < STATIONS)) {
    if (oldpos > newpos) {
      memcpy(&temp,&stationlist[oldpos],sizeof(Station));
      for (uint8_t i = oldpos; i>newpos; i--) {
        memcpy(&stationlist[i],&stationlist[i-1],sizeof(Station));
      }
      memcpy(&stationlist[newpos],&temp,sizeof(Station));
    } else {
      memcpy(&temp,&stationlist[oldpos],sizeof(Station));
      for (uint8_t i = oldpos; i<newpos; i++) {
        memcpy(&stationlist[i],&stationlist[i+1],sizeof(Station));
      }
      memcpy(&stationlist[newpos],&temp,sizeof(Station));
    }
  }
}
