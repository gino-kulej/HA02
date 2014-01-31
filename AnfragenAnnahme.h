/*
 *  AnfragenAnnahme.h
 *  Fahrstuhlsteuerung
 *

 */

#ifndef ANFRAGENANNAHME_H_
#define ANFRAGENANNAHME_H_


#endif

SC_MODULE(AnfragenAnnahme){
	
	sc_in<int[2]> etagen_wahl [2];
	sc_out<int> neue_wahl  [2];

	int anfrage1, anfrage2, anfrage3, alt, neu;

	SC_CTOR(AnfragenAnnahme){
		SC_METHOD(aktiv);
		sensitive << etagen_wahl[0].value_changed();
	}
	
	void aktiv()
	{
		
		alt = neu;
		neu = etagen_wahl[0].read();
		if (alt != neu) {
			anfrage1 = neu;
		}
		
		alt = neu;
		neu = etagen_wahl[0].read();
		if (alt != neu) {
			anfrage1 = neu;
		}
		
		
		alt = neu;
		neu = etagen_wahl[1].read();
		if (alt != neu) {
			anfrage2 = neu;
		}
			/*if (etagen_wahl[0].event() ) 
				anfrage1 = etagen_wahl[0].read();
			else {
				etagen_wahl[1].read();
			}
			 */
			
		}
		
	}


};


