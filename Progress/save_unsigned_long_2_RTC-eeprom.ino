/**
* Sources:
* - Example-Code DS3231
* - Unknown
**/

#include "Arduino.h"                                                 // Lade Modul für die Nutzung des Arduindo Chips
#include "Wire.h"                                                    // Lade Modul für die Nutzung der Verbindung zum RTC DS3231
#include "uEEPROMLib.h"                                              // Lade Modul für die Nutzung des EEPROMs auf der RTC DS3231
uEEPROMLib eeprom(0x57);                                             // Konfiguration der Speicheradresse auf der RTC (Lötbrücken auf der HW)
unsigned int eingabe = 0;                                            // Initialisiere die Variable "eingabe" als positive long und setze sie auf 0
unsigned int iAlt = 0;                                               // Initialisiere die Variable "iAlt" als positive long und setze sie auf 0
unsigned int eeAddress = 0;                                          // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im EEPROM)

void setup() {
  Serial.begin(115200);                                              // Setze die serielle Geschwindigkeit auf 115200 baud (für serielle Konsole)
  Wire.begin();                                                      // Initialisiert die Verbindung zum I2C als Master
  eeprom.eeprom_read(eeAddress, &iAlt);                              // Rufe den gespeicherten Wert vom RTC-EEPROM ab und fülle damit "iAlt"
  Serial.print ("Wert im RTC-EEPROM beim Starten: ");
  Serial.println (iAlt);                                             // Gebe "iAlt" aus
  Serial.println ();
  Serial.println ("Bitte zu speichernden Wert (0 - 4.294.967.295) eingeben und den ESP32 neu starten.");
}

void loop() {
  if (Serial.available() > 0) {                                       // Warte Eingabe ab
    eingabe = Serial.parseInt();                                      // Setze Wert der Eingabe auf die Variable "eingabe"
    if ((eingabe != iAlt) && (eingabe != 0)){                         // Kontrolliere ob die Eingabe gültig ist
      Serial.print ("Neuer Wert: ");
      Serial.println (eingabe);                                       // Gibt den Wert der gerade getätigten "Eingabe" aus
      Serial.println ("  -> Im RTC-EEPROM gespeichert!");
      Serial.println ();
      iAlt = eingabe;                                                 // Setze die Variable "iAlt" auf den Wert der "eingabe"
      eeprom.eeprom_write(eeAddress, iAlt);                           // Schreibe den Wert aus "iAlt" in den EEPROM
      Serial.println ("  ->Jetzt neu Starten und den Wert kontrollieren!");
    }
  }
}
