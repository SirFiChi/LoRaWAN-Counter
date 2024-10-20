/**
 * Source:
 * - https://esp32io.com/tutorials/esp32-dip-switch
 **/

#define ON LOW                                         // Definiert ON als LOW, für den Status der Pins
#define OFF HIGH                                       // Definiert OFF als HIGH, für den Status der Pins

void setup() {
  Serial.begin(115200);                                // Initialisiert den seriellen Monitor
  pinMode(20, INPUT_PULLUP);                           // Definiert Port 20 als Pullup Input
  pinMode(26, INPUT_PULLUP);                           // Definiert Port 26 als Pullup Input
}

void loop() {
    Serial.print("Pin 20: ");                          // Gibt Text "Pin 20: " am seriellen Port aus
    int state0 = digitalRead(20);                      // Liest Status von Port 20
    if (state0 == ON)                                  // Wenn Status von Port 20 ist "ON"...
      Serial.println("ON ");                           // ...gib "ON" aus
    else                                               // Ansonsten
      Serial.println("OFF ");                          // ...gib "OFF" aus
  
    Serial.print("Pin 26: ");                          // Gibt Text "Pin 26: " am seriellen Port aus
    int state1 = digitalRead(26);                      // Liest Status von Port 20
    if (state1 == ON)                                  // Wenn Status von Port 20 ist "ON"...
      Serial.println("ON ");                           // ...gib "ON" aus
    else                                               // Ansonsten
      Serial.println("OFF ");                          // ...gib "OFF" aus
  
  Serial.println();                                    // Gib eine leere Zeile auf der Konsole aus
  delay(2000);                                         // Warte zwei Sekunden
}
