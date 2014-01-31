#include "systemc.h"
#include "Etagenwahl.h"

#ifndef STEUERUNG_H_
#define STEUERUNG_H_

/*
 *  Steuerung.h
 *  Fahrstuhlsteuerung
 *
 *
 */


SC_MODULE(Steuerung){
	
	sc_in<int> tasten_und_ziel_wahl[3];
	// sc_in<int> fahrstuhl_modus;
	// sc_inout<int[6]> haltestellen;
	// sc_out<int>	ein_aussteigen1;
	
	SC_CTOR(Steuerung){
		SC_THREAD(aktiv);
		sensitive << tasten_und_ziel_wahl[0].value_changed() << tasten_und_ziel_wahl[1].value_changed() << tasten_und_ziel_wahl[2].value_changed();
	}
	
	
	void aktiv()
	{
		int passagier_wahl[3], passagier_ziel[3], alt[3], neu[3], dekode;
		
		while (true) {
			
			// Warte auf Anfrage
			wait();	
			
			for (int i = 0; i<3; i++) {
				alt[i] = neu[i];
				neu[i] = tasten_und_ziel_wahl[i].read();
				if (alt[i] != neu[i]) {
					// Dekodiere Tasten Wahl und Zieletage
					dekode = neu[i] / 5;
					passagier_wahl[i] = dekode;
					passagier_ziel[i] = neu[i] - (dekode*5);
					printf("Steuerung hat eine Anfrage von Passagier %d erhalten: Taste %d,  Etage %d, NachOben? %d, Zieletage %d, Kodiert %d\n",i+1, passagier_wahl[i], CalcStock(passagier_wahl[i]), Richtung_Up(passagier_wahl[i]), passagier_ziel[i], neu[i]);
					
					// int passt = fahrstuhl_modus.read();
					// printf("Die Richtung des Fahrstuhls und der Anfrage stimmen %s ueberein", passt ? "" : "nicht" );
					
					// passt ? -> neue Haltestelle hinzufuegen
					
					// passt nicht Anfrage speichern bis Randetage erreicht
					
				}
			}
					
			
		}
		
		// Etagenauswahl
	}
	
	
};

#endif
