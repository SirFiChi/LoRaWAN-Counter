  /**********
 *
 * Einstellungen für die Identfikation und die Authentifizierung beim TTN.
 * (ReferatID, DeviceID, OTAA Parameter)
 *
**********/


/**/
/* ReferatID */
/**/
// LISTE DER REFERATE UND EIGENBETRIEBE:
//
// 00 - Direktorium
// 01 - Referat für Arbeit und Wirtschaft
// 02 -Baureferat
// 03 - Gesundheitsreferat
// 04 - Referat für Klima- und Umweltschutz
// 05 - Kommunalreferat
// 06 - Kreisverwaltungsreferat
// 07 - Kulturreferat
// 08 - Personal- und Organisationsreferat
// 09 - Referat für Bildung und Sport
// 10 - Sozialreferat
// 11 - Referat für Stadtplanung und Bauordnung
// 12 - Stadtkämmerei
// 13 - Referat für Informations- und Telekommunikationstechnik
// 14 - Mobilitätsreferat
// 15 - Abfallwirtschaftsbetrieb München
// 16 - Stadtwerke München
// 17 - Markthallen München
// 18 - Gasteig
// 19 - München Klinik
// 20 - München Stift
// 21 - München Ticket
// 22 - Messe München
// 23 - Flughafen München
// 24 - Mediaschool Bayern
// 25 - Münchner Arbeit
// 26 - Münchner Gewerbehof- und Technologiezentrumsgesellschaft
// 27 - Tierpark Hellabrunn
// 28 - Munich Urban Colab
// 29 - Olympiapark
// 30 - Werk1
// 31 - Münchner Stadtentwässerung
// 32 - Münchner Kammerspiele
// 33 - Deutsches Theater München
// 34 - Münchner Volkshochschule
// 35 - Internationale Münchner Filmwochen
// 36 - Münchner Wohnen
// 37 - muenchen.de
// 38 - it@M
// 39 - MRG Münchner Raumentwicklungsgesellschaft
//
byte referatID = 7;                                               // Setzt das Referat


/**/
/* DeviceID - Wird individuell vergeben und identifiziert das Gerät im Referat*/
/**/
int deviceID = 2;                                                 // Setzt die Device-ID


 /**/
 /* Setzt das Sendeinterval */
 /**/
const long funkInterval = 6;                                      // Sendeinterval in Minuten. Default: 6 Minuten --- !!! Abweichungen möglich, da keine RTC verwendet wird !!!
