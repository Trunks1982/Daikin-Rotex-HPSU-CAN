#!/bin/bash

# Überprüfen, ob Homebrew installiert ist
if ! command -v brew &> /dev/null
then
    echo "Homebrew ist nicht installiert. Installiere Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Python und pip installieren
echo "Installiere Python und pip..."
brew install python

# Virtuelle Umgebung für ESPHome erstellen
echo "Erstelle virtuelle Umgebung für ESPHome..."
python3 -m venv ~/esphome_venv

# Virtuelle Umgebung aktivieren
source ~/esphome_venv/bin/activate

# ESPHome installieren
echo "Installiere ESPHome..."
pip install esphome

# Verzeichnis für ESPHome-Projekte erstellen
echo "Erstelle das Verzeichnis ~/esphome_projects..."
mkdir -p ~/esphome_projects

# ESPHome Dashboard starten
echo "Starte das ESPHome Dashboard..."
esphome dashboard ~/esphome_projects

echo "Installation abgeschlossen! Öffne deinen Browser und gehe zu http://localhost:6052"

