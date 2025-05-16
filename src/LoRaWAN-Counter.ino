/**********
 *
 * LoRaWAN-Counter v1.0
 *
 **********
 *
 * LHM - KULT-BIB-G4
 *
 **********
 * 
 * Zählt die Auslösung an bis zu zwei Kontakten (z.B. per Lichtschranke), speichert diese auf dem ESP32
 * und überträgt die Zählung zum TTN.
 * Wird der Controller vom Strom getrennt und wieder eingeschaltet, wird der Wert aus dem Speicher
 * geholt und weiter gezählt.
 * Über Jumper lässt sich einstellen ob die Werte "1:1", halbiert oder geviertelt übergeben werden sollen.
 * (Wenn z.B. der Eingang auch ein Ausgang ist und somit Personen doppelt zählen würde, und/oder die
 * Lichtschranke so tief hängt, dass Beine und nicht Personen gezählt werden.)
 * Der Faktor kann nur für die gesamte Einheit, nicht individuell für die Kontakte, gesetzt werden.
 * 
 **********
 *
 * Die Daten für das TTN werden bei der ersten Nutzung des Skripts im seriellen Monitor abgefragt und eingegeben.
 * Siehe: https://github.com/ropg/LoRaWAN_ESP32
 * Diese Daten können in der Arduino IDE über "Erease all flash before sketch upload" zurückgesetzt werden.
 *
 **********
 *
 * Floh Klare
 * https://github.com/SirFiChi/LoRaWAN-Counter
 *
 **********
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *   Die Boardtreiber esp32 von Espressif Systems in der Version 3.2.0 sind defekt und verhindern das Komplieren!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 **********
 *
 * HINWEIS: Die Lebensdauer des Speichers ist begrenzt. Sollte ein Bereich defekt sein, kann eine andere Adresse verwendet werden.
 *
**********/


/**/
/* DEBUG-Nachrichten aktivierbar, um den Ablauf zu verstehen und Problemquellen einzukreisen */
/**/
int DEBUG = 1;                                                      // DEBUG-Modus, 1=an, 0= aus
  

/**/
/* Definiert eine minimale Wartezeit, um die Richtlinien des TTN einzuhalten */
/**/
#define MINIMUM_DELAY 120                                           // Setzt das minimale Wartezeit auf 120 Sekunden


/**/
/* Module für Funktionalität und Hardware laden */
/**/
#include "module.h"                                                 // Hier gibt es nichts zu konfigurieren, außer es werden andere Komponenten verwendet.


/**/
/* KONFIGURATION der zuvor geladenen Module  */
/**/
#include "modulconfig.h"                                            // Die Grundkonfiguration der Module. Hier ist nur das Sende-Interval relevant.


/**/
/* Definiere und befülle ein paar globale Variablen */
/**/
  #inculde "config.h"                                               // !!! Hier werden die individuellen Einstellungen gesetzt !!!
  boolean initLoRaWANonce = false;                                  // Hilfsvariable, damit die Initialisierung des Aufbaus zum TTN nur einmal stattfindet
  boolean runLoRaWAN = false;                                       // Hilfsvariable, damit bereits nach dem Einschalten Messdaten gesendet werden, und nicht erst nach Ablauf der Zeit
  unsigned int eeAddress0esp = 0;                                   // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort 0 im EEPROM)
  unsigned int eeAddress1esp = 4;                                   // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort 4 im EEPROM)
  unsigned int eeAddressallesp = 8;                                 // Initialisiere die Variable "eeAddress" als int und setze sie auf 0 (für Speicherort 8 im EEPROM)
  unsigned long count1 = 0;                                         // für Kontakt 1
  unsigned long count2 = 0;                                         // für Kontakt 2
  unsigned long countall = count1 + count2;                         // Addition der beiden Kontakte
  unsigned long count1esp = 0;                                      // für Kontakt 1 auf dem ESP32
  unsigned long count2esp = 0;                                      // für Kontakt 2 auf dem ESP32
  unsigned long countallesp = count1esp + count2esp;                // Addition der beiden Kontakte auf dem ESP32
  unsigned long count1real = 0;                                     // für Kontakt 1
  unsigned long count2real = 0;                                     // für Kontakt 2
  unsigned long countallreal = 0;                                   // Addition der beiden Kontakte
  unsigned long count1Show = 0;                                     // für Kontakt 1
  unsigned long count2Show = 0;                                     // für Kontakt 2
  unsigned long countallShow = 0;                                   // Addition der beiden Kontakte
  uint32_t count1Trans = count1Show;                                // für Kontakt 1
  uint32_t count2Trans = count2Show;                                // für Kontakt 2
  uint32_t countallTrans = countallShow;                            // für beide Kontakte
  unsigned long teiler;                                             // Wert durch den die Zählung geteilt wird
  unsigned long previousMillis = 0;                                 // Für die Zeitsteuerung zum Versand des PayLoads
  const long sendinterval = funkInterval * 60 * 1000;               // Rechnet das Sendeinterval in Millisekunden um

/**/
/* Funktionen um das Display zu steuern */
/**/
 #include "display.h"                                               // Funktionen zum de-/aktivieren und Resetten des Displays. Hier gibt es nichts zu konfigurieren.



