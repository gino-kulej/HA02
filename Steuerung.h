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
	// jedes mal bei änderung prüfen
	sc_in<int> fahrstuhl_etage;
	// idle = 0, down = 1, top = 2
	sc_inout<int> fahrstuhl_modus;
	sc_inout<int> weiter_fahren;
	// sc_inout<int[6]> haltestellen;
	// sc_out<int>	ein_aussteigen1;
	// [0][..] = down haltestellen [1][..] = up haltestellen
	bool foo[2][4];
	//sc_event a,b;

	
	SC_CTOR(Steuerung){
		for (int x=0; x < 2; x++){
			for (int y=0; y < 4; y++){
				foo[x][y] = false;
			}
		}

		SC_THREAD(aktiv);
		sensitive << tasten_und_ziel_wahl[0].value_changed() << tasten_und_ziel_wahl[1].value_changed() << tasten_und_ziel_wahl[2].value_changed(); //<< b;
		SC_THREAD(fahrstuhlPruefen);
		sensitive << fahrstuhl_etage.value_changed(); //<< a;
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

					bool nachOben = Richtung_Up(passagier_wahl[i]);

					printf("Steuerung hat eine Anfrage von Passagier %d erhalten: Taste %d,  Etage %d, NachOben? %d, Zieletage %d, Kodiert %d\n", i + 1, passagier_wahl[i], CalcStock(passagier_wahl[i]), nachOben, passagier_ziel[i], neu[i]);


					int mode = calcModus(nachOben, fahrstuhl_etage.read()/10, CalcStock(passagier_wahl[i]));

					

					if (mode == 1){
						foo[0][CalcStock(passagier_wahl[i]) - 1] = true;
						foo[0][passagier_ziel[i] - 1] = true;
					}

					if (mode == 2){
						foo[1][CalcStock(passagier_wahl[i]) - 1] = true;
						foo[1][passagier_ziel[i] - 1] = true;
					}

					/*if (fahrstuhl_modus == 0){
						a.notify();
						wait();
					} */

					fahrstuhl_modus.write(mode);

					//printf("Array Down: %s %s %s %s \n", foo[0][0] ? "true" : "false", foo[0][1] ? "true" : "false", foo[0][2] ? "true" : "false", foo[0][3] ? "true" : "false");
					//printf("Array Up: %s %s %s %s \n", foo[1][0] ? "true" : "false", foo[1][1] ? "true" : "false", foo[1][2] ? "true" : "false", foo[1][3] ? "true" : "false");

					// int passt = fahrstuhl_modus.read();
					// printf("Die Richtung des Fahrstuhls und der Anfrage stimmen %s ueberein", passt ? "" : "nicht" );
					
					// passt ? -> neue Haltestelle hinzufuegen
					
					// passt nicht Anfrage speichern bis Randetage erreicht
			
					
				}
			}
					
		}
		
		// Etagenauswahl
	}


	void fahrstuhlPruefen(){
		while (true){

			int faEtage = fahrstuhl_etage.read();
			int faModus = fahrstuhl_modus.read();
			if (faEtage % 10 != 0){
				// fahrstuhl in ZwischenEtage
				printf("Fahrstuhl befindet sich in Zwischenetage: %d\n", faEtage);
			}
			else {
				printf("Fahrstuhl befindet sich auf Etage: %d\n", (faEtage / 10));
				// prüfen ob tür geöffnet werden soll
				// warte auf modus berechnung
				/*if (faModus == 0){
					wait();
					bool open = foo[0][((faEtage / 10) - 1)];
					printf("%s\n", open? "true" : "false");
					if (open){
						printf("offne tur!\n");
					}
					b.notify();
				}*/
				if (faModus == 1){
					bool open = foo[0][((faEtage / 10) - 1)];
						//printf("%s\n", open? "true" : "false");
					if (open){
						printf("offne tur!\n");
					}
				}
				if (faModus == 2){
					bool open = foo[1][((faEtage / 10) - 1)];
					//printf("%s\n", open ? "true" : "false");
					if (open){
						printf("offne tur!\n");
					}
				}
			}
			int w = weiter_fahren;
			weiter_fahren.write(++w);
			wait();
		}
	}

	//bla
	int calcModus(bool nachOben, int fahrstuhlEtage, int passagierEtage){
		int mode = fahrstuhl_modus.read();
		int r = mode;
		if (nachOben){
			if (passagierEtage >= fahrstuhlEtage){
				r = 2; // top, alles gut 
			}
			else {
				 // down (bis zur passagierEtage) danach für diese anfrage wieder auf top
					// down bis etage 0 
				 // warten auf amEndeAngekommen
				r = 1;
			}
		}
		else {
			if (passagierEtage >= fahrstuhlEtage){
				// top (bis zur passagierEtage) danach für diese anfrage wieder auf down
				// top bis etage 3 
				r = 2;
			}
			else{
				r = 1; // down, alles gut 
			}
		}
		return r;
	}
	
	
};

#endif
