# How to setup

## Software requirements
 * [ESPHome](https://esphome.io/) installed

## Configure ESPHome with `daiin_rotext_control` external_component

  Example configuration for esp32dev board:

  ```
  esphome:
      name: rotex
      friendly_name: Rotex
      platformio_options:
          build_unflags:
              - "-std=gnu++11"
              - "-fno-exceptions"
          build_flags:
              - "-std=gnu++17"
              - "-fexceptions"

  esp32:
    board: esp32-s3-devkitc-1
    framework:
      type: arduino

  external_components:
    - source: github://wrfz/DaikinRotexControl

  logger:
    level: INFO

  api:
    encryption:
      key: !secret api_encryption_key

  ota:
    platform: esphome
    password: !secret ota_password

  web_server:

  wifi:
    ssid: !secret wifi_ssid
    password: !secret wifi_password

  canbus:
    - platform: esp32_can
      id: can_bus
      can_id: 0x680
      rx_pin: GPIO2
      tx_pin: GPIO3
      bit_rate: 20kbps

  daikin_rotex_control:
    id: rotext_hpsu
    canbus_id: can_bus
    temperature_outside:
      name: "Aussentemperatur"
    operation_mode:
      name: "Betriebsmodus"
    operation_mode_select:
      name: "Betriebsmodus setzen"
    tdhw1:
      name: "Warmwassertemperatur"
    water_pressure:
      name: "Wasserdruck"
    water_flow:
      name: "Durchfluss"
    error_code:
      name: "Fehlercode"
  ```