Hi ich möchte mein erstes kleines Projekt vorstellen, ich wollte eine flexible Lösung die Standalone, mit Homeassistant und Iobroker läuft,um meine Rotex HPSU Compact mittels CAN BUS auszulesen und zu steuern.
Verwendet habe ich ein FREENOVE ESP32-S3-WROOM CAM Board (es sollte jeder ab S1 funktionieren, weil diese die 20kbits Baudrate unterstützen,
die normalen ESP32 Wroom funktionieren nicht weil diese erst ab 50kbit funktionieren) und ein Waveshare SN65HVD230 CAN Board mit 3,3V.
Software verwende ich ESPHome.Diese wird als erstes in der Grundkonfiguration geflasht und dann gegen meine YAML getauscht.

