#### Language Selection:
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)

# Warning!

The use of esphome-daikin-rotex-can may cause damage to the heating system if used improperly.
Usage is at your own risk, and the developer assumes no responsibility for any damages.
Using this software may void your warranty and manufacturer support.
Please note that this software is not supported by the manufacturer.
<br>
<br>

## Software requirements
 * [ESPHome](https://esphome.io/) installed

# How to setup

1. Open the `ESPHome Dashboard`.
2. Click on `NEW DEVICE` in the bottom right corner.
3. Click the `CONTINUE` button.
4. Enter any name for the configuration (e.g., Rotex) and confirm with `NEXT`.
5. Select `ESP32-S3`.
6. Complete the basic configuration by clicking `SKIP`.
7. Click `EDIT` on the newly created configuration.
8. Copy the following three sections from [examples/full.yaml](examples/full.yaml):
    - `esphome` → `platformio_options` (Very important to ensure successful compilation later)
    - canbus (Adjust GPIOs according to your needs)
    - daikin_rotex_can (Unnecessary entities can be removed)
9. In the ESPHome Dashboard, click on the three vertical dots in the bottom right corner of the configuration and then click `Install`.


Supported sensors and controls:
------------------------

|ID                              |Empfohlener Name                  |Beschreibubg                                     |
|---                             |---                               |---                                              |
|**bypass_valve**                |BPV                               |Prozentuale Öffnungsstellung des Bypass-Ventils  |
|**circulation_pump**            |Umwälzpumpe                       |Prozentuale Betriebsleistung der Umwälzpumpe     |
|**circulation_pump_min**        |Umwälzpumpe Min                   |Minimale Betriebsstufe der Umwälzpumpe           |
|**circulation_pump_max**        |Umwälzpumpe Max                   |Maximale Betriebsstufe der Umwälzpumpe           |
|**dhw_mixer_position**          |DHW Mischer Position              |Position des Warmwassermischers                  |
|**dhw_run**                     |Warmwasser bereiten               |Steuerung zur Aktivierung der Warmwasserbereitung|
|**ehs_for_ch**                  |EHS fuer CH                       |Externes Heizsystem für die Heizung              |
|**error_code**                  |Fehlercode                        |Fehlercode und Beschreibung                      |
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
|**spreizung_mod_hz**            |Spreizung MOD HZ                  |Temperaturdifferenz im Heizbetrieb               |
|**spreizung_mod_ww**            |Spreizung MOD WW                  |Temperaturdifferenz bei der Warmwasserbereitung  |
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
|**water_flow**                  |Durchfluss                        |Durchflussmenge in Litern pro Stunde             |
|**water_pressure**              |Wasserdruck                       |Wasserdruck des Systems                          |
|**target_hot_water_temperature**|T-WW-Soll1                        |Sollwert für die Warmwassertemperatur            |
|**target_supply_temperature**   |Vorlauf Soll                      |Sollwert der Vorlauftemperatur                   |
|**thermal_power**               |Thermische Leistung               |Aktuelle thermische Leistung                     |
|**total_energy_produced**       |Erzeugte Energie Gesamt           |Gesamt erzeugte Energie                          |