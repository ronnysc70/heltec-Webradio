/*
 * Heltec V3 Webradio mit MAX98357
 * Bedienung nur mit Encoder: Einzel-Click= Senderwahl, Doppelklick=IP-Adresse, langer Click=Standby
 *
 */


#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>
#include <NetworkClientSecure.h>
#include <Arduino.h>
#include <Wire.h>
#include <time.h>

NetworkClientSecure httpsClient;

/* Configuration of NTP */
// choose the best fitting NTP server pool for your country
#define NTP_SERVER "de.pool.ntp.org"

// choose your time zone from this list
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"

//home page and template for options
#include "index.h"
//Webserver instance
WebServer server(80);

//esp32 library to save preferences in flash
#include <Preferences.h>

//library für OLED
#include <U8g2lib.h>                    /* geändert für heltec */
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 21, /* clock=*/ 18, /* data=*/ 17);

//Audio instance
#include "Audio.h" //see repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"
// Digital I/O use
#define I2S_DOUT      48      //Heltec
#define I2S_BCLK      47
#define I2S_LRC       33

Audio audio;

//WiFI Einstellungen in der credentials.h
#include <credentials.h>
#define MAXWLANTRY 10  // try to connect with stored credentials MAXWLANTRY times
int tryCount = 0;


//library for rotary encoder
#include "AiEsp32RotaryEncoder.h"
#define ROTARY_ENCODER_A_PIN 39     //Heltec
#define ROTARY_ENCODER_B_PIN 38
#define ROTARY_ENCODER_BUTTON_PIN 37
#define ROTARY_ENCODER_VCC_PIN -1 
//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

//instance for rotary encoder
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

//structure for station list
typedef struct {
  char url[150];  //stream url
  char name[32];  //stations name
  uint8_t enabled;//flag to activate the station
} Station;
#define STATIONS 20 //number of stations in the list

//gloabal variables
Station stationlist[STATIONS];

//instance of prefernces
Preferences pref;
Preferences sender;       //für Senderliste
//button Encoder
unsigned long longPressTime = 1000;
unsigned long shortPressTime = 25;
unsigned long doublePressLimit = 0;
unsigned long doublePressMax = 700; 

//global variables
char ausgabe[20];                   // für sprintf
unsigned int curStation = 0;   //index for current selected station in stationlist
unsigned int actStation = 0;   //index for current station in station list used for streaming 
unsigned long lastchange = 0;  //time of last selection change
unsigned int maxVol = 50; //maximale Einstellung Lautstärke
unsigned int curVol;      //gespeicherte Lautstärke   
bool btnStation = false;  //Taster "Fav"
bool rotaryVol = true;    //Encoder betätigt Lautstärke
bool streamReady = false; //Stream läuft
bool internetLost = false; //Internet DNS nicht erreichbar
bool updateIsRunning = false;   //Update läuft
bool btnMode = false;     //Taste "Mode" gedrückt
bool btnStandby = false;     //Taste Standby gedrückt
time_t now;                          // this are the seconds since Epoch (1970) - UTC
tm tm;                             // the structure tm holds time information in a more convenient way *
unsigned long delayTimeRefresh;
unsigned int clicks = 0;

//OTA Programme
void onOTAStart()
{
  Serial.println("OTA Update startet");
  audio.stopSong();
  u8g2.clearBuffer();       //Heltec
  u8g2.setCursor(0,20);
  u8g2.print("Firmware Update");
  u8g2.sendBuffer();
  updateIsRunning = true;
}
void onOTAEnd(bool success)
{
  if (success)
  {
    Serial.println("Update erfolgreich");
    u8g2.clearBuffer();       //Heltec
    u8g2.setCursor(0,20);
    u8g2.print("Firmware Update");
    u8g2.setCursor(0,40);
    u8g2.print("fertig, Neustart");
    u8g2.sendBuffer();
    delay(3000);
    ESP.restart();
  }
  else
  {
    Serial.println("Update fehlerhaft");
     u8g2.clearBuffer();       //Heltec
    u8g2.setCursor(0,20);
    u8g2.print("Firmware Update");
    u8g2.setCursor(0,40);
    u8g2.print("!!!Fehler!!!");
    u8g2.sendBuffer();
    delay(3000);
    ESP.restart();
  }
}

//setup
void setup() 
{
  //Init Serial
  Serial.begin(115200);
  
  curStation = 0;
  curVol = 2;
  //start preferences instance
  pref.begin("radio", false);
  sender.begin("senderlist",false);
	//set current station to saved value if available
	if (pref.isKey("station")) curStation = pref.getUShort("station");
	if (curStation >= STATIONS) curStation = 0; //check to avoid invalid station number
  if (pref.isKey("volume")) curVol = pref.getUShort("volume");      //EEPROM volume lesen
  if (pref.isKey("standby")) btnStandby = pref.getBool("standby");      //EEPROM Standby lesen
  Serial.printf("Gespeicherte Lautstärke %i\n",curVol);
  Serial.printf("Gespeicherte Station %i von %i\n",curStation,STATIONS);
  
  //set active station to current station 
  actStation = curStation;
  if (curVol < 10)
  {
    curVol = 10;
  }
  //init Rotary / Buttons  
  setup_rotary();
  pinMode (ROTARY_ENCODER_BUTTON_PIN, INPUT_PULLUP); 
   //Setup Audio
  setup_audio();

  u8g2.begin();               //Heltec
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_courB10_tf);
  u8g2.clearBuffer();

  //Setup Timeserver
  // ESP32 seems to be a little more complex:
  configTime(0, 0, NTP_SERVER);  // 0, 0 because we will use TZ in the next line
  setenv("TZ", MY_TZ, 1);            // Set environment variable with your time zone
  tzset();
  
  //init Wifi
  while (!setup_wifi()) 
  {
    Serial.println("Cannot connect :(");
    u8g2.clearBuffer();                   //Heltec
    u8g2.setCursor(0,20);
    u8g2.print("WLAN nicht da...");
    u8g2.sendBuffer();
    
    delay(3000);
    ESP.restart();
  }
  //init Server
  setup_senderList(); //load station list from preferences
	setup_webserver();
  Serial.println("Webserver läuft");
  //init OTA
  ElegantOTA.begin(&server);
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onEnd(onOTAEnd);

  if (!(btnStandby))
  {
    //show on display and start streaming setEncoderValue(curStation);
    startUrl();
    showStation();  
  }
  else
  {
    //lcd.clear();
    showStandby();
  }
  delayTimeRefresh = millis();
  httpsClient.setInsecure();
}

void loop() 
{
  if (!(updateIsRunning))
  {
    if (!(btnStandby))
    {
      audio_loop();
      rotary_loop();
      if (!(btnStation))
      {
        wifi_loop();
      }
    }
    else        //Standby gewählt
    {
      if ((millis() - delayTimeRefresh) > 1000)     //jede Sekunde Aktualisierung der Uhrzeit und Datum
      {
        delayTimeRefresh = millis();
        showStandby();
      }
    }
    //read events from rotary button
    handle_rotary_button();
  }  
  //Check for http requests
	webserver_loop();
  ElegantOTA.loop(); 
}
