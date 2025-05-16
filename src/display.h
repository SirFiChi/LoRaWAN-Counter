/**********
 *
 * Funktionen zum Ein-/Ausschalten und Resetten des Display
 * Quellen: Heltec - Heltec.cpp und HT_Display.cpp
 *
**********/


/**/
/* Schaltet den Strom für das Display ein (Seit BoardVersion 3.2 notwendig)*/
/**/
void VextON(void) {
  pinMode(Vext, OUTPUT);                                            // Definiert Vext als Ausgang
  digitalWrite(Vext, LOW);                                          // Setzt Vext auf LOW
}



/**/
/* Schaltet den Strom für das Display aus (Seit BoardVersion 3.2)*/
/**/
void VextOFF(void)  //Vext default OFF
{
  pinMode(Vext, OUTPUT);                                            // Definiert Vext als Ausgang
  digitalWrite(Vext, HIGH);                                         // Setzt Vext auf HIGH
}



/**/
/* Setzt das Display zurück (Seit BoardVersion 3.2)*/
/**/
void displayReset(void) {
  pinMode(RST_OLED, OUTPUT);                                        // Definiert RST_OLED als Ausgang
  digitalWrite(RST_OLED, HIGH);                                     // Setzt RST_OLED auf HIGH
  delay(1);                                                         // Warte kurz
  digitalWrite(RST_OLED, LOW);                                      // Setzt RST_OLED auf LOW
  delay(1);                                                         // Warte kurz
  digitalWrite(RST_OLED, HIGH);                                     // Setzt RST_OLED auf HIGH
  delay(1);                                                         // Warte kurz
}
