#include "slcd.h"

/* Function that initializes sLCD */
void slcdInit(void){
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | /* Enable Clock to Port B & C */ 
								SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_SLCD_MASK; /* Enable Clock to Port D & E and sLCD module*/ 
	LCD->GCR |= LCD_GCR_PADSAFE_MASK; // Set PADSAFE during configuration
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK; // Clear LCDEN during configuration
	//multiplexers configuration for ports to act as sLCD controller

	PORTE->PCR[20] = PORT_PCR_MUX(0u);	// P59
	PORTE->PCR[21] = PORT_PCR_MUX(0u);	// P60
	PORTB->PCR[18] = PORT_PCR_MUX(0u);	// P14
	PORTB->PCR[19] = PORT_PCR_MUX(0u);	// P15
	PORTC->PCR[0] = PORT_PCR_MUX(0u);	  // P20
	PORTC->PCR[4] = PORT_PCR_MUX(0u);	  // P24
	PORTC->PCR[6] = PORT_PCR_MUX(0u);	  // P26
	PORTC->PCR[7] = PORT_PCR_MUX(0u);	  // P27
	PORTD->PCR[0] = PORT_PCR_MUX(0u);		// P40
	PORTD->PCR[2] = PORT_PCR_MUX(0u);	  // P42
	PORTD->PCR[3] = PORT_PCR_MUX(0u);	  // P43
	PORTD->PCR[4] = PORT_PCR_MUX(0u);	  // P44

	//sLCD register configuration
	LCD->GCR = LCD_GCR_RVTRIM(0x00) | // Regulated Voltage Trim
             LCD_GCR_CPSEL_MASK |   // Charge Pump
             LCD_GCR_LADJ(0x03) |   // Load Adjust
             LCD_GCR_VSUPPLY_MASK | //
             LCD_GCR_ALTDIV(0x00) | //
             LCD_GCR_SOURCE_MASK |  //
             LCD_GCR_LCLK(0x01) |   //
						 LCD_GCR_DUTY(0x03);    //
	//sLCD blinking configuration
	LCD->AR = LCD_AR_BRATE(0x03);	
	//FDCR register configuration
	LCD->FDCR = 0x00000000;
  //activation of 12 pins to control sLCD (2 registers, 32 bits each)
	LCD->PEN[0] = LCD_PEN_PEN(1u<<14) | // LCD_P14
								LCD_PEN_PEN(1u<<15) | // LCD_P15
								LCD_PEN_PEN(1u<<20) | // LCD_P20 
								LCD_PEN_PEN(1u<<24) | // LCD_P24
								LCD_PEN_PEN(1u<<26) | // LCD_P26
								LCD_PEN_PEN(1u<<27); // LCD_P27
	LCD->PEN[1] = LCD_PEN_PEN(1u<<8) | // LCD_P40
								LCD_PEN_PEN(1u<<10) | // LCD_P42
								LCD_PEN_PEN(1u<<11) | // LCD_P43
								LCD_PEN_PEN(1u<<12) | // LCD_P44
								LCD_PEN_PEN(1u<<27) | // LCD_P59
								LCD_PEN_PEN(1u<<28); // LCD_P60

	//configuration of 4 back plane pins (2 registers, 32 bits each)
	LCD->BPEN[0] = LCD_BPEN_BPEN(1u<<14) | //LCD_P14
								 LCD_BPEN_BPEN(1u<<15); //LCD_P15
	LCD->BPEN[1] = LCD_BPEN_BPEN(1u<<27) | //LCD_P40
								 LCD_BPEN_BPEN(1u<<28); //LCD_P52

	// waveform registers configuration – 4 active (because of 4 back planes)
	// (44.3.7 in KL46 Reference Manual)
	LCD->WF[3] =  LCD_WF_WF12(0x00) | LCD_WF_WF13(0x00) |
							  LCD_WF_WF14(0x44) | // COM2
							  LCD_WF_WF15(0x88);  // COM3

	LCD->WF[14] = LCD_WF_WF56(0x00) | LCD_WF_WF57(0x00) | LCD_WF_WF58(0x00) |
								LCD_WF_WF59(0x11);	// COM0
	
	LCD->WF[15] = LCD_WF_WF60(0x22) | // COM1 (00100010)
								LCD_WF_WF61(0x00) | LCD_WF_WF62(0x00) | LCD_WF_WF63(0x00);
	
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; // Clear PADSAFE at the end of configuration
	LCD->GCR |= LCD_GCR_LCDEN_MASK; // Set LCDEN at the end of configuration
	
		slcdSet(8,1);slcdSet(8,2);slcdSet(8,3);slcdSet(8,4);
			//test kropek
			slcdWlaczKropke(1U);delay_us(150);slcdWylaczKropke(1U);delay_us(50);
			slcdWlaczKropke(2U);delay_us(150);slcdWylaczKropke(2U);delay_us(50);
			slcdWlaczKropke(3U);delay_us(150);slcdWylaczKropke(3U);delay_us(50);
			slcdWlaczKropke(4U);delay_us(150);slcdWylaczKropke(4U);delay_us(50);
	//slcdWlaczKropke(1U);slcdWlaczKropke(2U);slcdWlaczKropke(3U);slcdWlaczKropke(4U);delay_us(250);
}
/* Function that outputs on sLCD (number) error message for debug purpose */
void slcdErr(uint8_t number){
	// E
	LCD->WF8B[LCD_Front_Pin[0]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F); //1st digit
	LCD->WF8B[LCD_Front_Pin[1]] = (LCD_S_A);
	// r
	LCD->WF8B[LCD_Front_Pin[2]] = (LCD_S_E | LCD_S_G); //2nd digit
	LCD->WF8B[LCD_Front_Pin[3]] = (LCD_C);
	// r
	LCD->WF8B[LCD_Front_Pin[4]] = (LCD_S_E | LCD_S_G); //3rd digit
	LCD->WF8B[LCD_Front_Pin[5]] = (LCD_C);

	switch(number){  //4 digit depends on provided number
		case 0x00:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_D | LCD_S_E | LCD_S_F); /* To display '0' we must active segments: d, e & f on first front plane */
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C); /* To display '0' we must active segments: a, b & c on second front plane */
			break;
		case 0x01:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_C);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_B | LCD_S_C);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x02:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_D | LCD_S_E | LCD_S_G);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_B | LCD_S_A);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x03:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_D | LCD_S_G);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_C | LCD_S_B | LCD_S_A);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x04:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_B | LCD_S_C);
			break;
		default:   //as default display 'r'
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_C);
			break;
	}
}
/*----------------------------------------------------------------------------
  Writing on sLCD (value) on specified position (digit)
*
* FP0 - Front Plane 0
*  f |_g
*  e |_
*    d
*
* FP1 - Front Plane 1
*    a_
*      | b
*    c |.
*        dot
 *----------------------------------------------------------------------------*/
