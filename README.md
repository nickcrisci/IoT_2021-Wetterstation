# IoT_2021-Wetterstation

## Installation
...
Damit die Wetterstation eine W-Lan Verbindung aufbauen kann, muss in dem Ordner "src" eine neue Datei angelegt werden.
Diese benötigt den Namen "wifi_secrets.h". In dieser Datei werden dann die W-Lan SSID (Name) und das W-Lan Passwort festgelegt.
Hierfür müssen folgende zwei Zeilen der Datei hinzugefügt werden:
```c
#define SSID "Your SSID"
#define PASSWORD "Your Password"
```
