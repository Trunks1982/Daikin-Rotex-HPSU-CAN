#### Language Selection:
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md) 

# Warning!!
Die Verwendung von Daikin-Rotex-HPSU-Can kann das Heizsystem beschädigen. Die Verwendung von Daikin-Rotex-HPSU-Can erfolgt auf eigene Gefahr. Der Schöpfer kann nicht für Schäden verantwortlich gemacht werden.

Sie können einen Verlust der Garantie und Unterstützung durch den Hersteller riskieren!!!!

Diese Software wird vom Hersteller nicht unterstützt!!!!




# Rotex/Daikin HPSU CAN Integration

Diese Lösung ermöglicht das Auslesen und Steuern der Rotex HPSU Compact über CAN-Bus, sowohl als Standalone-System als auch in Kombination mit **Home Assistant** und **ioBroker**.

## Voraussetzungen

- Ein ESP32-S3-WROOM Board ab Version S1 (Modelle ab S1 unterstützen die benötigte Baudrate von 20 kbit/s; die Standard-ESP32-WROOM-Modelle unterstützen nur 50 kbit/s) oder alternativ ein ESP32-S3 Mini (Zero).
- Ein **Waveshare SN65HVD230 CAN-Board** mit 3,3V.

## Installationsanleitung (Standalone)

### Schritt 1: Vorbereitungen

1. **Browser:** Verwende **Google Chrome** (Safari und Firefox werden nicht unterstützt).
2. **Firmware:** Lade die Datei `Rotex-Factory.bin` aus dem Ordner `.bin` herunter.

### Schritt 2: ESP32 flashen

1. Öffne die Webseite [https://web.esphome.io](https://web.esphome.io).
2. Verbinde den ESP32 per USB-C-Kabel mit deinem PC.
3. Klicke auf der Webseite auf „**Connect**“ und wähle im erscheinenden Fenster die USB-Serial-Schnittstelle aus.
4. Klicke anschließend auf „**Installieren**“.
5. Wähle die heruntergeladene Datei `Rotex-Factory.bin` aus und klicke erneut auf „**Installieren**“. Der Vorgang dauert etwa 2 Minuten.

### Schritt 3: WLAN-Verbindung des ESP32 konfigurieren

1. Starte den ESP32 neu und warte ca. 1–2 Minuten, bis der „Rotex Fallback Hotspot“ vom ESP32 erstellt wird.
2. Verbinde dich mit dem Hotspot (Passwort: `H3jGqANSa7aL`).
3. Öffne die Webseite [http://192.168.4.1](http://192.168.4.1) oder [http://rotex.local](http://rotex.local) im Browser.
4. Gib deine WLAN-SSID und dein WLAN-Passwort ein, um den ESP32 mit deinem Netzwerk zu verbinden.

### Schritt 4: Prüfung der Netzwerkverbindung

- Der ESP32 sollte nun über [http://rotex.local](http://rotex.local) erreichbar sein.  
- Falls dies nicht funktioniert, überprüfe bitte die IP-Adresse des ESP32 in deinem Router.

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


## Pinbelegung GPIO 47 und 48 (yaml)

![ESP32-S3toCAN](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/40b1881a-b7f5-40b5-a2d7-678ee19299d2)

## Pinbelegung GPIO 5 und 6 (Bin Datei)

![TxPin5_RxPin6](https://github.com/user-attachments/assets/b0e3ae0d-2354-4871-b295-c156836afddf)

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




