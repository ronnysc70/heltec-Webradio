
# Heltec Internet Radio mit MAX98357

Ein Internet-Radio kit Heltec WIFI V3 Board und MAX98357. 


## Bedienung   
nach dem Starten sucht das Board das eingestellte WLAN was in "credentials.h" hinterlegt ist. Bei erfolgreichen Verbinden zum WLAN wird der zuletzt aufgerufene Sender kontaktiert und gestreamt. Wenn der Sender gefunden wurde und funktioniert wird vor den Sendername ein Lautsprecher-Symbol eingeblendet. Die Lautstärke kann im Hauptbildschirm verändert werden. Zum Auswählen eines neuen Senders einmal den Encoder einmal drücken, den neuen Sender wählen und durhc Drücken bestätigen. Durch Doppelclick wird die vergebene IP-Adresse angezeigt. Mit dieser können per Webinterface Radiosender geändert, hinzugefügt und die Reihenfolge verändert werden. Neue Software kann per WiFi übertragen werden. Dazu "IP-Adresse"/update eingeben und den Angaben folgen. Langes Drücken über eine Sekunde beendet den Stream und die Uhrzeit und Datum werden angezeigt.

<ul>
<li>mit:</li>
  <ul>- Breadboard MAX98357</ul>
  <ul>- Breadboard KY 040</ul>
</ul>
<ul>
<li>benutzte Libaries:</li>
  <ul>- U8g2</ul>
  <ul>- audio.h - repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"</ul>
  <ul>- ElegantOTA</ul>
  <ul>- AiEsp32RotaryEncoder</ul>
</ul>
