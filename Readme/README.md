
# Achtung!!
Die Verwendung von Daikin-Rotex-HPSU-CAN kann potenziell Ihr Heizsystem beschädigen. Die Nutzung erfolgt auf eigene Verantwortung. Ich übernehme keine Haftung für entstandene Schäden.

## Anleitung: Installation ESPHome

Diese Anleitung hilft dir, ESPHome auf Linux/MacOS oder Windows zu installieren, das Dashboard zu nutzen und ein ESP32-Gerät für die Home Assistant-Steuerung deiner Rotex/Daikin HPSU Compact einzurichten.

### 1. Verwendung ESPHome Manager

1. Du lädst dir je nach Betriebssystem diese Datei auf deinen Computer herunter.
   - Windows: https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/tree/dev/scripts/ManageMinicondaESPHome_Win.ps1
   - Linux/MAC: https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/tree/dev/scripts/ManageESPHome_Linux_MacOS.sh

| **Windows**                                           | **Linux/MacOS**                                   |
|------------------------------------------------------|--------------------------------------------------|
| In einer Admin PowerShell diesen Befehl ausführen.<br>`PowerShell -ExecutionPolicy Bypass -File "X:\Path\To\InstallMinicondaESPHome.ps1"` | In einem Terminal/Console diese Befehle ausführen.<br>`chmod +x /Path/To/ManageESPHome_Linux_MacOS.sh`<br><br>`./Path/To/ManageESPHome_Linux_MacOS.sh` |

### Folgende Funktionen bieter der ESPHome Manager:

- **Installation von ESPHome inklusive Miniconda (Windows) oder Python (Linux/Mac) falls notwendig**  
- **Upgrade von ESPHome**
- **ESPHome Dashboard starten**
- **Deinstallation von ESPHome (sowie Miniconda)**
  
###### Nach dem starten des ESPHome Dashboards per ESPHome Manager ist das Dashboard unter folgender URL erreichbar:

```
http://localhost:6052
```

### 2 Das Dashboard verwenden

1. **Neues Projekt erstellen:**
   - Klicke im Dashboard auf "NEW DEVICE".
   - Gib einen Namen für dein Gerät ein, z. B. `rotex_hpsu`.
   - Beim Schritt zur Installation auf "SKIP THIS STEP" klicken.
   - Wähle `ESP32-S3` als Plattform aus und klicke auf "CONTINUE".
   - Encryption Key wegsichern und auf "SKIP" klicken.

2. **Konfigurationsdatei für den ESP32 erstellen:**
   - Im Dashboard wurde dein neues Gerät angelegt und man kommt durch klicken auf "EDIT" in den YAML-Editor.
   - Füge den folgenden https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/blob/main/examples/full.yaml YAML-Inhalt ein.

3. **secrets.yaml erstellen:**
   - Klicke im Dashboard oben rechts auf "SECRETS", dies erstellt eine Datei namens `secrets.yaml`.
   - Füge die folgenden Inhalte hinzu:

   ```yaml
   wifi_ssid: "Dein_WLAN_Name"
   wifi_password: "Dein_WLAN_Passwort"
   api_encryption_key: "DER VORHER GESICHERTE ENCRYPTION KEY"
   ```

4. **Gerät flashen:**
   - Verbinde den ESP32 per USB mit deinem Mac.
   - Klicke im Dashboard auf die drei Punkte bei deinem neu angelegten Gerät und wähle „Install“ aus.
   - Wähle „Plug into this computer“ und dann den entsprechenden USB-Port für dein Gerät aus.
   - Klicke auf „Flash“, um den ESP32 zu flashen.

   Der Flashvorgang dauert in der Regel 1–2 Minuten. Nach dem Flashen sollte sich der ESP32 automatisch mit deinem WLAN verbinden und online gehen.

5. **Falls das Dashboard nicht mehr benötigt wird:**

Für Windows das Pop-Up Terminal schließen, bei Linux/Mac den laufenden Prozess mit CTRL+C stoppen.

## 3. Den ESP32 in Home Assistant einbinden

1. Öffne **Home Assistant** und gehe zu **Einstellungen > Geräte & Dienste**.
2. Der ESP32 sollte als neues Gerät (`rotex_hpsu`) erscheinen und lässt sich direkt zu Home Assistant hinzufügen.
3. Überprüfe in Home Assistant, ob die Sensoren und Steuerungen vom ESP32 korrekt angezeigt werden und Daten empfangen.

## Zukünftige Updates und Anpassungen

Um den ESP32 erneut zu flashen oder die Konfiguration zu ändern, starte das ESPHome Dashboard per ESPHome Manager erneut. Nach Änderungen kannst du einfach den Flash-Vorgang erneut starten.

Diese Anleitung sollte dir helfen, ESPHome auf Windows oder Linux/Mac einzurichten, das Dashboard zu nutzen, den ESP32 zu flashen und ihn mit Home Assistant zu verbinden.

## Abschluss

Die Installation ist nun abgeschlossen, und du kannst den ESP32 gemäß den Schaubildern mit der Rotex/Daikin-Anlage verbinden.


**Hinweis**: Weitere Sensoren oder Steuerungen können einfach durch das Hinzufügen neuer Einträge in der ESPHome yaml-Konfiguration ergänzt werden.



