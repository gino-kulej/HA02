/*
 *  StrukturAnfrage.h
 *  Fahrstuhlsteuerung
 *
 */

#include "systemc.h"
#include "Etagenwahl.h"

#ifndef STRUKTURANFRAGE_H_
#define STRUKTURANFRAGE_H_


typedef struct Anfrage {
	int tasten_wahl; 
	int ziel_etage;
}
#endif