//Eine schlichte Implementierung der primalen Simplexmethode

//Einbindung von Standard-Bibliotheken
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
//Einbindung eigener, lokaler Bibliotheken
#include "conv.h"
#include "loa.h"

// gcc -Wall -ggdb simplex11.c -o simplex11.exe
// drmemory.exe simplex11.exe Sim-1.lp
 
int main (int  argc, char ** argv )
{
	//Überprüfe Argumente
	if ( 2 != argc ) 
	{
		printf("\nUsage: ./simplex.exe Modell.lp\n");
		return 1;
	}
	
	int anzahlVariablen; //inklusive Schlupfvariablen
	int anzahlOriginalVariablen; // nur x1 und x2
	int anzahlRestriktionen;
	double ** RestrMatrix = NULL;
	double * ZfwKoeff = NULL;
	double * RechteSeite = NULL;
	int minOderMax;
	int ok;


	
	ok = ReadModel( argc, argv, &anzahlVariablen, &anzahlOriginalVariablen, &anzahlRestriktionen, &RestrMatrix, &ZfwKoeff, &RechteSeite, &minOderMax );
	if( ok == 0 ) //Falls Einlesen fehlerhaft
	{
		//Speicherfreigabe
		if( ZfwKoeff != NULL ) free( ZfwKoeff );
		if( RechteSeite != NULL ) free( RechteSeite );
		if( RestrMatrix != NULL ) FreeDoubleMat( anzahlRestriktionen+1, RestrMatrix );
		return 0;
	}
	ShowModel(	anzahlVariablen, anzahlOriginalVariablen, anzahlRestriktionen, RestrMatrix, ZfwKoeff, RechteSeite );

	ShowDoubleVect( 1, anzahlVariablen, ZfwKoeff, "ZfwKoeff" );
	ShowDoubleVect( 1, anzahlRestriktionen, RechteSeite, "RechteSeite" );
	ShowDoubleMat( 1, anzahlRestriktionen, 1, anzahlVariablen, RestrMatrix, "RestrMatrix" );
	
	//HIER IST DIE SIMPLEX-METHODE ZU IMPLEMENTIEREN

	int * Basis = GenerateIntVect (anzahlRestriktionen +1);
	for (int i=1; i <=anzahlRestriktionen ; i++)
	{
		Basis [i]= anzahlOriginalVariablen + i;
	}
	ShowIntVect( 1, anzahlRestriktionen, Basis , "Basis" );
		
	int * NichtBasis = GenerateIntVect (anzahlOriginalVariablen + 1);
	for (int i=1; i <= anzahlOriginalVariablen  ; i++)
	{	
		NichtBasis [i]= i; 
	}
	ShowIntVect( 1, anzahlOriginalVariablen, NichtBasis , "NichtBasis" );
		

	double zfw = 0; 
	double Faktor = 0;
	int pivotspalte; 
	double * ReduzierteKostenVektor = GenerateDoubleVect (anzahlVariablen +1 ); // ReduzierteKosten nur für OriginalVariablen, die anderen sind 0 und daher nicht wichtig, haben keinen Einfluss (Hilfsmittel)
	int pivotzeile;
	double * Quotient = GenerateDoubleVect (anzahlRestriktionen +1); 
	double Optimierungspruefer = 0;  
	FILE *file_Schreiben;
	char filename[100];
	strcpy(filename, argv[1]);
	int Abbruch = 0;

	// hiervor beginnt die Schleife
	do
	{
		zfw = 0;
		Optimierungspruefer = 0; 
		ShowDoubleVect( 1, anzahlVariablen, ZfwKoeff, "ZfwKoeff" );
				
		//zfw berechnen 
		for (int z=1; z <= anzahlRestriktionen ; z++ )
		{
			zfw = ZfwKoeff[ Basis[z] ]*RechteSeite[z] + zfw;
		}
		
		//Reduzierte Kosten berechnen & pivotspalte ermitteln
		double maxReduzierteKosten = -10000000000; 
		for (int s = 1; s <= anzahlVariablen ; s++)
		{
			ReduzierteKostenVektor[s]= ZfwKoeff[s]; //
			for (int z = 1; z <= anzahlRestriktionen ; z++ )
			{
				ReduzierteKostenVektor[s] = ReduzierteKostenVektor[s] - (ZfwKoeff[Basis[z]] * RestrMatrix[z][s]); //
			}
		}
		for (int s = 1; s <= anzahlVariablen; s++) 
			{ 
				if (ReduzierteKostenVektor[s] > maxReduzierteKosten) 
				{
					pivotspalte = s;
					maxReduzierteKosten = ReduzierteKostenVektor[s];
				}
			}
		
		ShowDoubleVect( 1, anzahlVariablen, ReduzierteKostenVektor , "ReduzierteKostenVektor" );
		printf("\nDie maximalen reduzierte Kosten sind = %lf.\n", maxReduzierteKosten);
		
		//Abruchbedingung 1 (Alle Werte in der pivotspalte sind kleiner oder gleich 0, dann unbeschränkt)
		for(int z=1; z<= anzahlRestriktionen; z++)
		{
			if(Optimierungspruefer<=RestrMatrix[z][pivotspalte])
			{
				Optimierungspruefer=RestrMatrix[z][pivotspalte];	
			}
		}
		
		if(Optimierungspruefer<=0)
		{
			printf("\nDer Zielfunktionswert waechst unbeschraenkt, weil die Werte der Pivotspalte alle kleiner gleich 0 sind.");
			
			// Datei schreiben
			sprintf(filename, "%s_Opt.txt", filename);
			file_Schreiben = fopen(filename, "w");

			fprintf(file_Schreiben, "Der Zielfunktionswert waechst unbeschraenkt, weil die Werte der Pivotspalte alle kleiner gleich 0 sind.\n");
			fclose(file_Schreiben);

			printf("\nNachricht wurde in %s_Opt.txt geschrieben.\n", filename);
			Abbruch = 3;
			break;
		}

		// Abruchbedingung 2 (alle reduzierten Kosten kleiner oder gleich 0, dann ist die optimale Lösung gefunden)
		if (maxReduzierteKosten <= 0) // Ist maxReduzierteKosten kleiner gleich 0, sind es automatisch alle reduzierten Kosten
		{
			// Datei schreiben
			sprintf(filename, "%s_Opt.txt", filename);

			file_Schreiben = fopen(filename, "w");

			printf("\nEine eindeutige optimale Loesung wurde gefunden.\n");
			fprintf(file_Schreiben, "Eine eindeutige optimale Loesung wurde gefunden.\n");

			for (int i = 1; i <= anzahlRestriktionen; i++) 
			{
				if (Basis[i] <= anzahlOriginalVariablen) 
				{
					fprintf(file_Schreiben, "x_%d = %lf\n", Basis[i], RechteSeite[i]);
				}
			}
			fprintf(file_Schreiben,"\nDer Zielfunktionswert ist = %lf.", zfw );
			fclose(file_Schreiben); // Datei schließen
			printf("\nDie Loesung wurde in %s geschrieben.\n", filename);
			Abbruch = 2;
			break;
		}
		printf("\nDie pivotspalte ist Spalte %d.", pivotspalte);
	
		// Quotienten und Pivotzeile bestimmen
		double minQuotient = 10000000000; // Einfach eine random sehr hohe zahl, damit der erste berechnete Quotient aufjedenfall kleiner ist
		for (int z = 1; z<= anzahlRestriktionen; z++)
		{
			if(RestrMatrix[z][pivotspalte]<=0)
			{
				Quotient[z]=minQuotient;
			}
			else
			{
				Quotient[z]= RechteSeite[z]/RestrMatrix[z][pivotspalte];
			}
		}
		
		ShowDoubleVect (1, anzahlRestriktionen, Quotient, "QuotientVektor");
		
		for(int z=1; z<= anzahlRestriktionen; z++)
		{
			if (Quotient [z]<minQuotient)
			{
				pivotzeile=z;
				minQuotient = Quotient[z];
			}
		}
		printf ("\n\nDie pivotzeile ist Zeile %d\n", pivotzeile );
		
		//Pivotoperation (pivotzeile/Pivotelement)
		double pivotElement = RestrMatrix[pivotzeile][pivotspalte];
		printf ("\n\nDas PivotElement ist = %lf.", pivotElement);
		
		// Nächste Zeile berechnen 

		for (int z=1; z <= anzahlRestriktionen; z++ )
		{
			if (z != pivotzeile)
			{
				Faktor = RestrMatrix[z][pivotspalte]/pivotElement; // man rechnet alle Werte der pivotspalte außer der pivotzeile geteilt durch das Pivotelement, das ist dann der Faktor
				RechteSeite[z] = RechteSeite[z] - (Faktor* RechteSeite[pivotzeile]);
			
				for (int s=1; s <= anzahlVariablen; s++ )
				{
					RestrMatrix[z][s] = RestrMatrix[z][s] - (Faktor* RestrMatrix[pivotzeile][s]); // mit diesem Faktor 
				}
			}
		}	
		RechteSeite[pivotzeile] = RechteSeite[pivotzeile]/pivotElement;

		for (int s=1; s <= anzahlVariablen; s++ )
		{
			RestrMatrix [pivotzeile][s] = RestrMatrix [pivotzeile][s]/pivotElement; 	
		}
		ShowDoubleVect (1, anzahlRestriktionen, RechteSeite, "Rechte Seite");
		ShowDoubleMat (1, anzahlRestriktionen, 1, anzahlVariablen, RestrMatrix, "neueRestrMatrix");
		
		//Variablentausch
		printf("\nDer Zielfunktionswert ist = %lf.", zfw);
		Basis[pivotzeile]=pivotspalte;
		ShowIntVect( 1, anzahlRestriktionen, Basis , "Neue Basis" );
		
	}
	while (Abbruch < 2);
	
	//Speicherfreigabe
	if( ZfwKoeff != NULL ) free( ZfwKoeff );
	if( RechteSeite != NULL ) free( RechteSeite );
	if( RestrMatrix != NULL ) FreeDoubleMat( anzahlRestriktionen + 1, RestrMatrix );	
	free (ReduzierteKostenVektor);
	free (Quotient);
	free (Basis);
	free (NichtBasis);

	return 0;
}
