# How to setup

## Software requirements
 * [ESPHome](https://esphome.io/) installed

Configuration variables:
------------------------

- **temperature_outside** (*Optional*): Outdoor temperature, measured by the heat pump's outdoor unit (***Aussentemperatur***).
- **operation_mode** (*Optional*): Current operating mode of the heat pump (***Betriebsmodus***).
- **operation_mode_select** (*Optional*): Switch to change the operating mode (***Betriebsmodus setzen***).
- **tdhw1** (*Optional*): Current hot water temperature (***Warmwassertemperatur***).
- **water_pressure** (*Optional*): Current water pressure of the system (***Wasserdruck***).
- **water_flow** (*Optional*): Current water flow rate (L/h) of the system (***Durchfluss***).
- **error_code** (*Optional*): Error code and a brief description of the error. (***Fehlercode***).
- **tvbh** (*Optional*): Flow temperature on the backup heater. (***Vorlauftemperatur Heizung (TVBH)***).
- **tv** (*Optional*): Heating circuit flow. (***Heizkreis Vorlauf (TV)***).
- **tr** (*Optional*): Heating return temperature (***Ruecklauftemperatur Heizung***).
- **status_kompressor** (*Optional*): Status of the compressor (***Status Kompressor***).
- **status_kesselpumpe** (*Optional*): Status of the boiler pump (***Status Kesselpumpe***).
