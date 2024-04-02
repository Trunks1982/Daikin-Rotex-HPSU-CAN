
"Ein Kaffee für nächste lange Nacht vor dem Rechner wäre toll" ist aber natürlich kein muss.
<br>[![Paypal](https://user-images.githubusercontent.com/41958506/212499642-b2fd097a-0938-4bfc-b37b-74df64592c58.png)](https://www.paypal.com/donate/?hosted_button_id=H94LZRQW9PFQ4)

# Rotex /Daikin HPSU CAN



Hi ich möchte mein erstes kleines Projekt vorstellen, ich wollte eine flexible Lösung die Standalone, mit Homeassistant und Iobroker läuft,um meine Rotex HPSU Compact mittels CAN BUS auszulesen und zu steuern.
Verwendet habe ich ein ESP32-S3-WROOM Board (es sollte jeder ab S1 funktionieren, weil diese die 20kbits Baudrate unterstützen,
die normalen ESP32 Wroom funktionieren nicht weil diese erst ab 50kbit funktionieren) und ein Waveshare SN65HVD230 CAN Board mit 3,3V.
Ein ESP32 S3 mini (Zero) funktioniert auch.

## Installationsanleitung:
1. Wir benötigen um den ESP32 zu flashen einmal den Browser Chrome (Safari und Firefox werden nicht unterstützt).Dann benötigen wir die Datei rotex-factory.bin.
2. Wir öffnen die Website https://web.esphome.io und verbinden den ESP32 mit dem PC.Nach dem verbinden klicken wir auf der Website auf Connect.Danach öffnet sich ein Fenster wo wir die USBSerial Schnitstelle auswahlen.
3. Danach öffnet sich ein neues Fenster auf der Website wo wir auf Installieren klicken.
4. Jetzt klicken wir auf Datei auswählen und suchen die vorher runtergeladene rotex-factory.bin aus und klicken auf installieren.Dieser Vorgang benötigt ca 2min.
5. Nach erfolgreicher Installation warten wir ca 1-2 min, danach sollte sich ein Rotex Fallback Hotspot vom ESP32 aufbauen.
6. Jetzt verbinden wir uns mit dem ESP32 mittels WLAN und geben das Passwort H3jGqANSa7aL ein.
7. Jetzt sollte sich automatisch ein Browser Fenster öffnen wo eure WLAN SSID und das Passwort für euer WlAN abgefragt wird.Hier gebt ihr jetzt eure ugangsdaten für euer WLAN ein.
8. Nach erfolgreicher Verbindung eures ESP32 mit dem WLAN sollte dieser über http://rotex.local erreichbar sein.Wenn nicht bitte schaut in eurem Router nach der richtigen IP Adresse um.
9. Wer jetzt keine weitere Homeautomatsierungssoftware benutzt ist jetzt mit der Installation fertig und kann den ESP32 nach den Showbildern an die Rotex / Daikin anschliessen.
10. Wer eine Homeautomatiesierungsoftware zb Home Assistant benutzt kann den ESP32 verbinden.Dieser sollte von Home Assistant automatisch gefunden werden.
<br><br>
# :sparkles: Congratulation! :sparkles:


<br><br>
## Updaten des ESP32:
Die Rotex.bin Datei dient zum vereinfachten updaten über die WEB-UI Oberfläche des ESP32.Einfach OTA Update neu Datei auswählen und update klicken fertig!!! 


## Features:

- Verstellung von Raumsoll
- Verstellung WW Soll
- 1x Warm Wasser Taste ohne Heizstab (es wird für 10s auf 70Grad gestellt und dann wieder auf 45 Grad)
- Thermische Leistungsberechnung
- Betriebsmodus kann verstellt werden
- Heizkurven Verstellung in 0.01 Schritten (Standard in 0.1 Schritten)
- Neues Feature: Zwei neue Schalter angelegt um auch die Thermostat Eingänge von der Rotex / Daikin direkt zwischen Heizen / Kühlen umzuschalten.

Es werden alle relevanten Daten Abgefragt die man eigentlich benötigt.
<br>  
## weitere .YAML Dateien:

Wer Abtau Probleme hat kann sich in Iobroker oder Homeassitant leicht ein kleines Script bauen, wenn Abtauen erkannt wird soll auf Sommer geschaltet werden,
wenn heizen wieder erkannt schaltet wieder auf Betriebsmodus heizen um.So wird max 0.7 Grad Warmwasser aus dem Sepicher verbraucht.

Für Iobroker hab ich ein Blockly Script hochgeladen was ich selber nutze und super funktioniert.<br>  
Die Defrost.yaml ist für Home Assistant Automatisierung gedacht.Das ist die gleiche Automatisierung wie für IoBroker.

Getestet mit: - Rotex HSPU Compact 508 8kw

## WEBUI vom ESP:
![Bildschirmfoto 2023-12-17 um 11 33 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/524a204e-801a-4fe7-aacb-ac2e731a99b7)


## Ansicht Homeassistant:
![Bildschirmfoto 2024-04-01 um 01 37 19](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/d9c1d703-ac4a-4466-97e3-dbd6478cbed1)


## Pinbelegung GPIO 47 und 48

![ESP32-S3toCAN](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/40b1881a-b7f5-40b5-a2d7-678ee19299d2)

## Pin Belegung an der Rotex HPSU

![Rotex CAN Anschluss](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/05c36ae7-ddc9-4a1e-8a73-4559c765f132)







