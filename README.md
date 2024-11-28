
# Heltec Internet Radio mit MAX98357

Ein Internet-Radio mit Heltec WIFI V3 Board und MAX98357. <br>
Basis ist: <a href="https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/internetradio-mit-esp32-und-max-98357a">azdelivery Internetradio</a>


## Aufbau
nach dem Schaltplan unter /hardware/verkabelung.png die Boards verbinden. An das Audioboard einen kleine Lautsprecher anschließen und das Heltec Board mit +5V versorgen, entweder über die USB-C Buchse oder den 5V-Anschlüssen.

## Software
die entsprechenden Libraries über den Library-Manager von Arduino installieren, dabei nicht vergessen die ESP Core im Boardverwalter einzurichten. Die Audio-Library muss per Hand eingebunden werden.
Siehe hierzu: <a href="https://draeger-it.blog/arduino-ide-einbinden-einer-bibliothek/">https://draeger-it.blog/arduino-ide-einbinden-einer-bibliothek/</a>. Danach nicht vergessen in der Datei "credentials.h" SSID und Passwort des eigenen WLAN einzutragen. Zum Compilieren "Heltec Wifi Kit 32(V3)" wählen.

## Bedienung   
nach dem Starten sucht das Heltec-Board das eingestellte WLAN was in "credentials.h" hinterlegt ist. Bei erfolgreichen Verbinden zum WLAN wird der zuletzt aufgerufene Sender kontaktiert und gestreamt. Wenn der Sender gefunden wurde und funktioniert wird vor den Sendername ein Lautsprecher-Symbol eingeblendet. Die Lautstärke kann im Hauptbildschirm mit dem Encoder verändert werden. Zum Auswählen eines neuen Senders den Encoder einmal drücken, den neuen Sender wählen und durch Drücken bestätigen. Durch Doppelclick wird die vergebene IP-Adresse angezeigt. Mit dieser können per Webinterface Radiosender geändert, hinzugefügt und die Reihenfolge verändert werden. Neue Software kann als binary-Datei per WiFi übertragen werden. Dazu "IP-Adresse"/update eingeben und den Angaben folgen. Langes Drücken über eine Sekunde beendet den Stream und die Uhrzeit und Datum werden angezeigt.<br>
28.11.2024 Neu: in der untersten Zeile wird der Songtext als Laufschrift angezeigt.

<ul>
<li>Hardware:</li>
  <ul>- Heltec WiFi V3 Board</ul>
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
