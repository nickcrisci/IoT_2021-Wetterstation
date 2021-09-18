# IoT_2021-Wetterstation

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
