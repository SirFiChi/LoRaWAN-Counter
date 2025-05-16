/**********
 *
 * Konfiguration der geladenen Module aus module.h
 *
**********/


/**/
/* KONFIGURATION */
/**/
ezButton button1(47);                                               // Lichtschrankenkontakt 1 auf Pin 47
ezButton button2(48);                                               // Lichtschrankenkontakt 2 auf Pin 48
Preferences preferences;                                            // Initialisiert eine Instanz von Preferences zum Speichern auf dem ESP32 Flash Memory
#define EEPROM_SIZE 16                                              // Setze die Größe des Speichers auf 4 Byte
#define ON LOW                                                      // Definiert ON als LOW, für den Status der Pins
#define OFF HIGH                                                    // Definiert OFF als HIGH, für den Status der Pins


/**/
/* LoRaWAN Einstellungen */
/**/
LoRaWANNode* node;                                                  // Funktion unbekannt. Quelle: https://github.com/ropg/heltec_esp32_lora_v3/blob/main/examples/LoRaWAN_TTN/LoRaWAN_TTN.ino
RTC_DATA_ATTR uint8_t count = 0;                                    // Funktion unbekannt. Quelle: https://github.com/ropg/heltec_esp32_lora_v3/blob/main/examples/LoRaWAN_TTN/LoRaWAN_TTN.ino
