#### Sprache wählen:
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)

## Warnung!

Die Nutzung von esphome-daikin-rotex-can kann bei unsachgemäßer Anwendung zu Schäden an der Heizungsanlage führen.
Die Verwendung erfolgt auf eigenes Risiko, und der Entwickler übernimmt keine Haftung für etwaige Schäden.
Der Einsatz dieser Software kann zum Verlust von Garantieansprüchen und Hersteller-Support führen.
Es wird darauf hingewiesen, dass diese Software nicht vom Hersteller unterstützt wird.

## Software requirements
 * [ESPHome](https://esphome.io/) installiert

# Installationsanleitung

1. `ESPHome Dashboard` öffnen
2. Rechts unten auf `NEW DEVICE` klicken
3. Auf die Schaltfläche `CONTINUE` klicken
4. Einen beliebigen Name für die Konfiguration eingeben (z.B. Rotex) und mit `NEXT` bestätigen
5. `ESP32-S3` wählen
6. Mit `SKIP` die Erstellung der Grundkonfiguration abschließen
7. Bei der neu erstellten Konfiguration auf `EDIT` klicken
8. Aus [examples/full.yaml](examples/full.yaml) folgende drei Sektionen übernehmen:
    - `esphome` → `platformio_options` (Sehr wichtig, damit die spätere Kompilation funktioniert)
    - `canbus` (GPIOs den eigenen Bedürfnissen anpassen)
    - `daikin_rotex_can` (Nicht benötigte Entities können entfernt werden)
9. Im ESPHome Dashboard bei der Konfiguration rechts unten auf die drei senkrechten Punkte klicken und anschliessend auf `Install`


Unterstützte Sensoren und Schalter:
------------------------

|ID                              |Empfohlener Name                  |Beschreibubg                                     |
|---                             |---                               |---                                              |
|**bypass_valve**                |BPV                               |Prozentuale Öffnungsstellung des Bypass-Ventils  |
|**circulation_pump**            |Umwälzpumpe                       |Prozentuale Betriebsleistung der Umwälzpumpe     |
|**circulation_pump_min**        |Umwälzpumpe Min                   |Minimale Betriebsstufe der Umwälzpumpe           |
|**circulation_pump_max**        |Umwälzpumpe Max                   |Maximale Betriebsstufe der Umwälzpumpe           |
|**delta_temp_ch**               |Spreizung MOD HZ                  |Temperaturdifferenz im Heizbetrieb               |
|**delta_temp_ww**               |Spreizung MOD WW                  |Temperaturdifferenz bei der Warmwasserbereitung  |
|**dhw_mixer_position**          |DHW Mischer Position              |Position des Warmwassermischers                  |
|**dhw_run**                     |Warmwasser bereiten               |Steuerung zur Aktivierung der Warmwasserbereitung|
|**ehs_for_ch**                  |EHS fuer CH                       |Externes Heizsystem für die Heizung              |
|**error_code**                  |Fehlercode                        |Fehlercode und Beschreibung                      |
|**flow_rate**                   |Durchfluss                        |Durchflussmenge in Litern pro Stunde             |
|**flow_temperature_day**        |T Vorlauf Tag                     |Vorlauftemperatur für den Tagbetrieb             |
|**heating_curve**               |Heizkurve                         |Heizkurve zur Temperaturregelung                 |
|**hk_function**                 |HK Funktion                       |Heizkreisfunktion                                |
|**max_target_flow_temp**        |Max VL Soll                       |Maximale Vorlauftemperatur                       |
|**min_target_flow_temp**        |Min VL Soll                       |Minimale Vorlauftemperatur                       |
|**mode_of_operating**           |Betriebsart                       |Betriebsart der Wärmepumpe                       |
|**operating_mode**              |Betriebsmodus                     |Aktueller Betriebsmodus der Wärmepumpe           |
|**qboh**                        |EHS für DHW                       |Externes Heizsystem für die Warmwasserbereitung  |
|**qch**                         |Energie Heizung                   |Erzeugte Heizenergie                             |
|**qdhw**                        |Energie für WW                    |Erzeugte Energie für Warmwasser                  |
|**quiet**                       |Flüsterbetrieb                    |Aktivierung des Flüsterbetriebs                  |
|**runtime_compressor**          |Laufzeit Compressor               |Laufzeit des Kompressors                         |
|**runtime_pump**                |Laufzeit Pump                     |Laufzeit der Pumpe                               |
|**sg_mode**                     |SG Modus                          |SG-Modus                                         |
|**smart_grid**                  |Smart Grid                        |Smart-Grid-Funktion                              |
|**status_kesselpumpe**          |Status Kesselpumpe                |Status der Kesselpumpe                           |
|**status_kompressor**           |Status Kompressor                 |Status des Kompressors                           |
|**target_room1_temperature**    |Raumsoll 1                        |Raum-Soll Temperatur                             |
|**tdhw1**                       |Warmwassertemperatur              |Warmwassertemperatur                             |
|**temperature_outside**         |Aussentemperatur                  |Außentemperatur, gemessen von der Außeneinheit   |
|**t_ext**                       |T-Aussen                          |Außentemperatur.                                 |
|**tr**                          |Ruecklauftemperatur Heizung       |Rücklauftemperatur der Heizung                   |
|**tv**                          |Heizkreis Vorlauf (TV)            |Vorlauftemperatur des Heizkreises                |
|**tvbh**                        |Vorlauftemperatur Heizung (TVBH)  |Vorlauftemperatur des Heizsystems                |
|**t_hs**                        |T-WE                              |Warmwassererzeugertemperatur (T-WE)              |
|**water_pressure**              |Wasserdruck                       |Wasserdruck des Systems                          |
|**target_hot_water_temperature**|T-WW-Soll1                        |Sollwert für die Warmwassertemperatur            |
|**target_supply_temperature**   |Vorlauf Soll                      |Sollwert der Vorlauftemperatur                   |
|**thermal_power**               |Thermische Leistung               |Aktuelle thermische Leistung                     |
|**total_energy_produced**       |Erzeugte Energie Gesamt           |Gesamt erzeugte Energie                          |
