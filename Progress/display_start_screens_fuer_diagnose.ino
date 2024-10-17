/**
*
*  UNGETESTET - DER ESP32 ist gerade nicht zur Hand. Kompilieren war erfolgreich.
*
**/

/**
* Sources:
* - unknown
**/

#include <Wire.h>                                                     //
#include "HT_SSD1306Wire.h"                                           // Modul für das Display

/* Erstellt die beiden Eingänge für den Kontaktschalter (Lichtschranke) */
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // Konfiguriert das Display

/* Befülle ein paar Variablen für die Ausgabe am Display */
  unsigned long count1 = 234235;                                     // für Kontakt 1
  unsigned long count2 = 5435345;                                    // für Kontakt 2
  unsigned long countall = count1 + count2;  // Addition der beiden Kontakte

void setup() {
  display.init();                                                    // Initialisiert das Display
  display.setFont(ArialMT_Plain_10);                                 // Setzt die Standardschrift und -größe für das Display

    /* Output auf dem Display */
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_24);                                 // Schriftgröße auf 24
  display.drawString(0, 0, "LoRaWAN-Counter");                       // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0,28, "KULT-BIB-G4");                           // Gibt Text aus
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.setTextAlignment(TEXT_ALIGN_RIGHT);                        // Ausrichtung der Schrift ist rechts
  display.drawString(0,52, "1/5");                                   // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(1500);                                                       // Warte 1,5 Sekunden bis es weiter geht

  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status EEPROM");                         // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall:" + countall);                  // Gibt Text und Variable aus
  display.drawString(0,31, "count1:" + count1);                      // Gibt Text und Variable aus
  display.drawString(0,42, "count2:" + count2);                      // Gibt Text und Variable aus
  display.setTextAlignment(TEXT_ALIGN_RIGHT);                        // Ausrichtung der Schrift ist rechts
  display.drawString(0,52, "2/5");                                   // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(1000);                                                       // Warte 1 Sekunde bis es weiter geht

  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status RTC-EEPROM");                     // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall:" + countall);                  // Gibt Text und Variable aus
  display.drawString(0,31, "count1:" + count1);                      // Gibt Text und Variable aus
  display.drawString(0,42, "count2:" + count2);                      // Gibt Text und Variable aus
  display.setTextAlignment(TEXT_ALIGN_RIGHT);                        // Ausrichtung der Schrift ist rechts
  display.drawString(0,52, "3/5");                                   // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(1000);                                                       // Warte 1 Sekunde bis es weiter geht

  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status Aktuell");                        // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall: 0");                           // Gibt Text und Variable aus
  display.drawString(0,31, "count1: 0");                             // Gibt Text und Variable aus
  display.drawString(0,42, "count2: 0");                             // Gibt Text und Variable aus
  display.setTextAlignment(TEXT_ALIGN_RIGHT);                        // Ausrichtung der Schrift ist rechts
  display.drawString(0,52, "4/5");                                   // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(1000);                                                       // Warte 1 Sekunde bis es weiter geht

  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Größter Wert:");                         // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "EEPROM");                                // Gibt Text und Variable aus
  display.drawString(0,31, "");                                      // Gibt Text und Variable aus
  display.drawString(0,42, "Kopiere Wert und starte Zählung");       // Gibt Text und Variable aus
  display.setTextAlignment(TEXT_ALIGN_RIGHT);                        // Ausrichtung der Schrift ist rechts
  display.drawString(0,52, "5/5");                                   // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunden bis es weiter geht

  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_24);                                 // Schriftgröße auf 24
  display.drawString(0, 0, "Besucher:");                             // Gibt "Besucher:" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0,28, String(countall));                        // Gibt die Addition von count1 und count2 an Koordinate 0,28 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,52, "(" + String(count1) + "+" + String(count2) + ")");  // Gibt die Einzelwerte der Kontaktauslösungen, in Klammern und mit einem Plus an Koordinate 0,52 aus 
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
}

void loop() {
/* Loop lassen wir leer. Wir wollen nur eine einmalige Ausgabe am Display in Setup */
/* Dies wird die Startsequenz des ESP32, für "Diagnose-Zwecke" */
/* Es folgt noch ein Statusscreen für die LoRaWAN Anbindung ans TTN */
}
