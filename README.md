
                                              Rotex /Daikin HPSU CAN



Hi ich möchte mein erstes kleines Projekt vorstellen, ich wollte eine flexible Lösung die Standalone, mit Homeassistant und Iobroker läuft,um meine Rotex HPSU Compact mittels CAN BUS auszulesen und zu steuern.
Verwendet habe ich ein ESP32-S3-WROOM Board (es sollte jeder ab S1 funktionieren, weil diese die 20kbits Baudrate unterstützen,
die normalen ESP32 Wroom funktionieren nicht weil diese erst ab 50kbit funktionieren) und ein Waveshare SN65HVD230 CAN Board mit 3,3V.
Software verwende ich ESPHome. Diese wird als erstes in der Grundkonfiguration geflasht und dann gegen meine YAML getauscht.


- Der ESP Chip kann über diese Seite (https://web.esphome.io/?dashboard_install) für den ersten Gebrauch geflasht werden.
- Die .bin ist zum flashen der reinen Standalone Lösung gedacht.Da fehlt der key für Homeassistant.

- Features:

              - Verstellung von Raumsoll
  
              - Verstellung WW Soll
  
              - 1x Warm Wasser Taste ohne Heizstab (es wird für 10s auf 70Grad gestellt und dann wieder auf 45 Grad)
  
              - Thermische Leistungsberechnung
  
              - Betriebsmodus kann verstellt werden

  Es werden alle relevanten Daten Abgefragt die man eigentlich benötigt.

- WEBUI vom ESP:
![Bildschirmfoto 2023-12-17 um 11 33 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/524a204e-801a-4fe7-aacb-ac2e731a99b7)


- Ansicht Homeassistant:
![Bildschirmfoto 2023-12-17 um 11 34 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/3be9654f-3fa3-48dd-ba99-37aefdf688b9)

 - Pinbelegung GPIO 47 und 48


![ESP32-S3toCAN](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/40b1881a-b7f5-40b5-a2d7-678ee19299d2)




[Wer mir eine kleine Spende geben möchte kann es gerne hierrüber tun.Ist aber natürlich kein Zwang da Open Source!!
  ![Donate with PayPal]
  (https://raw.githubusercontent.com/stefan-niedermann/paypal-donate-button/master/paypal-donate-button.png)
]
(https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=QT54MSJR6QU7Y)


