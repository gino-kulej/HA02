/*
 *  Etagenwahl.h
 *  Fahrstuhlsteuerung
 *
 */
#include "systemc.h"


#ifndef ETAGENWAHL_H_
#define ETAGENWAHL_H_



bool Richtung_Up(int taste){
	if (taste == 0 || taste == 1 || taste == 3) return true;
	else return false; 
}

int CalcStock(int taste) {
	int r; 
	if (taste == 0) r = 1; 	
	else if (taste == 1 || taste == 2) r = 2;
	else if (taste == 3 || taste == 4) r = 3 ; 
	else if (taste == 5) r = 4;
	else r = -1;
	return r;
}

#endif