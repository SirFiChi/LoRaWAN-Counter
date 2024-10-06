/**
* Sources:
* - https://docs.arduino.cc/learn/built-in-libraries/eeprom/#example-3
* - "Microcontroller ESP32", ISBN: 978-3-367-10284-6, Seite 425
**/

#include <EEPROM.h>                                                  // Lade das Modul für den EEPROM
#define EEPROM_SIZE 4                                                // Setze die Größe des Speichers auf 4 Byte
unsigned long eingabe = 0;                                           // Initialisiere die Variable "eingabe" als positive long und setze sie auf 0
unsigned long iAlt = 0;                                              // Initialisiere die Variable "iAlt" als positive long und setze sie auf 0
int eeAddress = 0;                                                   // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort im EEPROM)

void setup() {
  Serial.begin(115200);                                              // Setze die serielle Geschwindigkeit auf 115200 baud (für serielle Konsole)
  EEPROM.begin(EEPROM_SIZE);                                         // Initialisiere den EEPROM auf die Größe
  iAlt = EEPROM.get( eeAddress, iAlt );                              // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "iAlt"
  Serial.print ("Wert im EEPROM beim Starten: ");
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
      Serial.println ("  -> Im EEPROM gespeichert!");
      Serial.println ();
      iAlt = eingabe;                                                 // Setze die Variable "iAlt" auf den Wert der "eingabe"
      EEPROM.put(eeAddress, iAlt);                                    // Schreibe den Wert aus "iAlt" in den EEPROM
      EEPROM.commit();                                                // Bestätige das Speichern in den EEPROM
      Serial.println ("  ->Jetzt neu Starten und den Wert kontrollieren!");
    }
  }
}
