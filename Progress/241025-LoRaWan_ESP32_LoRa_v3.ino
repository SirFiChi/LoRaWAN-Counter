/* LoRaWAN-Counter
 *
 * LHM - KUL-BIB-G4
 * 
 * Zählt die Auslösung an zwei Kontakten (per Lichtschranke), speichert diese auf dem ESP32 und
 * auf einem DS3231 (RTC) und überträgt die Zählung zu TTN.
 * Wird der Controller vom Strom getrennt und wieder eingeschaltet, wird der Wert aus dem Speicher
 * geholt und weiter gezählt.
 * Über Jumper lässt sich einstellen ob der Wert 1:1, halbiert oder geviertelt übergeben werden soll,
 * wenn z.B. der Eingang auch ein Ausgang ist und somit Personen doppelt zählen würde.
 * 
 * Floh Klare
 * https://github.com/SirFiChi/LoRaWAN-Counter
 *
 *
 **********
 *
 * Aktueller Stand:
 * - Teilen des Ergebnisses falls ein Jumper gesetzt wurde fehlt
 * - Auslesen/Speichern auf dem RTC funktioniert noch nicht
 * - LoRaWAN-Funktionalität noch nicht implementiert
 * - Zeitsteuerung über den RTC, senden nur alle 6 Minuten, muss implementiert werden
 * - Kommentare durch gehen und ggf. korrigieren
 * - Gehäuse für ESP32 muss entworfen und gedruckt werden
 *
 * - Speichern/Auslesen aus ESP32 funktioniert mit drei Werten
 * - Anzeige funktioniert korrekt
 * - Zählung der Werte funktioniert korrekt (inkl. Addition)
 * - Sicherungskasten und Platine fertig, außer Gehäuse des ESP32
 * 
 **********
 *
 *     Bekannte Fehler und ToDos
 *  - Controller sendet aktuell nur einmal, da LoRaWAN.sleep(loraWanClass) deaktiviert ist. Wird es aktiviert fällt der Controller in den Schlaf und zählt nicht weiter.
 *  - Die Werte müssen im Speicher abgelegt und wieder ausgelesen werden
 *  - Aktuell wird nur ein Testwert zum TTN übertragen, nicht die Besucherzählung
 *  - Ein zeitgesteuertes Senden (alle X Minuten) muss mit Hilfe des RTC Moduls implementiert werden
*/

/* Module laden */
#include <ezButton.h>                                                // Modul für die "Taster", zur Zählung der Auslösung an den Lichtschranken */
#include "Arduino.h"                                                 // Lade Modul für die Nutzung des Arduindo Chips
#include <Wire.h>                                                    // Lade Modul für die Nutzung der Verbindung zum RTC DS3231
#include "uEEPROMLib.h"                                              // Lade Modul für die Nutzung des EEPROMs auf der RTC DS3231
#include <Preferences.h>                                             // Lade Modul zum Speichern auf dem ESP32
#include <EEPROM.h>                                                  // Lade das Modul für den EEPROM
#include "HT_SSD1306Wire.h"                                          // Modul für das Display

/* KONFIGURATION */
/* Erstellt die beiden Eingänge für den Kontaktschalter (Lichtschranke) */
ezButton button1(47);                                                // Kontakt 1 auf Pin 47
ezButton button2(48);                                                // Kontakt 2 auf Pin 48
/* Erstellt die beiden Eingänge für den Kontaktschalter (Lichtschranke) */
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // Konfiguriert das Display
/* Speicheradresse des RTC, definiert über Lötbrücken */
uEEPROMLib eeprom(0x57);                                             // Konfiguration der Speicheradresse auf der RTC (Lötbrücken auf der HW)

#define I2C_SDA 41                                                   // SDA Pin für die RTC
#define I2C_SCL 42                                                   // SCL Pin für die RTC
Preferences preferences;
#define EEPROM_SIZE 16                                               // Setze die Größe des Speichers auf 4 Byte
#define ON LOW                                                       // Definiert ON als LOW, für den Status der Pins
#define OFF HIGH                                                     // Definiert OFF als HIGH, für den Status der Pins

