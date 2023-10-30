#ifndef ADC_H_
#define ADC_H_
#define ADC_VREF_TYPE ((1 << REFS0) | (1 << ADLAR))

void ADC_Init(void);
uint8_t ADC_read(uint8_t);

#endif /* ADC_H_ */