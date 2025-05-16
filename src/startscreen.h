/**********
 *
 * Ausgabe der Startsequenz auf dem Display.
 *
**********/


/**/
/* Titel-Bildschirm */
/**/
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


/**/
/* EEPROM Status */
/**/
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Status EEPROM");                         // Gibt "Status EEPROM" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "countall: " + String(countallesp));      // Gibt Text und Variable aus
  display.drawString(0,31, "count1: " + String(count1esp));          // Gibt Text und Variable aus
  display.drawString(0,42, "count2: " + String(count2esp));          // Gibt Text und Variable aus
  display.drawString(110,52, "2/4");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunden bis es weiter geht


/**/
/* Teiler Status */
/**/
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Definierter Teiler");                    // Gibt "Status Aktuell" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, String(teiler));                          // Gibt Text und Variable aus
  display.drawString(110,52, "3/4");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunde bis es weiter geht


/**/
/* Welcher Wert wird für den weiteren Lauf verwendet */
/**/
  display.clear();                                                   // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                         // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                 // Schriftgröße auf 16
  display.drawString(0, 0, "Los geht's");                            // Gibt "Größter Wert:" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                 // Schriftgröße auf 10
  display.drawString(0,20, "");                                      // Gibt Text und Variable aus
  display.drawString(0,31, "Beginne Zählung");                       // Gibt Text und Variable aus
  display.drawString(0,42, "");                                      // Gibt Text und Variable aus
  display.drawString(110,52, "4/4");                                 // Gibt Text aus
  display.display();                                                 // !!! Dieser Befehl gibt nun tatsächlich die gerade definierten Texte auf dem Display aus
  delay(2000);                                                       // Warte 2 Sekunden bis es weiter geht
