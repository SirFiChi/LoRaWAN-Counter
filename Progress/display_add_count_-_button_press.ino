/**
* Sources:
* - unknown
**/

#include <ezButton.h>                                                 // Modul für die "Taster", zur Zählung der Auslösung an den Lichtschranken */
#include <Wire.h>                                                     //
#include "HT_SSD1306Wire.h"                                           // Modul für das Display

/* Erstellt die beiden Eingänge für den Kontaktschalter (Lichtschranke) */
ezButton button1(47);                                                 // Kontakt 1 auf Pin 47
ezButton button2(48);                                                 // Kontakt 2 auf Pin 48
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // Konfiguriert das Display

void setup() {
  Serial.begin(115200);                                              // Setze die serielle Geschwindigkeit auf 115200 baud (für serielle Konsole)
  button1.setDebounceTime(50);                                       // Setzt die Nachfederunterdrückung des Kontakt1 auf 50ms
  button1.setCountMode(COUNT_FALLING);
  button2.setDebounceTime(50);                                       // Setzt die Nachfederunterdrückung des Kontakt2 auf 50ms
  button2.setCountMode(COUNT_FALLING);
  display.init();                                                    // Initialisiert das Display
  display.setFont(ArialMT_Plain_10);                                 // Setzt die Standardschrift und -größe für das Display
}

void loop()
{
  /* Ablauf für das Erkennen der Lichtschranke als "Buttons" */
  button1.loop();                                                    // Die notwendige Loop-Funktion für den ersten Kontakt
  button2.loop();                                                    // Die notwendige Loop-Funktion für den zweiten Kontakt

  /* Initialisiere die Variablen der zu zählenden Kontakte und addiere sie */
  unsigned long count1 = button1.getCount();                         // für Kontakt 1
  unsigned long count2 = button2.getCount();                         // für Kontakt 2
  unsigned long countall = button1.getCount() + button2.getCount();  // Addition der beiden Kontakte

  /* Hier kommt ein bisschen Output */
  /* Output auf der seriellen Schnittstelle */
  Serial.println("Gezählte Besucher:");
  Serial.println(countall);
  Serial.println("An Kontakt 1:" + count1);
  Serial.println("An Kontakt 2:" + count2);
  /* Output auf dem Display */
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
