#include "global.h"
#include <avr/interrupt.h>

ISR (TIMER0_COMPA_vect){
	
	//start new injection
	if (inj_port.status & (1 << scheduled)) {
		inj_port.status &= ~ (1 << scheduled);		// turn off schedule bit (is now current operation)
		inj_port.status |= (1 << firing);			// turn on "firing" bit
		INJECTOR_PORT |= (1 << INJECTOR);			// turn on Injector		
	}
	
	//check for end of injection
	if (inj_port.status & (1 << firing)) {
		if (inj_port.pwrun >= inj_port.pw) {
			inj_port.status &= ~((1 << firing) | (1 << scheduled));
			INJECTOR_PORT &= (1 << INJECTOR);		// turn Off Injector
		}
		inj_port.pwrun++;
	}
	
	if (engine.status & (1 << running)) {
		engine.rpm_c++;								// update the rpm counter
		
		if ((engine.rpm_c >> 8) >= 100) {			// if RPM counter is 25500 (or RPM period = 2.5 sec) then engine stalled
			engine.status = 0;						// engine is stalled, clear all in engine
			engine.rpm_c = 0;
			engine.rpm = 0;
		
			FUELPUMP_PORT &= ~ (1 << FUELPUMP);		// turn off fuel Pump

			inj_port.pw = 0;						// zero out pulse width
		}
	}
}
