#### Language Selection:
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)

## Warning!

The use of esphome-daikin-rotex-can may cause damage to the heating system if used improperly.
Usage is at your own risk, and the developer assumes no responsibility for any damages.
Using this software may void your warranty and manufacturer support.
Please note that this software is not supported by the manufacturer.
<br>
<br>

## Software requirements
 * [ESPHome](https://esphome.io/) installed

# How to setup

1. Open the `ESPHome Dashboard`
2. Click on `NEW DEVICE` in the bottom right corner
3. Click the `CONTINUE` button
4. Enter any name for the configuration (e.g., Rotex) and confirm with `NEXT`
5. Select `ESP32-S3`
6. Complete the basic configuration by clicking `SKIP`
7. Click `EDIT` on the newly created configuration
8. Copy the following three sections from [examples/full.yaml](examples/full.yaml):
    - `esphome` → `platformio_options` (Very important to ensure successful compilation later)
    - `canbus` (Adjust GPIOs according to your needs)
    - `daikin_rotex_can` (Unnecessary entities can be removed)
9. In the ESPHome Dashboard, click on the three vertical dots in the bottom right corner of the configuration and then click `Install`.


Supported sensors and controls:
------------------------

|ID                              |Empfohlener Name                  |Beschreibubg                                               |
|---                             |---                               |---                                                        |
|**bypass_valve**                |BPV                               |Percentage opening position of the bypass valve            |
|**circulation_pump**            |Umwälzpumpe                       |Percentage operating performance of the circulation pump   |
|**circulation_pump_min**        |Umwälzpumpe Min                   |Minimum operating level of the circulation pump            |
|**circulation_pump_max**        |Umwälzpumpe Max                   |Maximum operating level of the circulation pump            |
|**delta_temp_ch**               |Spreizung MOD HZ                  |Temperature difference during heating operation            |
|**delta_temp_dhw**              |Spreizung MOD WW                  |Temperature difference during hot water preparation        |
|**dhw_mixer_position**          |DHW Mischer Position              |Position of the hot water mixer                            |
|**dhw_run**                     |Warmwasser bereiten               |Control for activating hot water preparation               |
|**ehs_for_ch**                  |EHS fuer CH                       |External heating system for heating                        |
|**error_code**                  |Fehlercode                        |Error code and description                                 |
|**flow_rate**                   |Durchfluss                        |Flow rate in liters per hour                               |
|**flow_temperature_day**        |T Vorlauf Tag                     |Flow temperature for daytime operation                     |
|**heating_curve**               |Heizkurve                         |Heating curve for temperature control                      |
|**hk_function**                 |HK Funktion                       |Heating circuit function                                   |
|**max_target_flow_temp**        |Max VL Soll                       |Maximum flow temperature                                   |
|**min_target_flow_temp**        |Min VL Soll                       |Minimum flow temperature                                   |
|**mode_of_operating**           |Betriebsart                       |Operating mode of the heat pump                            |
|**operating_mode**              |Betriebsmodus                     |Current operating mode of the heat pump                    |
|**qboh**                        |EHS für DHW                       |External heating system for hot water preparation          |
|**qch**                         |Energie Heizung                   |Produced heating energy                                    |
|**qdhw**                        |Energie für WW                    |Produced energy for hot water                              |
|**quiet**                       |Flüsterbetrieb                    |Activation of quiet operation                              |
|**runtime_compressor**          |Laufzeit Compressor               |Runtime of the compressor                                  |
|**runtime_pump**                |Laufzeit Pump                     |Runtime of the pump                                        |
|**sg_mode**                     |SG Modus                          |SG mode                                                    |
|**smart_grid**                  |Smart Grid                        |Smart grid function                                        |
|**status_kesselpumpe**          |Status Kesselpumpe                |Status of the boiler pump                                  |
|**status_kompressor**           |Status Kompressor                 |Status of the compressor                                   |
|**target_room1_temperature**    |Raumsoll 1                        |Target room temperature                                    |
|**tdhw1**                       |Warmwassertemperatur              |Hot water temperature                                      |
|**temperature_outside**         |Aussentemperatur                  |Outside temperature measured by the outdoor unit           |
|**t_ext**                       |T-Aussen                          |Outside temperature                                        |
|**tr**                          |Ruecklauftemperatur Heizung       |Return temperature of the heating system                   |
|**tv**                          |Heizkreis Vorlauf (TV)            |Flow temperature of the heating circuit                    |
|**tvbh**                        |Vorlauftemperatur Heizung (TVBH)  |Flow temperature of the heating system                     |
|**t_hs**                        |T-WE                              |Hot water generator temperature (T-WE)                     |
|**water_pressure**              |Wasserdruck                       |Water pressure of the system                               |
|**target_hot_water_temperature**|T-WW-Soll1                        |Setpoint for hot water temperature                         |
|**target_supply_temperature**   |Vorlauf Soll                      |Setpoint for supply temperature                            |
|**thermal_power**               |Thermische Leistung               |Current thermal power                                      |
|**total_energy_produced**       |Erzeugte Energie Gesamt           |Total energy produced                                      |