void setup() {
  if (DEBUG == 1){Serial.println("DEBUG-Modus ist eingeschaltet.");}
  if (DEBUG == 1){Serial.println("DEBUG: Die Module wurden geladen.");}
  if (DEBUG == 1){Serial.println("DEBUG: Die Module wurden konfiguriert.");}
  if (DEBUG == 1){Serial.println("DEBUG: Die Variablen wurden angelegt.");}
  if (DEBUG == 1){Serial.println("DEBUG: Die Funktionen für die (De-)Aktivierung des Displays wurden eingebunden.");}

  Serial.begin(115200);                                             // Setze die serielle Geschwindigkeit auf 115200 baud (für serielle Konsole)
  heltec_setup();

  VextON();                                                         // Schaltet das Display ein, wichtig für Version V3.2 des Boards
  displayReset();                                                   // Resetet das Display, wichtig für Version V3.2 des Boards
  display.init();                                                   // Initialisiert das Display
  display.setFont(ArialMT_Plain_10);                                // Setzt die Standardschrift und -größe für das Display

  button1.setDebounceTime(50);                                      // Setzt die Nachfederunterdrückung des Kontakt1 auf 50ms, damit das Schließen des Kontakts nur einmal gezählt wird
  button1.setCountMode(COUNT_FALLING);
  button2.setDebounceTime(50);                                      // Setzt die Nachfederunterdrückung des Kontakt2 auf 50ms, damit das Schließen des Kontakts nur einmal gezählt wird
  button2.setCountMode(COUNT_FALLING);

  Wire.begin();                                                     // Initialisiert die Verbindung zum I2C als Master
  pinMode(20, INPUT_PULLUP);                                        // Definiert Port 20 als Pullup Input
  pinMode(26, INPUT_PULLUP);                                        // Definiert Port 26 als Pullup Input
  int jumper0 = digitalRead(20);                                    // Liest Status von Jumper 0 an Port 20
  int jumper1 = digitalRead(26);                                    // Liest Status von Jumper 1 an Port 26
  if (DEBUG == 1){
    Serial.print("Jumper 0 ist gesetzt auf: ");
    Serial.println(jumper0);
    Serial.print("Jumper 1 ist gesetzt auf: ");
    Serial.println(jumper1);
    Serial.println("(0 = Jumper gesteckt, 1 = Jumper nicht gesteckt)");
  }
  while ((jumper0 == 0) && (jumper1 == 0)){                         // Wenn Jumper 0&1 überbrückt...
    #include "reset.h"                                              // ...inkludiere reset.h um beim Booten die Werte im Speicher zu löschen.
    if (DEBUG == 1){Serial.println("DEBUG: Da beide Jumper überbrückt wird der Speicher gelöscht.");}
  }
  #include "jumper.h"                                               // Inkludiert die ausgelagerte Jumper-Abfrage


  EEPROM.begin(EEPROM_SIZE);                                        // Initialisiere den EEPROM auf die Größe
  if (DEBUG == 1){Serial.println("DEBUG: Speicher initialisiert mit definierter Größe.");}
  count1esp = EEPROM.get( eeAddress0esp, count1esp );               // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "count1esp"
  count2esp = EEPROM.get( eeAddress1esp, count2esp );               // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "count2esp"
  countallesp = EEPROM.get( eeAddressallesp, countallesp );         // Rufe den gespeicherten Wert vom EEPROM ab und fülle damit "countallesp"
  if (DEBUG == 1){Serial.println("DEBUG: Die Werte wurden aus dem Speicher abgerufen.");}

 /**/
 /* Gibt die Willkommensnachrichten des Startbildschirms aus */
 /**/
  #include "startscreen.h"                                          // Inkludiert die ausgelagerte Ausgabe des Startbildschirms


 /**/
 /* Diverse DEBUG-Ausgaben am Ende von setup() */
 /**/  
  if (DEBUG == 1){Serial.println("DEBUG: Der Startscreen wurde am Display dargestellt.");}
  if (DEBUG == 1){Serial.println("DEBUG: setup() abgeschlossen.");}
  if (DEBUG == 1){Serial.print("DEBUG: Aktuelle Systemzeit in Millis: ");}
  if (DEBUG == 1){Serial.println(millis());}                        // Gibt die aktuelle Laufzeit "millis()" aus
}



