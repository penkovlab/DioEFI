#include <inttypes.h>
#include "helpers.h"
#include "global.h"
#include <avr/interrupt.h>

/*	Computation of RPM
*			rpmk
*	rpm = --------
*			rpmp
*/
void calc_rpm(void) {
	uint16_t rpmp;	// period count between IRQ pulsed lines, in 0.1 millisecond resolution
	uint16_t rpmk;	// RPM constant

	cli();
	rpmp = engine.rpm_p;
	sei();

	rpmk = (config.rpmk_1 << 8) | config.rpmk_2;

	if (rpmp) {
		rpmp = rpmk / rpmp;

		if ((rpmp >> 8) != 0)
		engine.rpm = 255;					// 25500 RPM max
		else
		engine.rpm = rpmp & 0xFF;			// take the last 8 bits of the value
	} else {
		engine.rpm = 0;
	}
}

/*	Cranking Mode
*	Pulse width is directly set by the coolant temperature value of
*	CWL (at -40 degrees) and CWH (at 165 degrees) - value is interpolated 
*/
void cranking(void) {
	uint8_t pw;
	uint8_t my_status;

	cli();
	my_status = engine.status;
	my_status |= (1 << crank);
	my_status &= ~( (1 << startw) | (1 << warmup) );
	engine.status = my_status;
	sei();

	pw = linear_interp(0, 205, config.cwl, config.cwh, engine.coolant);

	cli();
	inj_port.pwcalc = pw;
	sei();
}

/*
void warmup_enrich(void) {
	uint8_t warm_enrich;
	uint8_t my_status;
	struct search_table_t st;

	cli();
	my_status = engine.status;
	if (my_status & (uint8_t) (1 << crank)) {
		my_status &= ~ (1 << crank);
		my_status |= (1 << startw) | (1 << warmup);
		engine.status = my_status;
		asecount = 0;
	}
	sei();



	 используем температуру двигателя для нахождения wwurange, 
	 "упакуем" 256 возможных значений сигнала датчика в [0-9] шкалу 
	search_table(config.wwurange, sizeof(config.wwurange), engine.coolant, &st);

	 затем, используя интерполяцию по wwu-table, 
	 а также шкалу [0-9] для нахождения значения обогащения 
	warm_enrich = linear_interp(st.lbound, st.ubound, config.wwu[st.index-1],
	config.wwu[st.index], engine.coolant);

	if (warm_enrich > 100) {
		cli();
		engine.status |= (1 << warmup);
		sei();
		
		 обогащение после пуска двигателя 
		if (engine.status & (1 << startw)) {
			if (asecount <= config.awc) {
				uint16_t wue;
				wue = warm_enrich + linear_interp(0, config.awc, config.awev, 0, asecount);
				if ((wue >> 8) != 0) wue = 0xFF;   проверка на переполнение 
				warm_enrich = (uint8_t)wue;
				} else {
				cli();
				engine.status &= ~ (1 << startw);
				sei();
			}
		}
		} else {
		 обогащения на прогреве закончилось 
		warm_enrich = 100;
		
		cli();
		engine.status &= ~( (1 << startw) | (1 << warmup) );
		sei();
		
	}

	corr.warm = warm_enrich;
}

void ve_table_lookup(void) {
	uint8_t ve_11, ve_12, ve_21, ve_22;
	uint8_t ve_low_kpa, ve_high_kpa;
	struct search_table_t kpa, rpm;

	search_table(config.kparangeve, sizeof(config.kparangeve), engine.kpa, &kpa);
	search_table(config.rpmrangeve, sizeof(config.rpmrangeve), engine.rpm, &rpm);

	ve_11 = *(config.VE+8*(kpa.index-1)+(rpm.index-1));
	ve_12 = *(config.VE+8*(kpa.index-1)+rpm.index);
	ve_21 = *(config.VE+8*kpa.index+(rpm.index-1));
	ve_22 = *(config.VE+8*kpa.index+rpm.index);
	
	ve_low_kpa = linear_interp(rpm.lbound, rpm.ubound, ve_11, ve_12, engine.rpm);
	ve_high_kpa = linear_interp(rpm.lbound, rpm.ubound, ve_21, ve_22, engine.rpm);

	corr.ve = linear_interp(kpa.lbound, kpa.ubound, ve_low_kpa, ve_high_kpa, engine.kpa);
}

void calc_total_enrichment(void) {
	uint8_t fuel_tmp, batt_tmp, pw;
	uint8_t batt_high, batt_low;
	uint16_t res;

	// 8-bit x 16-bit multiplications
	res = (uint16_t)corr.warm;
	res = mult_div100(corr.tpsfuelcut, res);
	res = mult_div100(corr.air, res);
	res = mult_div100(corr.ego, res);
	res = mult_div100(corr.baro, res);

	if ((res >> 8) != 0)                              
	corr.gammae = 0xFF;
	else
	corr.gammae = (uint8_t)res;                    

	res = mult_div100(corr.ve, res);
	
	if (!(config.config13 & _BV(CONTROL_STRATEGY)))  // speed-density
	res = mult_div100(engine.kpa, res);

	res = mult_div100(config.req_fuel, res);

	if ((res >> 8) != 0)                             // ????????? ???????????? "... ????? ????? ??????!"
	fuel_tmp = 0xFF;
	else
	fuel_tmp = (uint8_t)res;


	 battery voltage compensation 
	
	batt_low = config.injopen + config.battfac;

	if (config.injopen <= config.battfac)
	batt_high = 0;
	else
	batt_high = config.injopen - config.battfac;

	#warning "this should be configurable via configuration struct"
	batt_tmp = linear_interp(61, 164, batt_low, batt_high, sensors[BATT]);

	// final pulsewidth calculation, wuhuw!
	if (fuel_tmp) {
		res = batt_tmp + fuel_tmp + corr.tpsaccel - config.injocfuel;
		if ((res >> 8) != 0)
		pw = 0xFF;
		else
		pw = (uint8_t)res;
		} else {
		pw = 0;
	}

	cli();
	inj_port.pwcalc = pw;
	sei();

}
*/