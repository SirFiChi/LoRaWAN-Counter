# LoRaWAN-Counter
Digitaler Zutrittszähler für den digitalen Zwilling der LHM


Bei diesem Projekt handelt es sich um eine kostengünstige Lösung einfache Daten per LoRaWAN an das TTN zu senden, um diese für andere Dienste, in diesem Fall den digitalen Zwilling der LHM, bereitstellen zu können.

Als Pilotprojekt dient die bereits "analoge" Zählung der Besucher*Innen der Münchner Stadtbibliotheken. Diese werden mit einer Lichtschranke und einem Zählwerk erfasst. Um die Daten nicht ablesen und manuell übermitteln zu müssen, soll eine einfache technische Lösung umgesetzt werden, die den Einsatz des Personals ablöst.

---
## Ablauf
Der geplante Ablauf des Codes:

* Initialisierung der Komponenten
* Abfrage ob Jumper gesetzt sind (für Dividierung z.B. durch zwei, wenn Eingang gleich Ausgan, oder durch vier, wenn die Lichtschranke zu Tief hängt und Beine und nicht Hüften zählt. Ggf. auch einen Verbose Modus für das Display, um z.B. Signalstärke zu messen).
* Zugriff auf Speicher ob bereits ein Wert gespeichert wurde und zu diesem addieren.
* Zählen der Auslösungen an der Lichtschranke.
* Übermitteln der Daten alle X Minuten an das TTN. (Ggf. auch per Jumper anpassbar. Im Fall der MSB ist alle sechs Minuten angedacht.)

---
## Hardware
* Microcontroller: Basis ist ein "ESP32 LoRa V3" mit SX1262 Chip (863-928MHz) als Original der Firma Heltec oder rebranded.
* Gehäuse: Die Firma Mollom bietet absperrbare Gehäuse, die zwar keinen 100%igen Schutz vor Manipulation bieten, aber die Komponenten schützen und Manipulation sichtbar machen.
* Zwischenspeicher: Aktuell unklar pb MicroSD Karte, interner Speicher am ESP32 oder eine RTC (DS3131) mit Speicher.

Für einen Prototypen werden folgende Komponenten getestet:
* https://www.amazon.de/gp/product/B0CKRGVLK4 (ESP32 LoRa V3)
* https://www.amazon.de/gp/product/B07TZJYV1R (MicroSD Modul mit RTC)
* https://www.amazon.de/gp/product/B09XJZ6WGT (Gehäuse)
* https://www.amazon.de/gp/product/B077JYCD18 (Lichtschranke)
* https://www.amazon.de/gp/product/B07BDTMZQ6 (Netzteil 5V/3A)
* https://www.amazon.de/gp/product/B06XWSYRCF (Netzteil 12V/1,25A)
* https://www.amazon.de/gp/product/B0CYLPM7T7 (Kabeldurchführungen)
* https://makerworld.com/de/models/74345#profileId-78795 (Gehäuse für den ESP32 zum selbst Drucken)

---
## Software
* Es wird die Arduino IDE verwendet, um einen einfachen Einstieg zu gewährleisten, um auf mehr Ressourcen zugreifen zu können und um das Projekt möglichst einfach reproduzierbar zu machen.

Tutorials die für dieses Projekt betrachtet wurden
* https://esp32io.com/tutorials/esp32-dip-switch?utm_content=cmp-true (Dip-Switch auswerten)
* https://esp32io.com/tutorials/esp32-button-count-lcd (Button Count)
* https://resource.heltec.cn/download/WiFi_LoRa_32_V3/HTIT-WB32LA(F)_V3.png (PinOut)
* https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/ (Speichern der aktuellen Zählungen im Flash Memory des ESP32)
* https://randomnerdtutorials.com/esp32-data-logging-temperature-to-microsd-card/ (Speichern der aktuellen Zählungen auf MicroSD Karte)
* https://www.youtube.com/watch?v=AslPFY-UXv4&t=473s (Speichern der aktuellen Zählungen auf einem DS3231 RTC Modul)

---
## Sicherheit
* Als Übertragungsmittel wurde LoRaWAN gewählt, damit die Datenübertragung nicht durch die Infrastruktur der LHM läuft. Die Daten werden an das TTN übermittelt, und dort abgegriffen.
* Die bereitgestellten Daten sind in diesem Basis-Projekt maximal "TimeStamp", eindeutige Bezeichnung des Senders und Anzahl der Auslösungen. Es werden also keine personenbezogennen oder sicherheitsrelevanten Daten übermittelt!
* Durch die sehr geringe Relevanz der Daten ist eine unverschlüsselte Datenübertragung zu einem Drittanbieter nicht von Bedeutung.
* Durch die sehr geringe Relevanz der Daten ist eine erfolgreiche Manipulation der Software des Senders nicht von Bedeutung. (Dies ist zudem technisch nicht möglich.)
* Auch wenn der Quellcode über GitHub veröffentlicht wurde, ist eine Manipulation der Daten am Sender nicht möglich, da Zugangsdaten zum TTN (AppEUI, DevEUI, DEVKEY) benötigt werden. Diese müssen erst ausgelesen werden.
* Der Sender selbst wird in einem absperrbaren Gehäuse verbaut. Manipulation am Gehäuse wird wahrgenommen.
