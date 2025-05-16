int16_t state = radio.begin();                                      // Funktion unbekannt. Quelle: https://github.com/ropg/heltec_esp32_lora_v3/blob/main/examples/LoRaWAN_TTN/LoRaWAN_TTN.ino


/**/
/* Dieser Bereich (Initialisierung und Versand) wird nur einmal ausgeführt! */
/**/
if (initLoRaWANonce == false)                                       // Beim ersten Durchlauf wird die Initialisierung ausgefürt
{  
  initLoRaWANonce = true;                                           // Variable wird auf true gesetzt, damit die Initialisierung nicht nochmal ausgeführt wird

/**/
/* Initialisiere Funk-Modul */
/**/
  Serial.println("Initialisiere Funk-Modul");

  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("Funk-Modul konnte nicht initialisiert werden. Probiere es in Kürze nochmal.");
    goToSleep();                                                    // Gehe schlafen
  }

  node = persist.manage(&radio);                                    // Funktion unbekannt. Quelle: https://github.com/ropg/heltec_esp32_lora_v3/blob/main/examples/LoRaWAN_TTN/LoRaWAN_TTN.ino

  if (!node->isActivated()) {                                       // Gehe schlafen
    Serial.println("Verbindung zum TTN nicht möglich. Probiere es in Kürze nochmal.");
    goToSleep();                                                    // Gehe schlafen
  }

  if (DEBUG == 1){Serial.println("Verbindung zum TTN wurde aufgebaut. Das Senden ist möglich.");}

  node->setDutyCycle(true, 120);                                    // Setzt die Upload-Intervale für die Richtlinien des TTN


/**/
/* Payload wird generiert. */
/**/
#include "payload.h"                                                // Hier gibt es nichts zu konfigurieren, außer es werden andere "Sensoren" verwendet.

/**/
/* Ein Downlink mit 256 Byte wäre möglich, ist aber nicht weiter implementiert. */
/**/
  uint8_t downlinkData[256];                                      // Erstelle Array mit 256 Bytes für den Downlink
  size_t lenDown = sizeof(downlinkData);                          // Größe des Downlinks für den Transfer

  state = node->sendReceive(appData, sizeof(appData), 1, downlinkData, &lenDown);     // Sendet den Payload und empfängt den Downlink


/**/
/* Statusmeldung des Transfers ausgeben */
/**/
  if(state == RADIOLIB_ERR_NONE) {                                // Fragt ab ob Statusmeldung vorhanden ist
    Serial.println("Payload gesendet, kein Downlink empfangen.");
  } else if (state > 0) {
    Serial.println("Payload gesendet, Downlink empfangen.");
  } else {
    Serial.printf("Senden/Empfangen gab Fehler %d aus, wir probieren es später nochmal.\n", state);
  }


}



/**/
/* Dieser Bereich (Versand) wird erst beim zweiten Durchlauf ausgeführt! */
/**/
if (runLoRaWAN == true)                                             // Die Variable wird nach dem ersten Durchlauf auf Wahr gesetzt.
{  
  unsigned long currentMillis = millis();                           // Für die Zeitsteuerung zum Versand des PayLoads
  if (DEBUG == 1){Serial.println(currentMillis);}
  if (DEBUG == 1){Serial.println(previousMillis);}
  if (DEBUG == 1){Serial.println(sendinterval);}

/**/
/* Zeitschleife für den Versand */
/**/
  if ((currentMillis - previousMillis >= sendinterval) || (currentMillis < 20000)) {      // Ausführen wenn sendinterval erreicht ODER wenn aktulle Systemzeit kleiner als 20 Sekunden
    if (currentMillis - previousMillis >= sendinterval) {                                 // Ausführen wenn sendinterval erreicht
      previousMillis = currentMillis;                                                     // Setze aktuelle Systemzeit als letzte Systemzeit für die neue Schleife
    }

/**/
/* Baue den Payload auf */
/**/
    #include "payload.h"                                             // Hier gibt es nichts zu konfigurieren, außer es werden andere "Sensoren" verwendet.

/**/
/* Initialisierung für optionalen Donwlink */
/**/
    uint8_t downlinkData[256];                                      // Erstelle Array mit 256 Bytes für den Downlink
    size_t lenDown = sizeof(downlinkData);                          // Größe des Downlinks für den Transfer

    state = node->sendReceive(appData, sizeof(appData), 1, downlinkData, &lenDown);

/**/
/* Statusmeldung des Transfers ausgeben */
/**/
    if(state == RADIOLIB_ERR_NONE) {                                // Fragt ab ob Statusmeldung vorhanden ist
      Serial.println("Payload gesendet, kein Downlink empfangen.");
    } else if (state > 0) {
      Serial.println("Payload gesendet, Downlink empfangen.");
    } else {
      Serial.printf("Senden/Empfangen gab Fehler %d aus, wir probieren es später nochmal.\n", state);
    }
  }
}
runLoRaWAN = true;                                                  // Setzt die Variable "runLoRaWAN" auf Wahr, damit der Versand beim nächsten Durchlauf gestartet wird
