#include "systemc.h"
#include "Passagier.h"
#include "Steuerung.h"
#include "Fahrstuhl.h"
//#include <time.h>

//#include <iostream>

int sc_main(int argc, char * argv[]) {

	//srand(GetTickCount());
	//srand(time(NULL));
	/*std::random_device rd;
	std::srand(rd());*/
	//sc_event activateSensor;

	// generating the sc_signal
	sc_signal<int> tasten_wahl[3];
	sc_signal<int> fahrstuhlEtage;
	sc_signal<int> fahrstuhlModus;
	sc_signal<int> counter1,counter2,counter3;
	sc_signal<int> einAussteigen1, einAussteigen2, einAussteigen3;

	// generating the modules
	Passagier pass1("Passagier1");
	Passagier pass2("Passagier2");
	Passagier pass3("Passagier3");
	Steuerung steuer("Steuerung");
	Fahrstuhl fahrst("Fahrstuhl");


	// connecting modules via signals
	pass1.tasten_und_ziel_wahl(tasten_wahl[0]);
	pass1.einAussteigen(einAussteigen1);
	pass1.fahrstuhlModus(fahrstuhlModus);
	pass1.counter(counter1);
	steuer.tasten_und_ziel_wahl[0](tasten_wahl[0]);
	steuer.einAussteigen1(einAussteigen1);
	steuer.counter1(counter1);
	pass2.tasten_und_ziel_wahl(tasten_wahl[1]);
	pass2.einAussteigen(einAussteigen2);
	pass2.fahrstuhlModus(fahrstuhlModus);
	pass2.counter(counter2);
	steuer.tasten_und_ziel_wahl[1](tasten_wahl[1]);
	steuer.einAussteigen2(einAussteigen2);
	steuer.counter2(counter2);
	pass3.tasten_und_ziel_wahl(tasten_wahl[2]);
	pass3.einAussteigen(einAussteigen3);
	pass3.fahrstuhlModus(fahrstuhlModus);
	pass3.counter(counter3);
	steuer.tasten_und_ziel_wahl[2](tasten_wahl[2]);
	steuer.einAussteigen3(einAussteigen3);
	steuer.counter3(counter3);


	fahrst.fahrstuhl_etage(fahrstuhlEtage);
	steuer.fahrstuhl_etage(fahrstuhlEtage);
	fahrst.fahrstuhl_modus(fahrstuhlModus);
	steuer.fahrstuhl_modus(fahrstuhlModus);


	std::cout << "Ausfuhrung gestartet...\n";

	// Run the simulation till sc_stop is encountered
	sc_start();

	return 0;

};
