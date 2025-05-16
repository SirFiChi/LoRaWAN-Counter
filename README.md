# LoRaWAN-Counter V1.0
Digitaler Zutrittszähler für die Statistik der Münchner Stadtbibliothek
und für den digitalen Zwilling der LHM


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
* Daten-Speicher:interner Speicher ades ESP32

Für einen Prototypen werden folgende Komponenten getestet:
* https://www.amazon.de/gp/product/B0CKRGVLK4 (ESP32 LoRa V3)
* https://www.amazon.de/gp/product/B09XJZ6WGT (Gehäuse)
* https://www.amazon.de/gp/product/B077JYCD18 (Lichtschranke)
* https://www.amazon.de/gp/product/B07BDTMZQ6 (Netzteil 5V/3A)
* https://www.amazon.de/gp/product/B06XWSYRCF (Netzteil 12V/1,25A)
* https://www.amazon.de/gp/product/B0CYLPM7T7 (Kabeldurchführungen)
* ESP32-Hutschienen-Gehäuse für den ESP32 ist eine Eigenentwicklung

---
## Software
* Es wird die Arduino IDE verwendet, um einen einfachen Einstieg zu gewährleisten, um auf mehr Ressourcen zugreifen zu können und um das Projekt möglichst einfach reproduzierbar zu machen.
* In den Dateien wird auf die Herkunft von Fremdcode hingewiesen.
* Um den Versand alle X Minuten durchzuführen, während die Zählung weiterläuft, wurde das Skript LoRaWAN_TTN.ino von Rop Gonggrijp (ropg) und mein erster Versuch gekreuzt
* Ich bin kein Software-Entwickler. Was nach "Quick'n'Dirty" aussieht ist nur Dirty. Verbesserungsvorschläge werden gerne angenommen.

---
## Sicherheit
* Als Übertragungsmittel wurde LoRaWAN gewählt, damit die Datenübertragung nicht durch die Infrastruktur der LHM läuft. Die Daten werden an das TTN übermittelt, und dort abgegriffen.
* Die übertragenen Daten sind "Referat", "DeviceID", "Teiler" des Messwertes, "Zähler1", "Zähler2" und den "Gesamtzähler" (die Addition der beiden Zähler). Es werden also keine personenbezogennen oder sicherheitsrelevanten Daten übermittelt!
* Durch die sehr geringe Relevanz der Datensicherheit ist eine unverschlüsselte Datenübertragung zu einem Drittanbieter nicht von Bedeutung.
* Auch wenn der Quellcode über GitHub veröffentlicht wurde, ist eine Manipulation der Daten am Sender erst möglich, wenn der Mikrokontroller entwendet wird. Die Zugangsdaten zum TTN (AppEUI, DevEUI, DEVKEY) müssen hierfür ausgelesen werden.
* Der Sender selbst wird in einem absperrbaren Gehäuse verbaut. Manipulation am Gehäuse wird wahrgenommen.
