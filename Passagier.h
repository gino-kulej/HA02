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
	
	sc_inout<int> tasten_und_ziel_wahl;
	int start_taste, ziel, rdEinsteigen;
	sc_in<int> einAussteigen;
	sc_in<int> fahrstuhlModus;
	sc_in<int> counter;
	bool in;
	sc_event neu;
		
	SC_CTOR(Passagier){
		in = false;

		SC_THREAD(aktiv);
		sensitive << neu;//counter.value_changed(); 
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
		int faModus;
		int nachOben;
		while (true){
			wait();
			wait(SC_ZERO_TIME);
				faModus = fahrstuhlModus.read();
				//printf("Fahrstuhl ist im Modus: %d \n", faModus);
				nachOben = Richtung_Up(start_taste);

				bool einst = einsteigen(nachOben, faModus);
				//std::random_device rd;
				//std::srand(rd());
				// step in (innerhalb von 1-2 sek)
				//rdEinsteigen = 2 + std::rand() / RAND_MAX
				wait(2,SC_SEC);
				/*printf("in: %s\n", in ? "true" : "false");
				printf("einst: %s\n", einst ? "true" : "false");*/
				/*if (einAussteigen == -1 && !){
				}
				else*/ if (einst && !in){
					cout << "[" << sc_time_stamp() << "] " << "step in (" << name() << ")" << endl;
					in = true;
				} 
				else if(in){
					cout << "[" << sc_time_stamp() << "] " << "step out (" << name() << ")" << endl;
					in = false;
					neu.notify(60, SC_SEC);
				}
				activateSensor.notify();

		}

	}

	bool einsteigen(int nachOben,int faModus){
		if (faModus - 1 == nachOben ) return true;
		else return false;
	}
	
	
};

#endif
