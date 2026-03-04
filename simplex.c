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

int main (int  argc, char ** argv )
{
	//Überprüfe Argumente
	if ( 2 != argc ) 
	{
		printf("\nUsage: ./LOA_Template.exe Modell.lp\n");
		return 1;
	}
	
	int anzahlVariablen; //inklusive Schlupfvariablen
	int anzahlOriginalVariablen;
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



	//Speicherfreigabe
	if( ZfwKoeff != NULL ) free( ZfwKoeff );
	if( RechteSeite != NULL ) free( RechteSeite );
	if( RestrMatrix != NULL ) FreeDoubleMat( anzahlRestriktionen + 1, RestrMatrix );

	return 0;
}
