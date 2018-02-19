#include "leds.h"

void ledsInit()
{
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinMux(PORTD, PIN_LED_GREEN, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, PIN_LED_RED, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, PIN_LED_GREEN, &configLED);
	GPIO_PinInit(GPIOE, PIN_LED_RED, &configLED);
	ledGreenOff();
	ledRedOff();
}

void ledGreenOn()
{
	GPIO_PinWrite(GPIOD, PIN_LED_GREEN, LOGIC_LED_ON);
}

void ledGreenOff()
{
	GPIO_PinWrite(GPIOD, PIN_LED_GREEN, LOGIC_LED_OFF);
}

void ledGreenToggle()
{
	if ( GPIO_ReadPinInput(GPIOD, PIN_LED_GREEN) == LOGIC_LED_ON )
		ledGreenOff();
	else if ( GPIO_ReadPinInput(GPIOD, PIN_LED_GREEN) == LOGIC_LED_OFF )
		ledGreenOn();
}

void ledRedOn()
{
	GPIO_PinWrite(GPIOE, PIN_LED_RED, LOGIC_LED_ON);
}

void ledRedOff()
{
	GPIO_PinWrite(GPIOE, PIN_LED_RED, LOGIC_LED_OFF);
}

void ledRedToggle()
{
	if ( GPIO_ReadPinInput(GPIOE, PIN_LED_RED) == LOGIC_LED_ON )
		ledRedOff();
	else if ( GPIO_ReadPinInput(GPIOE, PIN_LED_RED) == LOGIC_LED_OFF )
		ledRedOn();
}
