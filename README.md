# IoT_2021-Wetterstation

## Bauanleitung
Für das Projekt wurde ein ESP32 mit Bauteilen und Sensoren von Grove verwendet. Sollten Sie andere Bauteile verwenden, kann es sein, dass Sie andere Bibliotheken benötigen.

### Bauteile
* ESP32
* Grove Base Shield v1.0 for ESP32-DevKitC (by ThingForward)
<img src="https://user-images.githubusercontent.com/65964109/131244330-b53510be-4a35-46ff-af6b-4886a2f660bb.png" width="250" height="200">

* Grove - Temperature and Humidity Sensor (v1.2)
<img src="https://user-images.githubusercontent.com/65964109/131242296-b34f276f-f838-41d7-a9b5-ee2552d66867.png" width="250" height="200">

* Grove - IMU 10DOF (v2.1)
<img src="https://files.seeedstudio.com/wiki/Grove-IMU_10DOF_v2.0/img/Grove-imu-10dof-v2.0.jpg" width="250" height="200">

* Grove - 4-Digit Display (v1.0)
<img src="https://files.seeedstudio.com/wiki/Grove-4-Digit_Display/img/Grove-4_digit_display.jpg" width="250" height="200">

* Grove - Button (v1.2)
<img src="https://res.cloudinary.com/rsc/image/upload/b_rgb:FFFFFF,c_pad,dpr_1.0,f_auto,h_337,q_auto,w_600/c_pad,h_337,w_600/Y1743248-01?pgw=1" width="220" height="200">

* Waveshare - Solar Power Manager
<img src="https://www.waveshare.com/img/devkit/accBoard/Solar-Power-Manager/Solar-Power-Manager-1.jpg" width="250" height="200">

* XCell - IFR 14500 (600mAh, 3.2V)
<img src="https://www.akkuman.de/shop/media/image/product/4494/lg/4x-xcell-ifr-14500-aa-solar-akku-lifepo4-32v-600mah~8.jpg" width="200" height="200">

* DEBO Solar 5W Entwicklerboards
<img src="https://cdn-reichelt.de/bilder/web/artikel_ws/A300/DEBO_SOLAR_5W_01.jpg" width="200" height="200">

### Schaltplan
<img width="1920" alt="Schaltplan" src="https://user-images.githubusercontent.com/65964109/132165112-9909c372-2904-4bb0-a2b2-2dbd50bf201b.png">

## Installation
...

Damit die Wetterstation eine W-Lan Verbindung aufbauen kann, und die Abweichung des Drucksensors durch die Höhenmeter mit einbezogen werden kann, muss in dem Ordner "src" eine neue Datei angelegt werden.
Diese benötigt den Namen "config.h". In dieser Datei werden dann die W-Lan SSID (Name) und das W-Lan Passwort, sowie die Höhenmeter am Standort der Wetterstation festgelegt.
Hierfür müssen folgende drei Zeilen der Datei hinzugefügt werden:
```c
#define SSID "Your SSID"
#define PASSWORD "Your Password"
#define ELEVATION <Your Elevation>
```

Dabei muss die Angabe der Höhenmeter als Zahl in Meter angegeben werden, z.B. 200.

## Fehlercodes

Das 4-Digit Display der Wetterstation ist in der Lage, einzelne Fehlercodes darzustellen, welche Auskunft über die W-Lan Verbindung, oder über den Zustand der Sensoren geben können. In der nachfolgenden Tabelle sind diese Fehlercodes kurz beschrieben.

Fehler Code | Bedeutung
----------- | -----------
E 1 1       | W-Lan Verbindung konnte nicht hergestellt werden
E 2 1       | Der Grove - IMU 10DOF wurde nicht gefunden, I2C Anschluss überprüfen
