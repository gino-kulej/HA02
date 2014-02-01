/*
 *  Fahrstuhl.h
 *  Fahrstuhlsteuerung
 *
 */

#include "systemc.h"
#include "Steuerung.h"
#include "Events.h"


#ifndef FAHRSTUHL_H_
#define FAHRSTUHL_H_


SC_MODULE(Fahrstuhl){
	// 10..15..20..25..30..35..40
	sc_inout<int> fahrstuhl_etage;
	// idle = 0, down = 1, top = 2
	sc_in<int> fahrstuhl_modus;
	//sc_in<int> weiter_fahren;

	SC_CTOR(Fahrstuhl){
		fahrstuhl_etage.initialize(10);
		//SC_THREAD(aktiv);
		//	sensitive << ein_aussteigen.value_changed();
		SC_THREAD(fahren);
		sensitive << weiterFahren;
	}
	
	//void aktiv(){
	//	while (true){
	//		wait();
	//	}
	//}

	void fahren(){
		while (true){
			
			int mode = fahrstuhl_modus.read();
			int e = fahrstuhl_etage.read();
			if (mode == 1){
				// down
				//printf("Modus: down , TICK down!\n");
				if (e > 10) tickDown();
				else amEnde.notify();
			}
			if (mode == 2){
				// up
				//printf("Modus: up, TICK up! \n");
				if (e < 40) tickUp();
				else amEnde.notify();
			}
			wait();
		}
	}
	
	void tickUp(){
		int e = fahrstuhl_etage.read();
		e += 5;
		wait(1, SC_SEC);
		fahrstuhl_etage.write(e);
	}

	void tickDown(){
		int e = fahrstuhl_etage.read();
		e -= 5;
		wait(1, SC_SEC);
		fahrstuhl_etage.write(e);
	}

};

#endif