/* Definiere und befülle ein paar Variablen */
  unsigned int eeAddress0rtc = 0;                                    // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im RTC-EEPROM)
  unsigned int eeAddress1rtc = 4;                                    // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im RTC-EEPROM)
  unsigned int eeAddressallrtc = 8;                                  // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im RTC-EEPROM)
  unsigned int eeAddress0esp = 0;                                    // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im EEPROM)
  unsigned int eeAddress1esp = 4;                                    // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im EEPROM)
  unsigned int eeAddressallesp = 8;                                  // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im EEPROM)
  unsigned long count1 = 0;                                          // für Kontakt 1
  unsigned long count2 = 0;                                          // für Kontakt 2
  unsigned long countall = count1 + count2;                          // Addition der beiden Kontakte
  unsigned long count1rtc = 0;                                       // für Kontakt 1 auf der RTC
  unsigned long count2rtc = 0;                                       // für Kontakt 2 auf der RTC
  unsigned long countallrtc = count1rtc + count2rtc;                 // Addition der beiden Kontakte auf der RTC
  unsigned long count1esp = 0;                                       // für Kontakt 1 auf dem ESP32
  unsigned long count2esp = 0;                                       // für Kontakt 2 auf dem ESP32
  unsigned long countallesp = count1esp + count2esp;                 // Addition der beiden Kontakte auf dem ESP32
  unsigned long count1real = 0;                                      // für Kontakt 1
  unsigned long count2real = 0;                                      // für Kontakt 2
  unsigned long countallreal = 0;                                    // Addition der beiden Kontakte
  String winner;

