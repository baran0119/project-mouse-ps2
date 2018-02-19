#include "gpio.h"

//#define PIN_GPIO_CLK 1UL // PTC1
//#define PIN_GPIO_DATA 2UL // PTC2

void SetClkInput()
{
	PTC->PDDR &= ~( 1UL << PIN_GPIO_CLK );
	PORTC->PCR[PIN_GPIO_CLK] &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
}

void SetDataInput()
{
	PTC->PDDR &= ~( 1UL << PIN_GPIO_DATA );
	PORTC->PCR[PIN_GPIO_DATA] &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
}

void SetClkOutput()
{
	PTC->PDDR |= ( 1UL << PIN_GPIO_CLK );
	PORTC->PCR[PIN_GPIO_CLK] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}

void SetDataOutput()
{
	PTC->PDDR |= ( 1UL << PIN_GPIO_DATA );
	PORTC->PCR[PIN_GPIO_DATA] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}

void gpioInit()
{
	/*PORTx->PCR[x] 	|= PORT_PCR_ISF_MASK
														| PORT_PCR_MUX( x ) 
														| PORT_PCR_IRQC( x ) 
														| PORT_PCR_DSE_MASK 
														| PORT_PCR_PFE_MASK 
														| PORT_PCR_SRE_MASK
														| PORT_PCR_PE_MASK 
														| PORT_PCR_PS_MASK */

	// clock for PORTC
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	// Set pins as GPIO
	PORTC->PCR[PIN_GPIO_CLK] |= PORT_PCR_MUX( 1 );
	PORTC->PCR[PIN_GPIO_DATA] |= PORT_PCR_MUX( 1 );
	
	// Set pins as input
	SetClkInput();
	SetDataInput();
}

uint8_t readData()
{
	return (PTC->PDIR & (1 << PIN_GPIO_DATA));
}

void writeData(uint8_t data)
{
	int bit[9];
	int cnt = 0;
	for (int i = 0; i < 8; i++)
	{
		bit[i] = ( ( data >> i ) & 0x01 );
		if (bit[i] == 1 )
			cnt++;
	}
	// odd parity
	bit[8] = ~( ( cnt%2 ) & 0x01 );

// http://www.computer-engineering.org/ps2protocol/
//1)   Bring the Clock line low for at least 100 microseconds.
	SetClkOutput();
	GPIO_PinWrite(GPIOC, PIN_GPIO_CLK, LOW);
	delay_us(125);

//2)   Bring the Data line low.
	SetDataOutput();
	GPIO_PinWrite(GPIOC, PIN_GPIO_DATA, LOW);

//3)   Release the Clock line.
	SetClkInput();

//4)   Wait for the device to bring the Clock line low.
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_CLK) & 0x01U) == LOW ) );
	//delay_us(1);

	for (int i = 0; i < 9; i++)
	{
//5)   Set/reset the Data line to send the first data bit
	GPIO_PinWrite(GPIOC, PIN_GPIO_DATA, bit[i]);
	
//6)   Wait for the device to bring Clock high.
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_CLK) & 0x01U) == HIGH ) );
	
//7)   Wait for the device to bring Clock low.
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_CLK) & 0x01U) == LOW ) );
	
//8)   Repeat steps 5-7 for the other seven data bits and the parity bit
	}

//9)   Release the Data line.
	SetDataInput();

//10) Wait for the device to bring Data low.
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_CLK) & 0x01U) == LOW ) );

//11) Wait for the device to bring Clock low.
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_CLK) & 0x01U) == LOW ) );

//12) Wait for the device to release Data and Clock
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_DATA) & 0x01U) == HIGH ) );
	while( !( ( ( (PTC->PDIR) >> PIN_GPIO_CLK) & 0x01U) == HIGH ) );
}
