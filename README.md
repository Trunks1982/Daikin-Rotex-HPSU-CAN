#### Language Selection:
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md) 

# Warning!!
Die Verwendung von Daikin-Rotex-HPSU-Can kann das Heizsystem beschädigen. Die Verwendung von Daikin-Rotex-HPSU-Can erfolgt auf eigene Gefahr. Der Schöpfer kann nicht für Schäden verantwortlich gemacht werden.

Sie können einen Verlust der Garantie und Unterstützung durch den Hersteller riskieren!!!!

Diese Software wird vom Hersteller nicht unterstützt!!!!




# Rotex /Daikin HPSU CAN



Wir bieten hier eine flexible Lösung die Standalone, mit Homeassistant und Iobroker läuft,um meine Rotex HPSU Compact mittels CAN BUS auszulesen und zu steuern.
Verwendet wird zb. ein ESP32-S3-WROOM Board (es sollte jeder ab S1 funktionieren, weil diese die 20kbits Baudrate unterstützen,
die normalen ESP32 Wroom funktionieren nicht weil diese erst ab 50kbit funktionieren) und ein Waveshare SN65HVD230 CAN Board mit 3,3V.
Ein ESP32 S3 mini (Zero) funktioniert auch.

## Installationsanleitung:
1. Du benötigst um den ESP32 zu flashen einmal den Browser Google Chrome (Safari und Firefox werden nicht unterstützt). Dann benötigst du die Datei Rotex-Factory.bin aus dem Ordner .bin.
2. Du öffnest dann die Website https://web.esphome.io und verbindest den ESP32 mittels USB-C mit dem PC. Nach dem verbinden klickst du auf der Website auf "Connect". Danach öffnet sich ein Fenster wo du die USB-Serial Schnitstelle auswählst.
3. Danach öffnet sich ein neues Fenster auf der Website wo du auf "Installieren" klickst.
4. Jetzt klickst du auf "Datei auswählen" und wählst die vorher runtergeladene Rotex-Factory.bin aus und klickst auf "Installieren". Dieser Vorgang benötigt ca 2min.
5. Nach erfolgreicher Installation startest du den ESP neu und wartest ca 1-2 min, danach sollte sich ein "Rotex Fallback Hotspot" vom ESP32 aufbauen.
6. Jetzt verbindest du dich mit dem ESP32 mittels WLAN und gibst das "Passwort H3jGqANSa7aL" ein.
7. Im Browser offnest du die Seite http://192.168.4.1 oder http://rotex.local, wo du deine WLAN SSID und das Passwort für dein WLAN abgefragt wird. Hier trägst du jetzt deine Zugangsdaten für dein WLAN ein.
8. Nach erfolgreicher Verbindung deines ESP32 mit dem WLAN sollte dieser über http://rotex.local erreichbar sein. Wenn nicht schau bitte auf deinem Router nach der richtigen IP Adresse.
9. Wen du jetzt keine weitere Homeautomatisierungssoftware benutzt, bist du jetzt mit der Installation fertig und kannst den ESP32 nach den Schaubildern an die Rotex / Daikin anschliessen.
10. Für eine Benutzung mit Home Assistant sollte das Addon ESPHome installiert werden, um in der secret.yaml Datei noch:<br>
    api_encryption_key: "IQlCgJuBZRG216PW71elFReuWeojcwsP9zUyY1xCJTg="<br>
    einzutragen. Die Datei ist unter ESPhome zufinden. Hier für würde ich zb. den File Editor verwenden.
13. Wenn du eine Homeautomatisierungssoftware zb. Home Assistant benutzt, kannst du jetzt den ESP32 verbinden. Dieser sollte von Home Assistant automatisch gefunden werden.
<br><br>
# :sparkles: Congratulation! :sparkles:


<br><br>
## Updaten des ESP32:
Die Rotex.bin Datei dient zum vereinfachten updaten über die WEB-UI Oberfläche des ESP32.Einfach OTA Update neu Datei auswählen und update klicken fertig!!! 


## Features:

- 1x Warm Wasser Taste ohne Heizstab (es wird für 10s auf 70Grad gestellt und dann wieder auf den vorher eingestellten Wert zurück)
- Thermische Leistungsberechnung
- Heizkurven Verstellung in 0.01 Schritten (Standard in 0.1 Schritten)
- Zwei neue Schalter angelegt um auch die Thermostat Eingänge von der Rotex / Daikin direkt zwischen Heizen / Kühlen umzuschalten.
- Neues Feature Fehlercode Anzeige mit Beschreibung des Fehlers mit Handbuch Seitenangabe!!
  
<br>  


Wer Abtau Probleme hat kann (demnächst) das neue Feature "Defrost" direkt über den ESP nutzen.So wird max 0.7 Grad Warmwasser aus dem Sepicher verbraucht.


Getestet mit: - Rotex HSPU Compact 508 8kw mit Rocon BM1, ROTEX HPSU compact Ultra

## WEBUI vom ESP:



![Bildschirmfoto 2024-09-17 um 13 13 06](https://github.com/user-attachments/assets/8bb5ca8e-323f-45d1-ab4b-a30f185a6ffc)

## Ansicht Homeassistant:
![Bildschirmfoto 2024-04-01 um 01 37 19](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/d9c1d703-ac4a-4466-97e3-dbd6478cbed1)


## Pinbelegung GPIO 47 und 48

![ESP32-S3toCAN](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/40b1881a-b7f5-40b5-a2d7-678ee19299d2)

## Pin Belegung an der Rotex HPSU

![Rotex CAN Anschluss](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/05c36ae7-ddc9-4a1e-8a73-4559c765f132)


## DIY Platine von (Dornieden)

mehr dazu im Discusions Forum https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/discussions/35:<br> 
https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/blob/main/DIY-Platine/README

<img width="554" alt="357024972-fda9cc55-63de-4862-963a-ea39f58e5028" src="https://github.com/user-attachments/assets/1640db7a-693b-45a5-8315-dcbc2e84892c">



# Telegram Gruppenchat
https://t.me/+C1iVhAjaENw0ODY0

<br><br>
"Ein Kaffee für nächste lange Nacht vor dem Rechner wäre toll" ist aber natürlich kein muss.
<br>[![Paypal](https://user-images.githubusercontent.com/41958506/212499642-b2fd097a-0938-4bfc-b37b-74df64592c58.png)](https://www.paypal.com/donate/?hosted_button_id=H94LZRQW9PFQ4)