void loop() {
 /**/
 /* Abfragen der Kontakte der Jumper, für die Verarbeitung als Teiler oder den Reset der Zählung */
 /**/
  int jumper0 = digitalRead(20);                                    // Liest Status von Port 20
  int jumper1 = digitalRead(26);                                    // Liest Status von Port 26


 /**/
 /* Erkennen der Kontakte als "Buttons" */
 /**/
  button1.loop();                                                   // Die notwendige Loop-Funktion für den ersten Kontakt
  button2.loop();                                                   // Die notwendige Loop-Funktion für den zweiten Kontakt
    unsigned long count1real = button1.getCount() + count1esp;      // für Kontakt 1
    unsigned long count2real = button2.getCount() + count2esp;      // für Kontakt 2
    unsigned long countallreal = count1real + count2real;           // Addition der beiden Kontakte zum Speichern
    unsigned long count1Show = count1real/teiler;                   // Erstellt Variable für den Wert der angezeigt werden soll, angepasst durch den Teiler
    unsigned long count2Show = count2real/teiler;                   // Erstellt Variable für den Wert der angezeigt werden soll, angepasst durch den Teiler
    unsigned long countallShow = count1real + count2real;           // Addition der beiden Kontakte
    uint32_t count1Trans = count1Show;                              // Erstelle Variable für den Transport über den Payload
    uint32_t count2Trans = count2Show;                              // Erstelle Variable für den Transport über den Payload
    uint32_t countallTrans = countallShow;                          // Erstelle Variable für den Transport über den Payload


/**/
/* Schreibe die Werte in das EEPROM des ESP32 */
/**/
  EEPROM.put(eeAddress0esp, count1real);                             // Schreibe den Wert aus "count1real" in den EEPROM
  EEPROM.commit();                                                   // Bestätige das Speichern in den EEPROM
  EEPROM.put(eeAddress1esp, count2real);                             // Schreibe den Wert aus "count2real" in den EEPROM
  EEPROM.commit();                                                   // Bestätige das Speichern in den EEPROM
  EEPROM.put(eeAddressallesp, countallreal);                         // Schreibe den Wert aus "countallreal" in den EEPROM
  EEPROM.commit();                                                   // Bestätige das Speichern in den EEPROM


/**/
/* Zeige die gezählten Besucher an */
/**/
  display.clear();                                                    // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                          // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_24);                                  // Schriftgröße auf 24
  display.drawString(0, 0, "Besucher:");                              // Gibt "Besucher:" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_16);                                  // Schriftgröße auf 16
  display.drawString(0,28, String(countallShow));                     // Gibt die Addition von count1 und count2 an Koordinate 0,28 aus 
  display.setFont(ArialMT_Plain_10);                                  // Schriftgröße auf 10
  display.drawString(0,52, "(" + String(count1Show) + "+" + String(count2Show) + ")");  // Gibt die Einzelwerte der Kontaktauslösungen, in Klammern und mit einem Plus an Koordinate 0,52 aus 
  display.display();                                                  // Befehl um die Bildausgabe zu starten


/**/
/* Integriert die ausgelagerte LoRaWAN-Funktionalität */
/**/
  #include "lorawan.h";                                               // Integriert die ausgelagerte LoRaWAN-Funktionalität


if (DEBUG == 1){Serial.println("DEBUG: loop() abgeschlossen");}
}



/**/
/* Schlafroutine, wenn keine Verbindung aufgebaut werden konnte. */
/**/
void goToSleep() {
  Serial.println("Ich gehe in den Tiefschlaf");                       // Gibt auf der Konsole aus, dass der ESP32 in den Tiefschlaf geht
  if (DEBUG == 1){Serial.println(millis());}



/**/
/* Zeige für fünf Sekunden an, dass es keinen Empfang gab und der Zähler schlafen geht. */
/**/
  display.clear();                                                    // Displayinhalt löschen
  display.setTextAlignment(TEXT_ALIGN_LEFT);                          // Ausrichtung der Schrift ist links
  display.setFont(ArialMT_Plain_16);                                  // Schriftgröße auf 16
  display.drawString(0, 0, "Kein Empfang");                           // Gibt "Kein Empfang" an Koordinate 0,0 aus 
  display.setFont(ArialMT_Plain_10);                                  // Schriftgröße auf 10
  display.drawString(0,28, "Gehe für 120 Sekunden");                  // Gibt "Gehe für 120 Sekunden" an Koordinate 0,28 aus 
  display.setFont(ArialMT_Plain_10);                                  // Schriftgröße auf 10
  display.drawString(0,52, "schlafen und starte neu.");               // Gibt "schlafen und starte neu." an Koordinate 0,52 aus 
  display.display();                                                  // Befehl um die Bildausgabe zu starten
  delay(5000);                                                        // Warte 5 Sekunden, damit man die Ausgabe lesen kann



/**/
/* Routine für den Tiefschlaf */
/**/
  persist.saveSession(node);                                          // Speichert die Session für die Nutzung nach dem Tiefschlaf
  uint32_t interval = node->timeUntilUplink();                        // Berechnet die minimale Arbeitszyklusverzögerung (gemäß FUP und Gesetz!)
  uint32_t delayMs = max(interval, (uint32_t)MINIMUM_DELAY * 1000);   // wähle das INTERVAL oder das MINIMUM_DELAY, je nachdem, was größer ist
  Serial.printf("Next TX in %i s\n", delayMs/1000);                   // Gibt auf der Konsole aus, wann der erneute Verbindungsaufbau stattfindet
  if (DEBUG == 1){Serial.println("DEBUG: ESP32 geht in den Schlafmodus durch goToSleep()");}
  delay(100);                                                         // Warte 0,1 Sekunden, damit die Nachricht vor dem DeepSleep geschrieben werden kann
  heltec_deep_sleep(delayMs/1000);                                    // Gute Nacht John Boy.
}