void setup() {
  Serial.begin(115200);                                              // Setze die serielle Geschwindigkeit auf 115200 baud (für serielle Konsole)

  display.init();                                                    // Initialisiert das Display
  display.setFont(ArialMT_Plain_10);                                 // Setzt die Standardschrift und -größe für das Display

  Wire.begin();                                                      // Initialisiert die Verbindung zum I2C als Master
  eeprom.eeprom_read(eeAddress0rtc, &count1rtc);                     // Rufe den gespeicherten Wert vom RTC-EEPROM ab und fülle damit "iAlt"
  eeprom.eeprom_read(eeAddress1rtc, &count2rtc);                     // Rufe den gespeicherten Wert vom RTC-EEPROM ab und fülle damit "iAlt"
  eeprom.eeprom_read(eeAddressallrtc, &countallrtc);                 // Rufe den gespeicherten Wert vom RTC-EEPROM ab und fülle damit "iAlt"
  Serial.println((String)"count1rtc: " + count1rtc);
  Serial.println((String)"count2rtc: " + count2rtc);
  Serial.println((String)"countallrtc: " + countallrtc);
  EEPROM.begin(EEPROM_SIZE);                                         // Initialisiere den EEPROM auf die Größe
  count1esp = EEPROM.get( eeAddress0esp, count1esp );                // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "iAlt"
  count2esp = EEPROM.get( eeAddress1esp, count2esp );                // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "iAlt"
  countallesp = EEPROM.get( eeAddressallesp, countallesp );          // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "iAlt"

  button1.setDebounceTime(50);                                       // Setzt die Nachfederunterdrückung des Kontakt1 auf 50ms
  button1.setCountMode(COUNT_FALLING);
  button2.setDebounceTime(50);                                       // Setzt die Nachfederunterdrückung des Kontakt2 auf 50ms
  button2.setCountMode(COUNT_FALLING);

  pinMode(20, INPUT_PULLUP);                                         // Definiert Port 20 als Pullup Input
  pinMode(26, INPUT_PULLUP);                                         // Definiert Port 26 als Pullup Input

  int jumper0 = digitalRead(20);                                     // Liest Status von Port 20
  int jumper1 = digitalRead(26);                                     // Liest Status von Port 26

  /* VERHALTEN WENN BEIDE JUMPER ON ==> RESET ALLER WERTE! */
  if ((jumper0 == ON) && (jumper1 == ON)){
     /* LÖSCHUNG ALLER DATEN */
    unsigned long null = 0;
    eeprom.eeprom_write(eeAddress0rtc, null);                        // Schreibe den Wert aus "null" in den RTC-EEPROM
    eeprom.eeprom_write(eeAddress1rtc, null);                        // Schreibe den Wert aus "null" in den RTC-EEPROM
    eeprom.eeprom_write(eeAddressallrtc, null);                      // Schreibe den Wert aus "null" in den RTC-EEPROM
    EEPROM.put(eeAddress0esp, null);                                 // Schreibe den Wert aus "null" in den ESP-EEPROM
    EEPROM.commit();                                                 // Bestätige das Schreiben in den ESP-EEPROM
    EEPROM.put(eeAddress1esp, null);                                 // Schreibe den Wert aus "null" in den ESP-EEPROM
    EEPROM.commit();                                                 // Bestätige das Schreiben in den ESP-EEPROM
    EEPROM.put(eeAddressallesp, null);                               // Schreibe den Wert aus "null" in den ESP-EEPROM
    EEPROM.commit();                                                 // Bestätige das Schreiben in den ESP-EEPROM
    /* Anweisung am Bildschirm wegen Löschung*/
    display.clear();                                                 // Displayinhalt löschen
    display.setTextAlignment(TEXT_ALIGN_CENTER);                     // Ausrichtung der Schrift ist links
    display.setFont(ArialMT_Plain_16);                               // Schriftgröße auf 24
    display.drawString(63, 0, "LÖSCHUNG");                           // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
    display.drawString(63,24, "ALLER WERTE");                        // Gibt Text aus
    display.drawString(63,48, "DURCHGEFÜHRT");                       // Gibt Text aus
    display.display();                                               // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
    delay(2500);                                                     // Warte 2,5 Sekunden bis es weiter geht
    display.clear();                                                 // Displayinhalt löschen
    display.setTextAlignment(TEXT_ALIGN_CENTER);                     // Ausrichtung der Schrift ist links
    display.setFont(ArialMT_Plain_16);                               // Schriftgröße auf 24
    display.drawString(63, 0, "JUMPER");                             // Gibt "LoRaWAN-Counter" an Koordinate 0,0 aus 
    display.drawString(63,24, "ENTFERNEN");                          // Gibt Text aus
    display.drawString(63,48, "UND NEU STARTEN");                    // Gibt Text aus
    display.display();                                               // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  }

/* Output auf dem Display, unterbrochen von einem Vergleich der Speicherinhalte */
/* Titel-Bildschirm */
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_CENTER);                       // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_24);                                 // Schriftgröße auf 24
  display.drawString(63, 0, "LoRaWAN");                              // Gibt "LoRaWAN" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_24);                                 // Schriftgröße auf 16
  display.drawString(63,24, "Counter");                              // Gibt Text aus
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(63,52, "KULT-BIB-G4");                          // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2500);                                                       // Warte 2,5 Sekunden bis es weiter geht

/* EEPROM Status */
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status EEPROM");                         // Gibt "Status EEPROM" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall: " + String(countallesp));      // Gibt Text und Variable aus
  display.drawString(0,31, "count1: " + String(count1esp));          // Gibt Text und Variable aus
  display.drawString(0,42, "count2: " + String(count2esp));          // Gibt Text und Variable aus
  display.drawString(110,52, "2/5");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunden bis es weiter geht

/* RTC Status */
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status RTC");                            // Gibt "Status RTC" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall: " + String(countallrtc));      // Gibt Text und Variable aus
  display.drawString(0,31, "count1: " + String(count1rtc));          // Gibt Text und Variable aus
  display.drawString(0,42, "count2: " + String(count2rtc));          // Gibt Text und Variable aus
  display.drawString(110,52, "3/5");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunde bis es weiter geht

