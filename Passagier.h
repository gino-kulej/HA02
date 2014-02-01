/*
 *  Passagier.h
 *  Fahrstuhlsteuerung
 *
 *
 */
#include "systemc.h"
#include <stdlib.h> 
#include "Etagenwahl.h"
#include "Events.h"
//#include <time.h>
#include <iostream>
#include <random>
#include <cstdlib>

#ifndef PASSAGIER_H_
#define PASSAGIER_H_



SC_MODULE(Passagier){
	
	sc_out<int> tasten_und_ziel_wahl;
	int start_taste, ziel, rdEinsteigen;
	sc_in<int> einAussteigen;
		
	SC_CTOR(Passagier){
		SC_THREAD(aktiv);
		SC_THREAD(step);
		sensitive << einAussteigen.value_changed();
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
			//printf("P %d \n",both);
			tasten_und_ziel_wahl.write(both);						
			// Warte auf Fahrstuhl
			wait();
		}
	}
	
	void randomStartTaste() {
		//srand(GetTickCount());
		std::random_device rd;
		std::srand(rd());
		// Folgender Tastekodierung: top1 = 0, top2 = 1, down2 = 2, top3 = 3, down3 = 4, down4 = 5.
		start_taste = std::rand() % 6;
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

	void step(){
		while (true){
			wait();
			if (einAussteigen.read() == 1){
				//std::random_device rd;
				//std::srand(rd());
				// step in (innerhalb von 1-2 sek)
				//rdEinsteigen = 2 + std::rand() / RAND_MAX;
				wait(2,SC_SEC);
				cout << "[" << sc_time_stamp() << "] " << "step in (" << name()<< ")" << endl;

				activateSensor.notify();
			}

		}

	}
	
	
};

#endif
