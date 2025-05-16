/**********
 *
 * Erstellen des zu versendenden Datenpakets (Sammlung und Konvertierung)
 *
**********/

/**/
/* Funktion um den Payload zu erstellen */
/**/
  uint8_t appData[16];                                              // Erstellt Array mit 16 Bytes für die Aufnahme der Daten im Payload
    appData[0] = referatID;                                         // Array 0 - Speichert die Information über das Referat
    appData[1] = deviceID >> 8;                                     // Array 1 - Der hohe Teil der DeviceID
    appData[2] = deviceID;                                          // Array 2 - Der niedrige Teil der DeviceID
    appData[3] = teiler;                                            // Array 3 - Der definierte Teiler
    appData[4] = count1Show >> 24;                                  // Array 4 - Höchster Wert von Zähler 1
    appData[5] = count1Show >> 16;                                  // Array 5 - Hoher Wert von Zähler 1
    appData[6] = count1Show >> 8;                                   // Array 6 - Niedrier Wert von Zähler 1
    appData[7] = count1Show;                                        // Array 7 - Niedrigster Wert von Zähler 1
    appData[8] = count2Show >> 24;                                  // Array 8 - Höchster Wert von Zähler 2
    appData[9] = count2Show >> 16;                                  // Array 9 - Hoher Wert von Zähler 2
    appData[10] = count2Show >> 8;                                  // Array 10 - Niedriger Wert von Zähler 2
    appData[11] = count2Show;                                       // Array 11 - Niedrigster Wert von Zähler 2
    appData[12] = countallShow >> 24;                               // Array 12 - Höchster Wert vom Gesamt-Zähler
    appData[13] = countallShow >> 16;                               // Array 13 - Hoher Wert vom Gesamt-Zähler
    appData[14] = countallShow >> 8;                                // Array 14 - Niedrier Wert von Gesamt-Zähler
    appData[15] = countallShow;                                     // Array 15 - Niedrigster Wert von Gesamt-Zähler








/**********
 *
 * Das nachfolgende JavaScript wird als Upload-Decoder im TTN eingetragen. Dadurch wird
 * der das oben generierte Datenpaket (Payload) in die einzelnen Werte zurück gerechnet.
 *
**********/
/**/
/*

function decodeUplink(input) {
  var referatID = (input.bytes[0]);
  var deviceID = (input.bytes[1] << 8) + input.bytes[2];
  var teiler = (input.bytes[3]);
  var zaehler1 = (input.bytes[4] << 24) + (input.bytes[5] << 16) + (input.bytes[6] << 8) + input.bytes[7];
  var zaehler2 = (input.bytes[8] << 24) + (input.bytes[9] << 16) + (input.bytes[10] << 8) + input.bytes[11];
  var zaehlerg = (input.bytes[12] << 24) + (input.bytes[13] << 16) + (input.bytes[14] << 8) + input.bytes[15];

  return {
    data: {
      Referat: referatID,
      Device: deviceID,
      Faktor: teiler,
      ZaehlerEins: zaehler1,
      ZaehlerZwei: zaehler2,
      ZaehlerGesamt: zaehlerg
    }
  };
}

*/
/**/
