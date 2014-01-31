/*
 *  Fahrstuhl.h
 *  Fahrstuhlsteuerung
 *
 */

#include "systemc.h"


#ifndef FAHRSTUHL_H_
#define FAHRSTUHL_H_


SC_MODULE(Fahrstuhl){

	SC_CTOR(Fahrstuhl){
		SC_THREAD(aktiv);
		//	sensitive << ein_aussteigen.value_changed();
	}
	
	void aktiv(){
		;
	}
	
};

#endif