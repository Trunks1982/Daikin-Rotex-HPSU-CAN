#### Language Selection:
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md) 
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.md)

<br><br>
"A coffee for the next long night in front of the computer would be great" is of course not a must.
<br>[![Paypal](https://user-images.githubusercontent.com/41958506/212499642-b2fd097a-0938-4bfc-b37b-74df64592c58.png)](https://www.paypal.com/donate/?hosted_button_id=H94LZRQW9PFQ4)

# Warning
The use of Daikin-Rotex-HPSU-Can can damage the heating system. The use of Daikin-Rotex-HPSU-Can is at your own risk. The creator cannot be held responsible for damage.

You can risk a loss of warranty and support from the manufacturer!!!!

This software is not supported by the manufacturer!!!!

# Rotex /Daikin HPSU CAN



Hi, I would like to present my first small project, I wanted a flexible solution that runs standalone, with Homeassistant and Iobroker, to read and control my Rotex HPSU Compact using CAN BUS.
I used an ESP32-S3-WROOM board (it should work for everyone from S1, because they support the 20kbits baudrate,
The normal ESP32 Wroom do not work because they only work from 50kbit) and a Waveshare SN65HVD230 CAN board with 3.3V.
An ESP32 S3 mini (Zero) also works.

## Installation instructions:

1. To flash the ESP32, you need the browser Google Chrome once (Safari and Firefox are not supported). Then you need the file rotex-factory.bin.

2. You then open the website https://web.esphome.io and connect the ESP32 to the PC using USB-C. After connecting, click on "Connect" on the website. After that, a window will open where you select the USB serial interface.

3. After that, a new window will open on the website where you click on "Install".

4. Now click on "Select file" and select the previously downloaded rotex-factory.bin and click on "Install". This process takes about 2 minutes.

5. After successful installation, you wait about 1-2 minutes, after which a "Rotex Fallback Hotspot" from the ESP32 should build up.

6. Now you connect to the ESP32 via WLAN and enter the "Password H3jGqANSa7aL".

7. Now a browser window should automatically open where you will be asked for your WLAN SSID and the password for your WLAN. Here you now enter your access data for your WLAN.

8. After successfully connecting your ESP32 to the WLAN, it should be accessible via http://rotex.local. If not, please check your router for the correct IP address.

9. If you do not use any other home automation software now, you are now finished with the installation and can connect the ESP32 to the Rotex / Daikin according to the diagrams.

10. If you use a home automation software e.g. Home Assistant, you can now connect the ESP32. This should be found automatically by Home Assistant.
<br><br>
# :sparkles: Congratulation! :sparkles:


<br><br>
## Updating the ESP32:

The Rotex.bin file is used for simplified updating via the WEB-UI interface of the ESP32. Simply select OTA Update new file and update click done!!!

## Features:

- Adjustment of room soll

- Adjustment WW Should

- 1x hot water button without heating rod (it is set to 70 degrees for 10s and then back to 45 degrees)

- Thermal performance calculation

- Operating mode can be adjusted

- Heating curves adjustment in 0.01 steps (default in 0.1 steps)

- New feature: Two new switches created to also switch the thermostat inputs from the Rotex / Daikin directly between heating / cooling.

- ### New feature error code display with description of the error with Hanbuch page information!!

All relevant data that you actually need is queried.
<br>  
## more . YAML files:

If you have thawing problems, you can easily build a small script in Iobroker or Homeassitant, if defrosting is detected should be switched to summer,

When heating is detected again, switches back to operating mode heating. Thus, a maximum of 0.7 degrees of hot water from the Sepicher is consumed.

For Iobroker I have uploaded a Blockly Script which I use myself and works great.<br>

The Defrost.yaml is intended for Home Assistant automation. This is the same automation as for IoBroker.

Tested with: - Rotex HSPU Compact 508 8kw
## WEBUI from ESP:
![Bildschirmfoto 2023-12-17 um 11 33 51](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/524a204e-801a-4fe7-aacb-ac2e731a99b7)


## View Homeassistant:
![Bildschirmfoto 2024-04-01 um 01 37 19](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/d9c1d703-ac4a-4466-97e3-dbd6478cbed1)


## Pin assignment GPIO 47 and 48

![ESP32-S3toCAN](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/40b1881a-b7f5-40b5-a2d7-678ee19299d2)

## Pin assignment at the Rotex HPSU

![Rotex CAN Anschluss](https://github.com/Trunks1982/Daikin-Rotex-HPSU-CAN/assets/62701386/05c36ae7-ddc9-4a1e-8a73-4559c765f132)








