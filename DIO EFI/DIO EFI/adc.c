#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include "adc.h"


void ADC_Init(void)
{
	ADMUX = ADC_VREF_TYPE;
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t ADC_read(uint8_t adc_input)
{
	ADMUX = adc_input | ADC_VREF_TYPE;
	_delay_us(10);
	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADIF)) == 0);
	ADCSRA |= (1 << ADIF);
	return ADCH;
}

