
# Achtung!!
Die Verwendung von Daikin-Rotex-HPSU-CAN kann potenziell Ihr Heizsystem beschädigen. Die Nutzung erfolgt auf eigene Verantwortung. Ich übernehme keine Haftung für entstandene Schäden.

# Anleitung: ESPHome in einer virtuellen Umgebung auf macOS nutzen

Diese Anleitung hilft dir, eine Python-virtuelle Umgebung zu erstellen, ESPHome darin zu installieren, das Dashboard zu nutzen und ein ESP32-Gerät für die Home Assistant-Steuerung deiner Rotex/Daikin HPSU Compact einzurichten.

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

## 4. ESPHome Dashboard starten

Um das ESPHome Dashboard zu starten, führe den folgenden Befehl im Terminal aus:

```bash
esphome dashboard ~/esphome_projects
```

> **Hinweis:** Ersetze `~/esphome_projects` durch den Pfad zu dem Verzeichnis, in dem deine ESPHome YAML-Dateien gespeichert werden sollen.

Nach dem Ausführen des obigen Befehls sollte das Terminal dir eine Adresse anzeigen, typischerweise `http://localhost:6052`. Öffne einen Webbrowser und gehe zu dieser URL:

```
http://localhost:6052
```

### 4.1 Das Dashboard verwenden

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

## 5. Den ESP32 in Home Assistant einbinden

1. Öffne **Home Assistant** und gehe zu **Einstellungen > Geräte & Dienste**.
2. Der ESP32 sollte als neues Gerät (`rotex_hpsu`) erscheinen und lässt sich direkt zu Home Assistant hinzufügen.
3. Überprüfe in Home Assistant, ob die Sensoren und Steuerungen vom ESP32 korrekt angezeigt werden und Daten empfangen.

## 6. Beenden der virtuellen Umgebung

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



