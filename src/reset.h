/**********
 *
 * Löschen aller Daten im EEPROM ==> Zurücksetzen des Zählers.
 *
**********/


/**/
/* LÖSCHUNG ALLER DATEN */
/**/
    unsigned long null = 0;
    EEPROM.put(eeAddress0esp, null);                                // Schreibe den Wert aus "null" in den ESP-EEPROM
    EEPROM.commit();                                                // Bestätige das Schreiben in den ESP-EEPROM
    EEPROM.put(eeAddress1esp, null);                                // Schreibe den Wert aus "null" in den ESP-EEPROM
    EEPROM.commit();                                                // Bestätige das Schreiben in den ESP-EEPROM
    EEPROM.put(eeAddressallesp, null);                              // Schreibe den Wert aus "null" in den ESP-EEPROM
    EEPROM.commit();                                                // Bestätige das Schreiben in den ESP-EEPROM


/**/
/* Hinweis am Display, dass die Daten gelöscht werden */
/**/
    display.clear();                                                // Displayinhalt löschen
    display.setTextAlignment(TEXT_ALIGN_CENTER);                    // Ausrichtung der Schrift ist zentriert
    display.setFont(ArialMT_Plain_16);                              // Schriftgröße auf 16
    display.drawString(63, 0, "LÖSCHUNG");                          // Gibt "LÖSCHUNG" an Koordinate 63,0 aus 
    display.drawString(63,24, "ALLER WERTE");                       // Gibt "ALLER WERTE" an Koordinate 63,24 aus
    display.setFont(ArialMT_Plain_10);                              // Schriftgröße auf 10
    display.drawString(63,48, "DURCHGEFÜHRT");                      // Gibt "DURCHGEFÜHRT" an Koordinate 63,48 aus
    display.display();                                              // Befehl um die Bildausgabe zu starten
    delay(2500);                                                    // Warte 2,5 Sekunden bis es weiter geht
    display.clear();                                                // Displayinhalt löschen
    display.setTextAlignment(TEXT_ALIGN_CENTER);                    // Ausrichtung der Schrift ist zentriert
    display.setFont(ArialMT_Plain_16);                              // Schriftgröße auf 24
    display.drawString(63, 0, "JUMPER");                            // Gibt "JUMPER" an Koordinate 63,0 aus
    display.drawString(63,24, "ENTFERNEN");                         // Gibt "ENTFERNEN" an Koordinate 63,24 aus
    display.setFont(ArialMT_Plain_10);                              // Schriftgröße auf 10
    display.drawString(63,48, "UND NEU STARTEN");                   // Gibt "UND NEU STARTEN" an Koordinate 63,48 aus
    display.display();                                              // Befehl um die Bildausgabe zu starten
    delay(9999999);                                                 // Wartet lange, damit die Anzeige stehen bleibt
