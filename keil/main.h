#ifndef MAIN_H
#define MAIN_H

#include "MKL43Z4.h"

#define PIN_GPIO_CLK 1UL // PTA1/PTC1
#define PIN_GPIO_DATA 2UL // PTA2/PTC2

#define PORTA_INTERRUPT (IRQn_Type) PORTA_IRQn
#define PORTC_D_INTERRUPT (IRQn_Type) PORTC_PORTD_IRQn

#define	SHIFT_X (12)
#define	SHIFT_Y (23)

extern int dataReady;
extern uint8_t tab[33];
extern uint8_t bit;

//extern uint8_t MouseStatusByte; // bajt statusowy myszy
extern uint8_t XDirectionByte;	// bajt kierunku X (poziom)
extern uint8_t YDirectionByte;	// bajt kierunku Y (pion)
extern uint8_t ZDirectionByte;	// bajt kierunku Y (pion)

extern uint8_t LPM; // lewy przycisk myszy
extern uint8_t PPM; // prawy przycisk myszy
extern uint8_t XSign; // bit znaku X
extern uint8_t YSign; // bit znaku Y
extern uint16_t ToDisplay; // do wyswietlenia

void PORTA_IRQHandler(void);
void PORTC_PORTD_IRQHandler(void);
void initInterrupt(void);

#endif //MAIN_H
