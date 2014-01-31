/*
 *  Passagier.h
 *  Fahrstuhlsteuerung
 *
 *
 */
#include "systemc.h"
#include <stdlib.h> 
#include "Etagenwahl.h"
#include <time.h>


#ifndef PASSAGIER_H_
#define PASSAGIER_H_



SC_MODULE(Passagier){
	
	sc_out<int> tasten_und_ziel_wahl;
	int start_taste, ziel;

	// sc_in<int>	ein_aussteigen;
		
	SC_CTOR(Passagier){
		SC_THREAD(aktiv);
	// sensitive << ein_aussteigen.value_changed();
	}
	
	void aktiv()
	{
		int both;
		while (true) {
			
			// Etagenauswahl & Ziel wahl
			randomStartTaste();
			// Kodieren in eine Zahl
			both = start_taste*5+ziel;
			// -> an Steuerung übermitteln
			tasten_und_ziel_wahl.write(both);						
			// Warte auf Fahrstuhl
			wait();
		}
	}
	
	void randomStartTaste() {
		
		// Folgender Tastekodierung: top1 = 0, top2 = 1, down2 = 2, top3 = 3, down3 = 4, down4 = 5.
		start_taste = rand() % 6;
		randomZielEtage(start_taste);
		
	}
	
	void randomZielEtage(int start) {
		int etage = CalcStock(start);
		if (Richtung_Up(start)) {
			ziel = rand() % (4-etage) + (etage+1);
		} 
		else {
			ziel = rand() % (etage-1) + 1; 
		}
		
	}
	
	
};

#endif
