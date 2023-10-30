#include <avr/io.h>
#include <inttypes.h>
#include "adc.h"
#include "global.h"
#include "fuelcalc.h"
#include <avr/interrupt.h>

//TODO
// Опрос датчиков
// Обогащение в рабочем режиме
// Связь с TunerStudio
// Сохранение настроек


int main(void)
{
	FUELPUMP_DDR |= (1 << FUELPUMP);
	INJECTOR_DDR |= (1 << INJECTOR);
	
	TCCR0A |= (1 << WGM01);									// CTC mode - clear when matching
	OCR0A = 200;											// for 0.1ms period
	TIMSK0 |= (1 << OCIE0A);								// timer/counter compare match interrupt enable
	TCCR0B |= (0 << CS00) | (1 << CS01) | (0 << CS02);		// clk/8
	
	ADC_Init();
	
	FUELPUMP_PORT |= (1 << FUELPUMP);						// turn on fuel Pump
	
	EICRA |= (1<<ISC01) | (1<<ISC01);						// the rising edge of INT0 generates an interrupt request.
	EIMSK |= (1<<INT0);										// external Interrupt Request 0 Enable

	sei();

    while (1) 
    {	
		if (engine.status & (1 << running)) {
			calc_rpm();										// computation of RPM

			if (engine.rpm <= config.cranking_thres) {		// check if we are cranking
				cranking();
			} else {
				//warmup_enrich();
				//ve_table_lookup();
				//calc_total_enrichment();
			}
		}		
    }
}
















