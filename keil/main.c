#include "MKL43Z4.h"
#include "leds.h"
#include "slcd.h"
#include "main.h"
#include "gpio.h"

uint8_t tab[33];
uint8_t bit = 0;
int	dataReady = 0;

int main()
{
	ledsInit();
	slcdInit();
	slcdDisplay(0, 16);
	gpioInit();

	delay_us(100);
	writeData(CMD_SET_DEFAULTS); delay_us(100);
	writeData(CMD_ENABLE_DATA_REPORTING); delay_us(100);

	initInterrupt();

	while(1)
	{
		// polling data ready
		if (dataReady == 1)
		{
			uint8_t XDirectionByte = 0;	// bajt kierunku X (poziom)
			uint8_t YDirectionByte = 0;	// bajt kierunku Y (pion)
			uint8_t LPM = 0; // lewy przycisk myszy
			uint8_t PPM = 0; // prawy przycisk myszy
			uint8_t XSign = 0; // bit znaku X
			uint8_t YSign = 0; // bit znaku Y
			uint16_t ToDisplay = 0; // do wyswietlenia
			
			// bajty przesuniecia
			for (int i = 0; i < 8; i++)
			{
				XDirectionByte |= ( ( tab[SHIFT_X + i] & 0x1 ) << i );
				YDirectionByte |= ( ( tab[SHIFT_Y + i] & 0x1 ) << i );
			}

			// flagi
			LPM = tab[1] & 0x1;
			PPM = tab[2] & 0x1;
			XSign = tab[5] & 0x1;
			YSign = tab[6] & 0x1;

			// Zielony led - lewy przycisk
			if ( LPM == 1 )
				ledGreenOn();
			else
				ledGreenOff();

			// Czerwony led - prawy przycisk
			if ( PPM == 1 )
				ledRedOn();
			else
				ledRedOff();

			// Wyswietl na wyswietlaczu
			ToDisplay = ( ( XDirectionByte << 8 ) | YDirectionByte );
			slcdDisplay(ToDisplay,16);

			// Kropka jezeli minus = w lewo
			if ( XSign == 1 )
				slcdWlaczKropke(1UL);
			else
				slcdWylaczKropke(1UL);

			// Kropka jezeli minus = w dol
			if ( YSign == 1 )
				slcdWlaczKropke(3UL);
			else
				slcdWylaczKropke(3UL);
		}
		else
			delay_us(100);

		dataReady = 0;
	}

}

void PORTC_PORTD_IRQHandler()
{
	// interrupt from clk ?
	if ( ( (PORTC->ISFR) >> PIN_GPIO_CLK )  & 0x01U )
	{
		if ( bit < 33 )
		{
			tab[bit] = readData();
			bit++;
		}
		else
		{
			dataReady = 1;
			bit = 0;
		}
	}

	NVIC_ClearPendingIRQ(PORTC_D_INTERRUPT);
	PORTC->PCR[PIN_GPIO_CLK] |= PORT_PCR_ISF_MASK;
	PORTC->ISFR |= PORT_ISFR_ISF(PIN_GPIO_CLK);
}

void initInterrupt()
{
	SetClkInput();
	SetDataInput();

	// Configure interrupts
	PORTC->PCR[PIN_GPIO_CLK] |= PORT_PCR_IRQC(10); // falling-edge

	NVIC_SetPriority(PORTC_D_INTERRUPT, 1);
	NVIC_ClearPendingIRQ(PORTC_D_INTERRUPT);
	NVIC_EnableIRQ(PORTC_D_INTERRUPT);
}
