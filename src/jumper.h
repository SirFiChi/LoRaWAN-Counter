/**********
 *
 * Definiert die Variable "teiler" nach der dem Zustand der Jumperbrücken.
 *
**********/


/**/
/* Es wird ein Teiler nach Konfiguration der Jumper definiert. */
/* Das Ergebnis dividieren, abhängig von... */
/* 1. Ein- und Ausgang sind seperat und Zählung ist real (1:1) */
/* 2.1. Eingang ist Ausgang und Ergebnis muss halbiert werden (1/2) */
/* 2.2. Ein- und Ausgang sind seperat aber Lichtschranke auf Höhe der Füße (1/2) */
/* 3. Eingang ist Ausgang und Lichtschranke auf Höhe der Füße (1/4) */
/**/
  if ((jumper0 == 1) && (jumper1 == 1)){                            // Wenn kein Jumper gesteckt
    teiler = 1;                                                     // Teile Zählung durch 1 (1:1)
  } else if ((jumper0 == 0) && (jumper1 == 1)){                     // Wenn Jumper 1 gesteckt
    teiler = 2;                                                     // Teile Zählung durch 2 (1/2)
  } else if ((jumper0 == 1) && (jumper1 == 0)){                     // Wenn Jumper 2 gesteckt
    teiler = 4;                                                     // Setze Teiler auf 4 (1/4)
  }
