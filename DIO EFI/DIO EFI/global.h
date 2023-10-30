#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <inttypes.h>

#define F_CPU 16000000

//Hardware Wiring
#define INJECTOR_PORT	PORTD
#define INJECTOR_DDR	DDRD
#define INJECTOR		0

#define FUELPUMP_PORT	PORTD
#define FUELPUMP_DDR	DDRD
#define FUELPUMP		0

/***********************************************************************/
#define firing    0	   // 0 = not squirting		1 = squirting
#define scheduled 1	   // 0 = nothing scheduled 1 = scheduled to squirt

struct squirt_t {
	uint8_t status;    // uses the defines above
	uint8_t pw;        // injector squirt time in 1/10 ms (0 to 25.5 ms)
	uint8_t pwrun;     // pulse width timing variable - from 0 to 25.5 ms
	uint8_t pwcalc;    // next pulse width
};
volatile struct squirt_t inj_port;
/***********************************************************************/
#define running 0  // 0 = engine not running 1 = running
#define crank   1  // 0 = engine not cranking 1 = engine cranking
#define startw  2  // 0 = not in startup warmup 1 = in warmup enrichment
#define warmup  3  // 0 = not in warmup 1 = in warmup

struct engine_t {
	uint8_t  status;     // uses the defines above
	uint16_t rpm_p;      // rpm period
	uint16_t rpm_c;      // rpm counter
	uint8_t  rpm;        // computed engine RPM - rpm/100
	uint8_t  kpa;        // MAP value in units of KPa
	uint8_t  coolant;    // coolant temperature in Degrees F plus 40 (allows -40 degress to fit in integer)
	uint8_t  batt;       // current battery voltage
};
volatile struct engine_t engine;
/***********************************************************************/
struct corr_t {
	uint8_t warm;		 // total Warm up Correction
};
struct corr_t corr;
/***********************************************************************/
volatile uint8_t asecount; // counter value for after-start enrichment counter - every ignition pulse

/***********************************************************************/
/* Configuration variables */
/***********************************************************************/
struct config_t {
	uint8_t awc;				// After-start number of cycles
	uint8_t wwu[10];			// Warmup bins(function of temperature)
	uint8_t rpmk_1;				// Constant for RPM = 12,000/ncyl - downloaded constant
	uint8_t rpmk_2;				// Constant for RPM = 12,000/ncyl - downloaded constant
	uint8_t cranking_thres; 	// ѕорог режима пуска по оборотам двигател€ (х/100)
	uint8_t wwurange[10]; 	    // WWURANGE has an offset of +40 F
	uint8_t cwl;				// Crank Enrichment at -40
	uint8_t cwh;				// Crank Enrichment at 165
};
volatile struct config_t config;



#endif /* GLOBAL_H_ */