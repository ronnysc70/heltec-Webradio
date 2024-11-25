
# Heltec Internet Radio mit MAX98357

Ein Internet-Radio mit Heltec WIFI V3 Board und MAX98357. 

## Aufbau
nach dem Schaltplan unter /hardware/verkabelung.png die Boards verbinden. An das Audioboard einen kleine Lautsprecher anschließen und das Heltec Board mit +5V versorgen, entweder über die USB-C Buchse oder den 5V-Anschlüssen.

## Bedienung   
nach dem Starten sucht das Heltec-Board das eingestellte WLAN was in "credentials.h" hinterlegt ist. Bei erfolgreichen Verbinden zum WLAN wird der zuletzt aufgerufene Sender kontaktiert und gestreamt. Wenn der Sender gefunden wurde und funktioniert wird vor den Sendername ein Lautsprecher-Symbol eingeblendet. Die Lautstärke kann im Hauptbildschirm mit dem Encoder verändert werden. Zum Auswählen eines neuen Senders den Encoder einmal drücken, den neuen Sender wählen und durch Drücken bestätigen. Durch Doppelclick wird die vergebene IP-Adresse angezeigt. Mit dieser können per Webinterface Radiosender geändert, hinzugefügt und die Reihenfolge verändert werden. Neue Software kann per WiFi übertragen werden. Dazu "IP-Adresse"/update eingeben und den Angaben folgen. Langes Drücken über eine Sekunde beendet den Stream und die Uhrzeit und Datum werden angezeigt.

<ul>
<li>mit:</li>
  <ul>- breakout Board MAX98357</ul>
  <ul>- breakout Board KY-040</ul>
</ul>
<ul>
<li>benutzte Libaries:</li>
  <ul>- U8g2</ul>
  <ul>- audio.h - repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"</ul>
  <ul>- ElegantOTA</ul>
  <ul>- AiEsp32RotaryEncoder</ul>
</ul>
