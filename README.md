
"Ein Kaffee für nächste lange Nacht vor dem Rechner wäre toll" ist aber natürlich kein muss.
<br>[![Paypal](https://user-images.githubusercontent.com/41958506/212499642-b2fd097a-0938-4bfc-b37b-74df64592c58.png)](https://www.paypal.com/donate/?hosted_button_id=H94LZRQW9PFQ4)

# Rotex /Daikin HPSU CAN



Hi ich möchte mein erstes kleines Projekt vorstellen, ich wollte eine flexible Lösung die Standalone, mit Homeassistant und Iobroker läuft,um meine Rotex HPSU Compact mittels CAN BUS auszulesen und zu steuern.
Verwendet habe ich ein ESP32-S3-WROOM Board (es sollte jeder ab S1 funktionieren, weil diese die 20kbits Baudrate unterstützen,
die normalen ESP32 Wroom funktionieren nicht weil diese erst ab 50kbit funktionieren) und ein Waveshare SN65HVD230 CAN Board mit 3,3V.
Ein ESP32 S3 mini (Zero) funktioniert auch.Noch mal ein tick kleiner als der normale S3.

Als Software verwende ich ESPHome. Diese wird als erstes in der Grundkonfiguration geflasht und dann gegen meine YAML getauscht.
<br><br>
Die .bin Datei dient zum vereinfachten updaten über die WEB-UI Oberfläche.Einfach OTA Update neu Datei auswählen und update klicken fertig!!! 


## Features:

- Verstellung von Raumsoll
- Verstellung WW Soll
- 1x Warm Wasser Taste ohne Heizstab (es wird für 10s auf 70Grad gestellt und dann wieder auf 45 Grad)
- Thermische Leistungsberechnung
- Betriebsmodus kann verstellt werden
- Heizkurven Verstellung in 0.01 Schritten (Standart in 0.1 Schritten)
- ## Neues Feature: Zwei neue Schalter angelegt um auch die Thermostat Eingänge von der Rotex / Daikin direkt zwischen Heizen / Kühlen umzuschalten.

Es werden alle relevanten Daten Abgefragt die man eigentlich benötigt.
<br>  
Wer Abtau Probleme hat kann sich in Iobroker oder Homeassitant leicht ein kleines Script bauen, wenn Abtauen erkannt wird soll auf Sommer geschaltet werden,
wenn heizen wieder erkannt schaltet wieder auf Betriebsmodus heizen um.So wird max 0.7 Grad Warmwasser aus dem Sepicher verbraucht.

Für Iobroker hab ich ein Blockly Script hochgeladen was ich selber nutze und super funktioniert.<br>  
Die Defrost.yaml ist für Home Assistant Automatisierung gedacht.Das ist die gleiche Automatisierung wie für IoBroker.

Getestet mit: - Rotex HSPU Compact 508 8kw

## WEBUI vom ESP:
![Bildschirmfoto 2023-12-17 um 11 33 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/524a204e-801a-4fe7-aacb-ac2e731a99b7)


## Ansicht Homeassistant:
![Bildschirmfoto 2023-12-17 um 11 34 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/3be9654f-3fa3-48dd-ba99-37aefdf688b9)

## Pinbelegung GPIO 47 und 48

![ESP32-S3toCAN](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/40b1881a-b7f5-40b5-a2d7-678ee19299d2)

## Pin Belegung an der Rotex HPSU

![Rotex CAN Anschluss](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/05c36ae7-ddc9-4a1e-8a73-4559c765f132)







