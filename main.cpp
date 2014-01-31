#include "systemc.h"
#include "Passagier.h"
#include "Steuerung.h"
#include "Fahrstuhl.h"
//#include <iostream>

int sc_main (int argc, char * argv[]) {
	
	srand (time(NULL));

	// generating the sc_signal
	sc_signal<int> tasten_wahl[3];
	sc_signal<int> fahrstuhlEtage;
	sc_signal<int> fahrstuhlModus;
	sc_signal<int> weiterFahren;
	
	// generating the modules
	Passagier pass1("Passagier1");
	Passagier pass2("Passagier2");
	Passagier pass3("Passagier3");
	Steuerung steuer("Steuerung");
	Fahrstuhl fahrst("Fahrstuhl");

	
	// connecting modules via signals
	pass1.tasten_und_ziel_wahl(tasten_wahl[0]);
	steuer.tasten_und_ziel_wahl[0](tasten_wahl[0]);
	pass2.tasten_und_ziel_wahl(tasten_wahl[1]);
	steuer.tasten_und_ziel_wahl[1](tasten_wahl[1]);
	pass3.tasten_und_ziel_wahl(tasten_wahl[2]);
	steuer.tasten_und_ziel_wahl[2](tasten_wahl[2]);

	fahrst.fahrstuhl_etage(fahrstuhlEtage);
	steuer.fahrstuhl_etage(fahrstuhlEtage);
	fahrst.fahrstuhl_modus(fahrstuhlModus);
	steuer.fahrstuhl_modus(fahrstuhlModus);
	fahrst.weiter_fahren(weiterFahren);
	steuer.weiter_fahren(weiterFahren);

	
	
	
	std::cout << "Ausfuhrung gestartet...\n";

	// Run the simulation till sc_stop is encountered
	sc_start();
	
	return 0;

};
