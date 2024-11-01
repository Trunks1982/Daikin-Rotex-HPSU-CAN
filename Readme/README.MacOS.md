
# Achtung!!
Die Verwendung von Daikin-Rotex-HPSU-CAN kann potenziell Ihr Heizsystem beschädigen. Die Nutzung erfolgt auf eigene Verantwortung. Ich übernehme keine Haftung für entstandene Schäden.

# Anleitung: ESPHome in einer virtuellen Umgebung auf macOS nutzen

Diese Anleitung hilft dir, eine Python-virtuelle Umgebung zu erstellen, ESPHome darin zu installieren, das Dashboard zu nutzen und ein ESP32-Gerät für die Home Assistant-Steuerung deiner Rotex/Daikin HPSU Compact einzurichten.

### 1. Dashboard Installation

1. Du lädst dir die Datei von https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/tree/dev/Install%20Scripte%20EspHome auf deinen MAC herunter.
2. Du öffnest das Terminal Programm und gehst in das Verzeichnis in dem du das Script heruntergeladen hast, danch führst du den Befehl "sudo bash ./install_esphome_mac.sh" aus.
3. Jetzt sollte der Installationsvorgang automatisch ausgeführt werden. Es kann eine weile dauern bis alles installiert ist.
4. Nach dem Ausführen des obigen Befehls sollte das Terminal dir eine Adresse anzeigen, typischerweise `http://localhost:6052`. Öffne einen Webbrowser und gehe zu dieser URL:

```
http://localhost:6052
```

### 2 Das Dashboard verwenden

1. **Neues Projekt erstellen:**
   - Klicke im Dashboard auf "New Device".
   - Gib einen Namen für dein Gerät ein, z. B. `rotex_hpsu`.
   - Wähle `ESP32-S3` als Plattform aus und klicke auf "Continue".

2. **Konfigurationsdatei für den ESP32 erstellen:**
   - Das Dashboard öffnet nun einen YAML-Editor, in dem du die Konfiguration für dein Gerät hinzufügen kannst.
   - Füge den folgenden https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/blob/main/examples/full.yaml YAML-Inhalt ein,


3. **secrets.yaml erstellen:**
   - Klicke im Dashboard auf den „Editor“ und erstelle eine Datei namens `secrets.yaml`.
   - Füge die folgenden Inhalte hinzu:

   ```yaml
   wifi_ssid: "Dein_WLAN_Name"
   wifi_password: "Dein_WLAN_Passwort"
   api_encryption_key: "IQlCgJuBZRG216PW71elFReuWeojcwsP9zUyY1xCJTg="
   ```

4. **Gerät flashen:**
   - Verbinde den ESP32 per USB mit deinem Mac.
   - Klicke im Dashboard auf die Schaltfläche „Install“ neben deinem Gerät.
   - Wähle „Plugged in via USB“ und dann den entsprechenden USB-Port für dein Gerät aus.
   - Klicke auf „Flash“, um den ESP32 zu flashen.

   Der Flashvorgang dauert in der Regel 1–2 Minuten. Nach dem Flashen sollte sich der ESP32 automatisch mit deinem WLAN verbinden und online gehen.

## 3. Den ESP32 in Home Assistant einbinden

1. Öffne **Home Assistant** und gehe zu **Einstellungen > Geräte & Dienste**.
2. Der ESP32 sollte als neues Gerät (`rotex_hpsu`) erscheinen und lässt sich direkt zu Home Assistant hinzufügen.
3. Überprüfe in Home Assistant, ob die Sensoren und Steuerungen vom ESP32 korrekt angezeigt werden und Daten empfangen.

## 4. Beenden der virtuellen Umgebung

Wenn du mit der Arbeit in der virtuellen Umgebung fertig bist, kannst du sie deaktivieren:

```bash
deactivate
```

Damit kehrst du zum normalen Terminal-Modus zurück.

## Zukünftige Updates und Anpassungen

Um den ESP32 erneut zu flashen oder die Konfiguration zu ändern, starte das ESPHome Dashboard erneut und lade deine Konfigurationsdatei. Nach Änderungen kannst du einfach den Flash-Vorgang erneut starten.

Diese Anleitung sollte dir helfen, ESPHome in einer virtuellen Umgebung auf macOS einzurichten, das Dashboard zu nutzen, den ESP32 zu flashen und ihn mit Home Assistant zu verbinden.

## Abschluss

Die Installation ist nun abgeschlossen, und du kannst den ESP32 gemäß den Schaubildern mit der Rotex/Daikin-Anlage verbinden.


**Hinweis**: Weitere Sensoren oder Steuerungen können einfach durch das Hinzufügen neuer Einträge in der ESPHome yaml-Konfiguration ergänzt werden.