void slcdSet(uint8_t value, uint8_t digit){	
	switch(value){
		case 0x00:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_D | LCD_S_E | LCD_S_F); /* To display '0' we must active segments: d, e & f on first front plane */
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C); /* To display '0' we must active segments: a, b & c on second front plane */
			break;
		case 0x01:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_C);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_C | LCD_S_B);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x02:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_D | LCD_S_E | LCD_S_G);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_B | LCD_S_A);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x03:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_D | LCD_S_G);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_C | LCD_S_B | LCD_S_A);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x04:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_B | LCD_S_C);
			break;
		case 0x05:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_F | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_C);
			break;
		case 0x06:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D  | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_C);
			break;
		case 0x07:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_C);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x08:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D  | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x09:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_G | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x0A:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x0B:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_C);
			break;
		case 0x0C:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_E | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A);
			break;
		case 0x0D:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_B | LCD_S_C);
			break;
		case 0x0E:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A);
			break;
		case 0x0F:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A);
			break;
		default:   //as default display 'r'
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_C);
			break;
	}
	if(digit>4){ 
		slcdErr(1);
	}
}
/* Function for conversion purpose, displaying (value) in specified (format) */
void slcdDisplay(uint16_t value ,uint16_t format){	
	uint16_t tab[4] = {0, 0, 0, 0};
	int i = 0;

	for(i = 3; i>=0; i--){
		tab[i] = value % format;
		value = value / format;
	}

	for(i=0; i < 4; i++){		
		slcdSet(tab[i], i+1);
	}
}
/* Clear sLCD */
void slcdClear(void){
	uint8_t i = 0;
	
	for(i=0; i < 8; i++){
		LCD->WF8B[LCD_Front_Pin[i]] = LCD_C;
	}
}

// zad. 1. kropka - która kropka - 1, 2 lub 3, natomiast 4 to dwukropek
void slcdWlaczKropke(uint8_t kropka){
	LCD->WF8B[LCD_Front_Pin[((2*kropka)-1)]] |= LCD_S_DEC; // 0x11 = 00010001
}
void slcdWylaczKropke(uint8_t kropka){
	LCD->WF8B[LCD_Front_Pin[((2*kropka)-1)]] &= ~LCD_S_DEC; // 0xEE = 11101110
}