/* Laufzeit Status */
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status Aktuell");                        // Gibt "Status Aktuell" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall: " + String(countall));         // Gibt Text und Variable aus
  display.drawString(0,31, "count1: " + String(count1));             // Gibt Text und Variable aus
  display.drawString(0,42, "count2: " + String(count2));             // Gibt Text und Variable aus
  display.drawString(110,52, "4/5");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunde bis es weiter geht

// Vergleich in welchem Speicher der größte Wert liegt (Flüchtig, RTC, ESP32) //
/* Bei der Auswertung des Gesamtergebnis setzen wir noch einen Gewinner, der beim Start auf dem Display ausgegeben wird */
  if ((countall <= countallesp) && (countall <= countallrtc)){
    if (countallrtc <= countallesp){
      count1 = count1esp;
      count2 = count2esp;
      countall = countallesp;
      winner = "ESP32 EEPROM";
    } else {
      count1 = count1rtc;
      count2 = count2rtc;
      countall = countallrtc;
      winner = "RTC EEPROM";
    }
  } else {
    winner = "Interner Speicher";
  }

/* Welcher Wert wird für den weiteren Lauf verwendet */
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Größter Wert:");                         // Gibt "Größter Wert:" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, winner);                                  // Gibt Text und Variable aus
  display.drawString(0,31, "");                                      // Gibt Text und Variable aus
  display.drawString(0,42, "Kopiere Wert und starte");               // Gibt Text und Variable aus
  display.drawString(110,52, "5/5");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunden bis es weiter geht
}

void loop() {
 /* Ablauf für das Erkennen der Lichtschranke als "Buttons" */
  button1.loop();                                                    // Die notwendige Loop-Funktion für den ersten Kontakt
  button2.loop();                                                    // Die notwendige Loop-Funktion für den zweiten Kontakt

  /* Frage die Werte aus den gezählten Auslösungen aus und schreibe sie in Variablen für die Ausgabe am Display */
  unsigned long count1real = button1.getCount() + count1;            // für Kontakt 1
  unsigned long count2real = button2.getCount() + count2;            // für Kontakt 2
  unsigned long countallreal = count1real + count2real;              // Addition der beiden Kontakte

/* Schreibe die Werte in das EEPROM des RTC */
  eeprom.eeprom_write(eeAddress0rtc, count1real);                    // Schreibe den Wert aus "iAlt" in den EEPROM
  eeprom.eeprom_write(eeAddress1rtc, count2real);                    // Schreibe den Wert aus "iAlt" in den EEPROM
  eeprom.eeprom_write(eeAddressallrtc, countallreal);                // Schreibe den Wert aus "iAlt" in den EEPROM

/* Schreibe die Werte in das EEPROM des ESP32 */
  EEPROM.put(eeAddress0esp, count1real);                             // Schreibe den Wert aus "iAlt" in den EEPROM
  EEPROM.commit();                                                   // Bestätige das Speichern in den EEPROM
  EEPROM.put(eeAddress1esp, count2real);                             // Schreibe den Wert aus "iAlt" in den EEPROM
  EEPROM.commit();                                                   // Bestätige das Speichern in den EEPROM
  EEPROM.put(eeAddressallesp, countallreal);                         // Schreibe den Wert aus "iAlt" in den EEPROM
  EEPROM.commit();                                                   // Bestätige das Speichern in den EEPROM

  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_24);                                 // Schriftgröße auf 24
  display.drawString(0, 0, "Besucher:");                             // Gibt "Besucher:" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0,28, String(countallreal));                    // Gibt die Addition von count1 und count2 an Koordinate 0,28 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,52, "(" + String(count1real) + "+" + String(count2real) + ")");  // Gibt die Einzelwerte der Kontaktauslösungen, in Klammern und mit einem Plus an Koordinate 0,52 aus 
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
}
