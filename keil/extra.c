#include "extra.h"												//Declarations

/*----------------------------------------------------------------------------
	Function making delay of value * 48 clock cycles
*----------------------------------------------------------------------------*/
void delay_us(uint32_t value){
	uint32_t delay, x;
	
	for(x=0; x < value; x++){
		for(delay=0; delay < 48; delay++){};
	}
}
