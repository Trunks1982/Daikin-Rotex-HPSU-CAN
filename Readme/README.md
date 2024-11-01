#### Language Selection:
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md) 

# Rotex /Daikin HPSU CAN
**Daikin-Rotex-HPSU-CAN** ist eine flexible Standalone-Lösung zum Auslesen und Steuern von Rotex/Daikin Luftwärmepumpen über den CAN-Bus.
Die Steuerung und der Datenzugriff erfolgen wahlweise über den integrierten Webserver oder bequem über Home Assistant bzw. ioBroker.

Als Hardware wird ein **ESP32** benötigt, der eine Baudrate von **20 kbit/s** unterstützt.  
Erfolgreich getestet wurden das **ESP32-S3-WROOM-Board** und ein **WaveShare ESP32-S3 Mini**.  
Eine Übersicht weiterer Varianten und deren unterstützte Bitrate findest du in der folgenden [Tabelle](https://esphome.io/components/canbus/esp32_can).  

Zusätzlich ist ein kompatibler **CAN-Transceiver** erforderlich, wie der **Waveshare SN65HVD230** (3,3V).

## WEBUI vom ESP:
![Bildschirmfoto 2024-09-17 um 13 13 06](https://github.com/user-attachments/assets/8bb5ca8e-323f-45d1-ab4b-a30f185a6ffc)

## Ansicht Homeassistant:
![Bildschirmfoto 2024-04-01 um 01 37 19](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/d9c1d703-ac4a-4466-97e3-dbd6478cbed1)

## Achtung!!
Die Verwendung von Daikin-Rotex-HPSU-CAN kann potenziell Ihr Heizsystem beschädigen. Die Nutzung erfolgt auf eigene Verantwortung. Ich übernehme keine Haftung für entstandene Schäden.

Bitte beachten Sie, dass durch die Verwendung von Daikin-Rotex-HPSU-CAN möglicherweise Ihre Garantie sowie der Support durch den Hersteller erlischt!

# Anleitung: ESPHome in einer virtuellen Umgebung auf macOS nutzen

Diese Anleitung hilft dir, eine Python-virtuelle Umgebung zu erstellen, ESPHome darin zu installieren und ein ESP32-Gerät für die Home Assistant-Steuerung deiner Rotex/Daikin HPSU Compact einzurichten.

## 1. Überprüfen und Installieren von Python und PIP

### 1.1 Python-Installation überprüfen

Öffne das Terminal auf deinem Mac und gib den folgenden Befehl ein, um zu überprüfen, ob Python installiert ist und welche Version du hast:

```bash
python3 --version
```

Wenn Python installiert ist, zeigt der Befehl die Version an (z. B. `Python 3.9.1`). Wenn du eine Fehlermeldung erhältst, dass der Befehl nicht gefunden wurde, musst du Python installieren.

### 1.2 Python installieren (falls nicht vorhanden)

Falls Python nicht installiert ist oder du eine andere Version benötigst, kannst du es einfach über Homebrew installieren. Hier sind die Schritte:

#### 1.2.1 Homebrew installieren (falls noch nicht installiert)

Homebrew ist ein beliebter Paketmanager für macOS, mit dem du Software einfach installieren kannst. Öffne das Terminal und gib den folgenden Befehl ein:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Folge den Anweisungen im Terminal, um die Installation abzuschließen.

#### 1.2.2 Python mit Homebrew installieren

Installiere Python mit Homebrew:

```bash
brew install python
```

Überprüfe erneut die Python-Installation:

```bash
python3 --version
```

Du solltest jetzt die installierte Version von Python sehen.

### 1.3 PIP überprüfen

Überprüfe, ob PIP installiert ist:

```bash
pip3 --version
```

Wenn du eine Fehlermeldung erhältst, dass der Befehl nicht gefunden wurde, kannst du PIP mit folgendem Befehl installieren:

```bash
python3 -m ensurepip --upgrade
```

## 2. Erstellen einer virtuellen Python-Umgebung

Erstelle eine neue virtuelle Umgebung für ESPHome im Home-Verzeichnis (oder in einem beliebigen Ordner deiner Wahl):

```bash
python3 -m venv ~/esphome_venv
```

Aktiviere die virtuelle Umgebung:

```bash
source ~/esphome_venv/bin/activate
```

Nach der Aktivierung der Umgebung solltest du `(esphome_venv)` links im Terminal-Prompt sehen, was zeigt, dass die virtuelle Umgebung aktiv ist.

## 3. ESPHome in der virtuellen Umgebung installieren

Installiere ESPHome über `pip` in der aktiven virtuellen Umgebung:

```bash
pip install esphome
```

Überprüfe die Installation:

```bash
esphome version
```

Wenn alles richtig installiert ist, zeigt der Befehl die aktuelle Version von ESPHome an.

## 4. Konfigurationsdatei für den ESP32 erstellen

Erstelle eine neue Datei für die ESPHome-Konfiguration, z. B. `rotex_hpsu.yaml`, mit folgendem Inhalt. Diese Datei enthält die WLAN-Zugangsdaten, API-Schlüssel und CAN-Bus-Konfiguration:

```yaml
esphome:
  name: rotex_hpsu

esp32:
  board: esp32dev

wifi:
  ssid: "Dein_WLAN_Name"
  password: "Dein_WLAN_Passwort"
  manual_ip:
    static_ip: 192.168.1.50  # Beispiel-IP für den ESP32
    gateway: 192.168.1.1
    subnet: 255.255.255.0

api:
  encryption:
    key: "IQlCgJuBZRG216PW71elFReuWeojcwsP9zUyY1xCJTg="

ota:

# CAN-Bus Konfiguration
canbus:
  - platform: sn65hvd230
    rx_pin: GPIO3  # Passenden Pin des ESP32 eintragen
    tx_pin: GPIO1  # Passenden Pin des ESP32 eintragen
    baud_rate: 20000

sensor:
  - platform: canbus
    name: "Temperatur Sensor"
    id: temp_sensor
    can_id: 0x100  # Beispiel CAN-ID, anpassen falls nötig
    filters:
      - multiply: 0.1
```

> **Hinweis:** Ersetze `"Dein_WLAN_Name"` und `"Dein_WLAN_Passwort"` durch die Zugangsdaten deines WLANs. Passe auch die `static_ip` und die GPIO-Pins für RX und TX an dein Setup an.

## 5. secrets.yaml erstellen

Vor dem Flashen des ESP32 musst du auch eine `secrets.yaml`-Datei erstellen. Diese Datei enthält sensible Informationen wie WLAN-Passwörter und API-Schlüssel.

Erstelle eine neue Datei namens `secrets.yaml` im selben Verzeichnis wie die `rotex_hpsu.yaml`-Datei und füge folgende Inhalte hinzu:

```yaml
wifi_ssid: "Dein_WLAN_Name"
wifi_password: "Dein_WLAN_Passwort"
api_encryption_key: "IQlCgJuBZRG216PW71elFReuWeojcwsP9zUyY1xCJTg="
```

> **Hinweis:** Ersetze die Werte entsprechend mit deinen tatsächlichen WLAN-Zugangsdaten und dem API-Schlüssel.

Aktualisiere die `rotex_hpsu.yaml`-Datei, um die Werte aus der `secrets.yaml`-Datei zu verwenden:

```yaml
wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

api:
  encryption:
    key: !secret api_encryption_key
```

## 6. ESP32 flashen

Verbinde den ESP32 per USB mit deinem Mac und führe im Terminal den folgenden Befehl aus, um ESPHome auf den ESP32 zu flashen:

```bash
esphome run rotex_hpsu.yaml
```

ESPHome wird den angeschlossenen ESP32 erkennen und eine Verbindung herstellen. Wähle den USB-Port deines ESP32, wenn du dazu aufgefordert wirst. Der Flashvorgang dauert in der Regel 1–2 Minuten.

Nach dem Flashen sollte sich der ESP32 automatisch mit deinem WLAN verbinden und online gehen.

## 7. Den ESP32 in Home Assistant einbinden

1. Öffne **Home Assistant** und gehe zu **Einstellungen > Geräte & Dienste**.
2. Der ESP32 sollte als neues Gerät (`rotex_hpsu`) erscheinen und lässt sich direkt zu Home Assistant hinzufügen.
3. Überprüfe in Home Assistant, ob die Sensoren und Steuerungen vom ESP32 korrekt angezeigt werden und Daten empfangen.

## 8. Beenden der virtuellen Umgebung

Wenn du mit der Arbeit in der virtuellen Umgebung fertig bist, kannst du sie deaktivieren:

```bash
deactivate
```

Damit kehrst du zum normalen Terminal-Modus zurück.

## Zukünftige Updates und Anpassungen

Um den ESP32 erneut zu flashen oder die Konfiguration zu ändern, aktiviere zuerst die virtuelle Umgebung wieder:

```bash
source ~/esphome_venv/bin/activate
```

Bearbeite die YAML-Datei für neue Anpassungen und flashe das Gerät erneut:

```bash
esphome run rotex_hpsu.yaml
```

Diese Anleitung sollte dir helfen, ESPHome in einer virtuellen Umgebung auf macOS einzurichten, den ESP32 zu flashen und ihn mit Home Assistant zu verbinden.



## Features:

- 1x Warm Wasser Taste ohne Heizstab (es wird für 10s auf 70Grad gestellt und dann wieder auf den vorher eingestellten Wert zurück)
- Thermische Leistungsberechnung
- Heizkurven Verstellung in 0.01 Schritten (Standard in 0.1 Schritten)
- Zwei neue Schalter angelegt um auch die Thermostat Eingänge von der Rotex / Daikin direkt zwischen Heizen / Kühlen umzuschalten.
- Neues Feature Fehlercode Anzeige mit Beschreibung des Fehlers mit Handbuch Seitenangabe!!
  
<br>  


Wer Abtau-Probleme hat, kann das neue Feature "Defrost" direkt über den ESP nutzen.So wird max 0.7 Grad Warmwasser aus dem Sepicher verbraucht.


Getestet mit: 
- Rotex HSPU Compact 508 8kw mit Rocon BM1, 
- ROTEX HPSU compact Ultra
- Dakin ECH2O (bis 01/2022)


## Pinbelegung GPIO 47 und 48 (Beispiel)

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




