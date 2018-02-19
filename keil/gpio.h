#ifndef GPIO_H
#define GPIO_H

#include "MKL43Z4.h"
#include "extra.h"
#include "main.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#define CMD_SET_DEFAULTS 0xF6
#define CMD_ENABLE_DATA_REPORTING 0xF4

#define LOW 0
#define HIGH 1

void SetClkInput(void);
void SetDataInput(void);
void SetClkOutput(void);
void SetDataOutput(void);

void gpioInit(void);
uint8_t readData(void);
void writeData(uint8_t data);

#endif //GPIO_H
