#include "global.h"
#include <avr/interrupt.h>

ISR (INT0_vect){
	engine.rpm_p = engine.rpm_c;			// rpm period
	engine.rpm_c = 0;						// reset rpm counter
	FUELPUMP_PORT |= (1 << FUELPUMP);		// turn on fuel Pump
	engine.status |= (1 << running);		// set engine running value
	
	if (asecount < 255) asecount++;			// after-start enrichment counter

	inj_port.pw = inj_port.pwcalc;			// latched calculated pulse width
	inj_port.pwrun = 0;						// running counter variable set to zero
	inj_port.status |= (1 << scheduled);	// squirt on every pulse if cranking
}
