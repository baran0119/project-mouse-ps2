#ifndef leds_h
#define leds_h

#include "MKL43Z4.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#define PIN_LED_GREEN 5UL // PTD5
#define PIN_LED_RED 31UL // PTE31

#define LOGIC_LED_ON 0
#define LOGIC_LED_OFF 1

static const gpio_pin_config_t configLED =
{
	kGPIO_DigitalOutput,
	LOGIC_LED_OFF,
};

void ledsInit(void);

void ledGreenOn(void);
void ledGreenOff(void);
void ledGreenToggle(void);
void ledRedOn(void);
void ledRedOff(void);
void ledRedToggle(void);

#endif //leds_h
