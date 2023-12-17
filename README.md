Hi ich möchte mein erstes kleines Projekt vorstellen, ich wollte eine flexible Lösung die Standalone, mit Homeassistant und Iobroker läuft,um meine Rotex HPSU Compact mittels CAN BUS auszulesen und zu steuern.
Verwendet habe ich ein ESP32-S3-WROOM CAM Board (es sollte jeder ab S1 funktionieren, weil diese die 20kbits Baudrate unterstützen,
die normalen ESP32 Wroom funktionieren nicht weil diese erst ab 50kbit funktionieren) und ein Waveshare SN65HVD230 CAN Board mit 3,3V.
Software verwende ich ESPHome.Diese wird als erstes in der Grundkonfiguration geflasht und dann gegen meine YAML getauscht.


- Der ESP Chip kann über diese Seite (https://web.esphome.io/?dashboard_install) für den ersten Gebrauch geflasht werden.
- Die .bin ist zum flashen der reinen Standalone Lösung gedacht.Da fehlt der key für Homeassistant.

WEBUI vom ESP:
![Bildschirmfoto 2023-12-17 um 11 33 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/524a204e-801a-4fe7-aacb-ac2e731a99b7)


Ansicht Homeassistant:
![Bildschirmfoto 2023-12-17 um 11 34 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/3be9654f-3fa3-48dd-ba99-37aefdf688b9)

Pinbelegung GPIO 47 und 48


![IMG_7065](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/3c096a3c-5b6c-49c7-8372-59d175c0f3c8)
