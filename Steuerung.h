#include "systemc.h"
#include "Etagenwahl.h"
#include "Events.h"
#include <algorithm> 
#include <stdlib.h> 

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
	//sc_inout<int> weiter_fahren;
	sc_inout<int> einAussteigen1, einAussteigen2, einAussteigen3;
	sc_inout<int> counter1, counter2, counter3;
	// [0][..] = down haltestellen [1][..] = up haltestellen
	bool foo[2][4];
	sc_event a,b,wait1,wait2,wait3, no;//activateSensor;
	bool doorOpen,ziel,idle;
	int passagier_wahl[3], passagier_ziel[3], alt[3], neu[3], dekode, mode, ein;

	
	SC_CTOR(Steuerung){
		for (int x=0; x < 2; x++){
			for (int y=0; y < 4; y++){
				foo[x][y] = false;
			}
		}
		doorOpen = false;
		ziel = false;
		idle = false;

		SC_THREAD(aktiv);
		sensitive << b;
		SC_THREAD(fahrstuhlPruefen);
		sensitive << fahrstuhl_etage.value_changed() << a;
		SC_THREAD(sensor);
		sensitive << activateSensor;
		SC_THREAD(setIdle);
		sensitive << amEnde;
		SC_THREAD(annehmen1);
		sensitive << tasten_und_ziel_wahl[0].value_changed();
		SC_THREAD(annehmen2);
		sensitive << tasten_und_ziel_wahl[1].value_changed();
		SC_THREAD(annehmen3);
		sensitive << tasten_und_ziel_wahl[2].value_changed();
		SC_METHOD(waiter1);
		sensitive << wait1;
		SC_METHOD(waiter2);
		sensitive << wait2;
		SC_METHOD(waiter3);
		sensitive << wait3;
		//SC_METHOD(notifier);
		//sensitive << no;
		//dont_initialize();
	}

	void setIdle(){
		while (true){
			wait();
			idle = true;
			a.notify();
		}
	}

	void annehmen(int i){
		alt[i] = neu[i];
		neu[i] = tasten_und_ziel_wahl[i].read();
		if (alt[i] != neu[i]) {
			// Dekodiere Tasten Wahl und Zieletage
			dekode = neu[i] / 5;
			passagier_wahl[i] = dekode;
			passagier_ziel[i] = neu[i] - (dekode * 5);
			bool nachOben = Richtung_Up(passagier_wahl[i]);
			cout << "[" << sc_time_stamp() << "] ";
			printf("Steuerung hat eine Anfrage von Passagier %d erhalten: Taste %d,  Etage %d, NachOben? %d, Zieletage %d, Kodiert %d\n", i + 1, passagier_wahl[i], CalcStock(passagier_wahl[i]), nachOben, passagier_ziel[i], neu[i]);
			neu[i] = -1;
			b.notify();
		}
	}

	void annehmen1(){
		while (true){
			wait();
			annehmen(0);
			wait1.notify(150, SC_SEC);
		}
	}
	
	void annehmen2(){
		while (true){
			wait();
			annehmen(1);
			wait2.notify(150, SC_SEC);
		}
	} 

	void annehmen3(){
		while (true){
			wait();
			annehmen(2);
			wait3.notify(150, SC_SEC);
		}
	} 
	
	void aktiv()
	{
		while (true) {
			
			// Warte auf Anfrage
			wait();
			
			
			for (int i = 0; i<3; i++) {
				//alt[i] = neu[i];
				//neu[i] = tasten_und_ziel_wahl[i].read();
				if (neu[i] == -1) {
					// Dekodiere Tasten Wahl und Zieletage
					//dekode = neu[i] / 5;
					//passagier_wahl[i] = dekode;
					//passagier_ziel[i] = neu[i] - (dekode*5);

					bool nachOben = Richtung_Up(passagier_wahl[i]);

					//printf("Steuerung hat eine Anfrage von Passagier %d erhalten: Taste %d,  Etage %d, NachOben? %d, Zieletage %d, Kodiert %d\n", i + 1, passagier_wahl[i], CalcStock(passagier_wahl[i]), nachOben, passagier_ziel[i], neu[i]);


					//mode = calcModus(nachOben, fahrstuhl_etage.read()/10, CalcStock(passagier_wahl[i]));

					/*if (i == 1) wait1.notify();
					if (i == 2) wait2.notify();
					if (i == 3) wait3.notify();*/

					if (!nachOben){
						foo[0][CalcStock(passagier_wahl[i]) - 1] = true;
						foo[0][passagier_ziel[i] - 1] = true;
					}

					if (nachOben){
						foo[1][CalcStock(passagier_wahl[i]) - 1] = true;
						foo[1][passagier_ziel[i] - 1] = true;
					}

					/*if (fahrstuhl_modus == 0){
						a.notify();
						wait();
					} */

					//printf("Array Down: %s %s %s %s \n", foo[0][0] ? "true" : "false", foo[0][1] ? "true" : "false", foo[0][2] ? "true" : "false", foo[0][3] ? "true" : "false");
					//printf("Array Up: %s %s %s %s \n", foo[1][0] ? "true" : "false", foo[1][1] ? "true" : "false", foo[1][2] ? "true" : "false", foo[1][3] ? "true" : "false");

					// int passt = fahrstuhl_modus.read();
					// printf("Die Richtung des Fahrstuhls und der Anfrage stimmen %s ueberein", passt ? "" : "nicht" );
					
					// passt ? -> neue Haltestelle hinzufuegen
					
					// passt nicht Anfrage speichern bis Randetage erreicht
			
					
				}
				// prüfen ob auf Etage 1 tür geöffnet werden muss
				a.notify();
				//fahrstuhl_modus.write(mode);


			}
					
		}
		
		// Etagenauswahl
	}



	void fahrstuhlPruefen(){
		bool open1, open2;
		while (true){
			wait();
			int faEtage = fahrstuhl_etage.read();
			int faModus = fahrstuhl_modus.read();
			if (idle) {
				faModus = 0;
				wait(SC_ZERO_TIME);
			}
			cout << "[" << sc_time_stamp() << "] ";
			if (faEtage % 10 != 0){
				// fahrstuhl in ZwischenEtage
				printf("Fahrstuhl befindet sich in Zwischenetage: %d\n", faEtage);
			}
			else {
				faEtage = faEtage/10;
				printf("Fahrstuhl befindet sich auf Etage: %d\n", (faEtage));
				// prüfen ob tür geöffnet werden soll
				if (faModus == 0){
					//modus noch nich gesetzt
					mode = calcModus(faEtage);
					printf("Fahrstuhl ist im Modus: %d \n", mode);
					fahrstuhl_modus.write(mode);
					wait(SC_ZERO_TIME);
					idle = false;
					//prüfe aktuelle Etage
					open1 = foo[0][faEtage-1];
					open2 = foo[1][faEtage-1];
					
					if (open1 && mode == 1) foo[0][faEtage - 1] = false; 
					if (open2 && mode == 2) foo[1][faEtage - 1] = false;
					if (open1 || open2) openDoor();
				}
				else{
					if (faModus == 1){
						open1 = foo[0][((faEtage)-1)];
						//printf("%s\n", open? "true" : "false");
						if (open1 && findPassSameDirection(faEtage,faModus)){
							foo[0][((faEtage)-1)] = false;
							openDoor();
						}
					}
					if (faModus == 2 ){
						open1 = foo[1][((faEtage)-1)];
						//printf("%s\n", open ? "true" : "false");
						if (open1 && findPassSameDirection(faEtage, faModus)){
							foo[1][((faEtage)-1)] = false;
							openDoor();
						}
					}
				}
			}

			if (doorOpen){
				wait(doorClosed);
			}

			if (!leer()) weiterFahren.notify();
			else{
				fahrstuhl_modus.write(0);
			/*	printf("Array Down: %s %s %s %s \n", foo[0][0] ? "true" : "false", foo[0][1] ? "true" : "false", foo[0][2] ? "true" : "false", foo[0][3] ? "true" : "false");
				printf("Array Up: %s %s %s %s \n", foo[1][0] ? "true" : "false", foo[1][1] ? "true" : "false", foo[1][2] ? "true" : "false", foo[1][3] ? "true" : "false");

				printf("Passagier 1 Stock == faEtage: %s", (CalcStock(passagier_wahl[0]) == faEtage) ? "true" : "false");
				printf(" && einst1: %d \n", einAussteigen1.read());

				printf("Passagier 2 Stock == faEtage: %s", (CalcStock(passagier_wahl[1]) == faEtage) ? "true" : "false");
				printf(" && einst2: %d \n", einAussteigen2.read());

				printf("Passagier 3 Stock == faEtage: %s", (CalcStock(passagier_wahl[2]) == faEtage) ? "true" : "false");
				printf(" && einst3: %d \n", einAussteigen3.read());*/

				printf("STOPP! Fahrstuhl ist im Modus: 0 \n");
			}
			/*w = weiter_fahren;
			weiter_fahren.write(++w);*/
		}
	}

	void openDoor(){
		doorOpen = true;
		cout << "[" << sc_time_stamp() << "] ";
		printf("Oeffne Tuer! ");
		wait(2,SC_SEC);
		cout << "[" << sc_time_stamp() << "] \n";
		// warte dass tür geschlossen werden kann
		activateSensor.notify();
		//sc_time a = sc_time(2, SC_SEC);
		wait(deactivateSensor);
		cout << "[" << sc_time_stamp() << "] ";
		printf("Schliesse Tuer! ");
		wait(2,SC_SEC);
		cout << "[" << sc_time_stamp() << "] \n";
		doorOpen = false;
		doorClosed.notify();
	}


	void sensor(){
		int faEtage, faModus;
		while (true){
			wait();
			//printf("Array Up: %s %s %s %s \n", foo[1][0] ? "true" : "false", foo[1][1] ? "true" : "false", foo[1][2] ? "true" : "false", foo[1][3] ? "true" : "false");
			//printf("Array Down: %s %s %s %s \n", foo[0][0] ? "true" : "false", foo[0][1] ? "true" : "false", foo[0][2] ? "true" : "false", foo[0][3] ? "true" : "false");
			if (doorOpen){
				faEtage = fahrstuhl_etage.read() / 10;
				faModus = fahrstuhl_modus.read();
				
				/*printf("Passagier 1 Stock == faEtage: %s", (CalcStock(passagier_wahl[0]) == faEtage) ? "true" : "false");
				printf(" && einst1 == 0: %s \n", einAussteigen1 == 0 ? "true" : "false");

				printf("Passagier 2 Stock == faEtage: %s", (CalcStock(passagier_wahl[1]) == faEtage) ? "true" : "false");
				printf(" && einst2 == 0: %s \n", einAussteigen2 == 0 ? "true" : "false");

				printf("Passagier 3 Stock == faEtage: %s", (CalcStock(passagier_wahl[2]) == faEtage) ? "true" : "false");
				printf(" && einst3 == 0: %s \n", einAussteigen3 == 0 ? "true" : "false");*/
				
				if ((CalcStock(passagier_wahl[0])) == faEtage && einAussteigen1 == 0 && einsteigen(Richtung_Up(passagier_wahl[0]),faModus)){
						einAussteigen1.write(1);
						//cout << "bla \n";
				}
				else if ((CalcStock(passagier_wahl[1])) == faEtage && einAussteigen2 == 0 && einsteigen(Richtung_Up(passagier_wahl[1]),faModus)){
						einAussteigen2.write(1);
						//cout << "bla \n";
				}
				else if ((CalcStock(passagier_wahl[2])) == faEtage && einAussteigen3 == 0 && einsteigen(Richtung_Up(passagier_wahl[2]),faModus)){
						einAussteigen3.write(1);
						//cout << "bla \n";
				}
				else if ((passagier_ziel[0] == faEtage) && einAussteigen1 == 1){
					einAussteigen1.write(-1);
					//wait1.notify(60,SC_SEC);

				}
				else if ((passagier_ziel[1] == faEtage) && einAussteigen2 == 1){
					einAussteigen2.write(-1);
					//wait2.notify(60,SC_SEC);
				}
				else if ((passagier_ziel[2] == faEtage) && einAussteigen3 == 1){
					einAussteigen3.write(-1);
					//wait3.notify(60,SC_SEC);
				}
				else {
					wait(3, SC_SEC);
					deactivateSensor.notify();
				}
				wait(SC_ZERO_TIME);
			}
		}

	}

	bool einsteigen(int nachOben, int faModus){
		if (faModus - 1 == nachOben) return true;
		else return false;
	}



	int calcModus(int faEtage){
		if (faEtage == 1) return 2;
		if (faEtage == 4) return 1;

		int m = 3;
		// find min distance
		for (int i = 0; i < 3; i++){
			m = min(m, abs((CalcStock(passagier_wahl[i])) - faEtage));
		}
		// TODO find most persons waiting direction 
		
		// select one direction
		
		if (foo[0][(faEtage-1) - m]) return 1;
		if (foo[1][(faEtage - 1) + m]) return 2;
	}
	
	bool findPassSameDirection(int faEtage, int faModus){
		bool same = false;
		// fall: passagier wartet (Prüfe: gleiche Richtung?)
		for (int i = 0; i < 3; i++){
			if ((CalcStock(passagier_wahl[i]) == faEtage) && (same == false)){
				 same = (Richtung_Up(passagier_wahl[i]) + 1) == faModus;
			}
		}

		// fall: passagier muss hier aussteigen!
		if (!same){
			same = foo[faModus - 1][faEtage - 1];
			ziel = true;
		}
		return same;
	}

	bool leer(){
		//alles fertig haltestellen leer 
		bool r = true;
		for (int x = 0; x < 2; x++){
			for (int y = 0; y < 4; y++){
				if (foo[x][y]) r = false;
			}
		}
		return r;
	}

	void waiter1(){
		int count1;
		count1 = counter1.read();
		counter1.write(++count1);
		
	}

	void waiter2(){
		int count2;
		count2 = counter2.read();
		counter2.write(++count2);
	}

	void waiter3(){
		int count3;
		count3 = counter3.read();
		counter3.write(++count3);
	}
	
};

#endif
