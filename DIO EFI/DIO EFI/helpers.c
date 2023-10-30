#include "helpers.h"
#include <stdlib.h>
#include <inttypes.h>


/*	Linear Interpolation - 2D
*            (y2 - y1)
*  Y = Y1 +  --------- * (x - x1)
*            (x2 - x1)
*/
uint8_t linear_interp(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t x) {
	uint8_t x21, dx;
	div_t d;

	if (x <= x1)  							// check less than
	return y1;

	if (x >= x2)  							// check greater than
	return y2;

	x21 = x2 - x1;
	if (x21 == 0) 							// divide by zero!		
	return y1;

	dx = x - x1;

	if (y2 < y1) {  						// negative slope
		d = div( (y1-y2) * dx, x21 );
		if (d.rem >= (x21 / 2)) d.quot++;	// rounding
		return y1 - d.quot;
	} else {        						// positive slope
		d = div( (y2-y1) * dx, x21 );
		if (d.rem >= (x21 / 2))  			// rounding
		d.quot++;
		return y1 + d.quot;
	}
}
