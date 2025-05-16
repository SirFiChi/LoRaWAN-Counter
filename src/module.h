/**********
 *
 * Laden der Module als Hardware-Treiber und für Funktionalität
 *
**********/


/**/
/* Module laden */
/**/
#include <heltec_unofficial.h>
#include <LoRaWAN_ESP32.h>                                          // Board-Treiber für die LoRaWan Funktionalität
#include <ezButton.h>                                               // Modul für die "Taster", zur Zählung der Auslösung an den Lichtschranken */
#include "Arduino.h"                                                // Lade Modul für die Nutzung des Arduindo Chips
#include <Wire.h>                                                   // Lade Modul für die Nutzung der Verbindung zum Display
#include <Preferences.h>                                            // Lade Modul zum Speichern auf dem ESP32
#include <EEPROM.h>                                                 // Lade das Modul für den EEPROM
#include "settings.h"                                               // Einstellungen für ReferatID, DeviceID und TheThingsNetwork (OTTA)
#include "pins_arduino.h"                                           // Definiert PIN-Nummern und Funktionen
