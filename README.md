# Simplex-Algorithmus (C)

Dieses Projekt enthält eine Implementierung der **primalen Simplex-Methode** in der Programmiersprache **C** zur Lösung linearer Optimierungsprobleme.

Das Programm liest ein lineares Optimierungsmodell aus einer `.lp`-Datei ein, verarbeitet die Daten als Matrixstruktur und führt anschließend iterativ Pivot-Operationen durch, bis eine optimale Lösung gefunden wird oder das Problem als unbeschränkt erkannt wird.

## Funktionen

- Implementierung der primalen Simplex-Methode
- Einlesen von linearen Optimierungsmodellen aus `.lp`-Dateien
- Berechnung der Pivotspalte anhand der reduzierten Kosten
- Bestimmung der Pivotzeile mittels Quotiententest
- Iterative Verbesserung der Lösung durch Pivot-Operationen
- Erkennung optimaler Lösungen oder unbeschränkter Probleme
- Ausgabe der optimalen Lösung und des Zielfunktionswerts in eine Ergebnisdatei

## Technologien

- C
- Matrix- und Vektoroperationen
- Datei-Ein- und Ausgabe

## Funktionsweise

1. Das Programm liest ein lineares Optimierungsmodell aus einer `.lp`-Datei ein.
2. Die Daten werden in Matrizen und Vektoren gespeichert.
3. Anschließend wird das Simplex-Tableau aufgebaut.
4. In jeder Iteration wird:
   - die Pivotspalte bestimmt
   - die Pivotzeile über den Quotiententest ausgewählt
   - eine Pivot-Operation durchgeführt
5. Der Algorithmus endet, wenn die optimale Lösung erreicht ist oder das Problem unbeschränkt ist.

## Kompilieren

gcc simplex.c -o simplex


## Programm ausführen

./simplex modell.lp

## Ausgabe

Die optimale Lösung und der Zielfunktionswert werden in eine Datei geschrieben, zum Beispiel: modell.lp_Opt.txt


## Hintergrund

Dieses Projekt wurde im Rahmen einer universitären Programmieraufgabe entwickelt, um ein besseres Verständnis für **lineare Optimierung, Algorithmen und deren Implementierung in C** zu erlangen.
