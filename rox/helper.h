/****************************************************************************
*
*   Copyright (c) 2007 Osiloke Harold Emoekpere
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*	with help from societyofrobots.com and other sources
*
****************************************************************************/

//AVR includes
#include <avr/io.h>
// include I/O definitions (port names, pin names, etc)
#include <avr/signal.h>
// include "signal" names (interrupt names)
#include <avr/interrupt.h>
// include interrupt support
#include "global.h"
// include our global settings
#include "timer.h"
// include timer function library (timing, PWM, etc)
#include "pulse.h"
// include pulse output support
#include "servo.h"
#include "iocompat.h"  
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "a2d.h"		// include A/D converter function library
//from SOR --- thanks guys
//define port functions; example: PORT_ON( PORTD, 6);
#define PORT_ON( port_letter, number )			port_letter |= (1<<number)
#define PORT_OFF( port_letter, number )			port_letter &= ~(1<<number)
#define PORT_ALL_ON( port_letter, number )		port_letter |= (number)
#define PORT_ALL_OFF( port_letter, number )		port_letter &= ~(number)
#define FLIP_PORT( port_letter, number )		port_letter ^= (1<<number)
#define PORT_IS_ON( port_letter, number )		( port_letter & (1<<number) )
#define PORT_IS_OFF( port_letter, number )		!( port_letter & (1<<number) )
//make pin output or input
#define PIN_OUT( pin_name, number )			pin_name |= (_BV(number))
#define PIN_IN( pin_name, number )			pin_name &= (~_BV(number))
#define PIN_ALL_OUT( pin_name)				pin_name |= 0xFF
#define PIN_ALL_IN( pin_name)				pin_name |= 0x00

// Input Ports.
#define COMMAND_PORT      PORTB
#define COMMAND_DDR       DDRB
#define COMMAND_PIN       PINB
#define COMMAND_BIT       4

#define LSENS_PORT      PORTC
#define LSENS_DDR       DDRC
#define LSENS_PIN       PINC
#define LSENS_BIT       5

#define RSENS_PORT      PORTC
#define RSENS_DDR       DDRC
#define RSENS_PIN       PINC
#define RSENS_BIT       4

#define MSENS_PORT      PORTC
#define MSENS_DDR       DDRC
#define MSENS_PIN       PINC
#define MSENS_BIT       3

// Output Ports
#define LMOT_PORT  		PORTD
#define LMOT_DDR		DDRD
#define LMOT_PIN   		PIND
#define LMOT_BIT   		2

#define RMOT_PORT  		PORTD
#define RMOT_DDR		DDRD
#define RMOT_PIN   		PIND
#define RMOT_BIT   		3

#define LLED_PORT  		PORTD
#define LLED_DDR		DDRD
#define LLED_PIN   		PIND
#define LLED_BIT   		6

#define SLED_PORT  		PORTD
#define SLED_DDR		DDRD
#define SLED_PIN   		PIND
#define SLED_BIT   		5

#define RLED_PORT  		PORTD
#define RLED_DDR		DDRD
#define RLED_PIN   		PIND
#define RLED_BIT   		7

#define PLED_PORT  		PORTD
#define PLED_DDR		DDRD
#define PLED_PIN   		PIND
#define PLED_BIT   		4

void delay_inms(uint16_t x); //General purpose delay

//#define SENS_THRESH     0

void initialize_ports(void)
	{
	//PIN_ALL_OUT(DDRB);  //configure all C ports for input
	
	/*OUTPUT PORTS*/
	//LIGHTS
	PIN_OUT(LLED_DDR,LLED_BIT);
	PIN_OUT(RLED_DDR,RLED_BIT);
	PIN_OUT(SLED_DDR,SLED_BIT);
	PIN_OUT(PLED_DDR,PLED_BIT);

	//MOTORS
	PIN_OUT(LMOT_DDR,LMOT_BIT);
	PIN_OUT(RMOT_DDR,RMOT_BIT);
	//rprintf("Initialization Complete\r\n");
	
		/*INPUT PORTS*/

	//SENSORS
	PIN_IN(LSENS_DDR,LSENS_BIT);
	PIN_IN(RSENS_DDR,RSENS_BIT);
	PIN_IN(MSENS_DDR,MSENS_BIT);
	PIN_IN(COMMAND_DDR,COMMAND_BIT);

	//SET ALL PORTS TO 0
	PORT_ALL_OFF( PORTB, 0xff );
	PORT_ALL_OFF( PORTC, 0xff );
	PORT_ALL_OFF( PORTD, 0xff );
	
	PORT_ON(LLED_PORT, LLED_BIT );//turn LED 
	PORT_ON(RLED_PORT, RLED_BIT );//turn LED 

	//DDRB |= 0x00;
	//DDRC |= 0x00;
	}
void turn_on_led(void)
	{
	//initA2D()
	PORT_OFF(PLED_PORT, PLED_BIT );//turn LED 
	}
void turn_off_led(void)
	{
	PORT_ON(PLED_PORT, PLED_BIT );//turn LED off
	}
void servo_left(uint16_t speed)
	{
	PORT_ON(LMOT_PORT, LMOT_BIT);
	delay_inms(speed);
	PORT_OFF(LMOT_PORT, LMOT_BIT);//keep off
	//delay_inms(100);
	}
void servo_right(uint16_t speed)
	{
	PORT_ON(RMOT_PORT, RMOT_BIT);
	delay_inms(speed);		
	PORT_OFF(RMOT_PORT, RMOT_BIT);//keep off
	//delay_inms(100);
	}
void move_left(void)//45, 45 maximum
	{
	
	
	turn_led('l');
	//servo_left(1.5);//dont move
	servoSetPosition(1, 52);
	//servo_right(2);//forward
	servoSetPosition(0, 255);
	}
void move_right(void)//29, 29 maximum
	{
	turn_led('r');
	servoSetPosition(1, 255);
	//servo_left(2);//forward
	servoSetPosition(0, 52);
	//servo_right(1.5);//dont move
	}
void move_straight(void)//29, 45 maximum
	{
	turn_led('s');
	//servo_left(2);//forward
	servoSetPosition(1, 255);
	//servo_right(2);//forward
	servoSetPosition(0, 255);
	}
void move_reverse(void)//29, 45 maximum
	{
	turn_led('v');
	servo_left(0);//back
	servo_right(0);//back
	}
void hold_position(void)//39, 35
	{
	
	turn_led('h');
	servo_left(52);//39
	servo_right(52);//35
	}
void delay_cycles(unsigned long int cycles)
	{
	while(cycles > 0)
		cycles--;
	}
void initA2D(void)
	{
		a2dInit(); // initialize analog to digital converter (ADC)
		a2dSetPrescaler(ADC_PRESCALE_DIV32); // configure ADC scaling
		a2dSetReference(ADC_REFERENCE_AVCC); // configure ADC reference voltage

		//rprintf("Initialization Complete\r\n");
		/**************************************************/

	}
void initUART(void)
	{
		uartInit();  // initialize UART
		uartSetBaudRate(38400);// set UART baud rate
		rprintfInit(uartSendByte);// initialize rprintf system

	}
void read_input8(int value,unsigned int port_number)
{
	value=a2dConvert8bit(port_number);
	

}
//General short delays

void delay_inms(uint16_t x)
{
  uint8_t y, z;
  for ( ; x > 0 ; x--){
    for ( y = 0 ; y < 90 ; y++){
      for ( z = 0 ; z < 6 ; z++){
        asm volatile ("nop");
      }
    }
  }
}

static void blink_n_times (int8_t n)
{
	while (n-- > 0)
	{
                /* led on, pin=0 */
                turn_on_led();
                delay_inms(200);
                /* set output to 5V, LED off */
                turn_off_led();
                delay_inms(200);
	}
}
void turn_led(char led)
{
	switch(led)
	{
		case 'l':
			PORT_OFF(LLED_PORT, LLED_BIT );//turn LED 
			PORT_ON(RLED_PORT, RLED_BIT );//turn LED 
			//PORT_ON(SLED_PORT, SLED_BIT );//turn LED
			break;

		case 'r':		
			PORT_ON(LLED_PORT, LLED_BIT );//turn LED 
			PORT_OFF(RLED_PORT, RLED_BIT );//turn LED 
			//PORT_ON(SLED_PORT, SLED_BIT );//turn LED
			break;

		case 's':		
			PORT_OFF(LLED_PORT, LLED_BIT );//turn LED 
			PORT_OFF(RLED_PORT, RLED_BIT );//turn LED 
			//PORT_OFF(SLED_PORT, SLED_BIT );//turn LED
			break;

		case 'v':		
			PORT_ON(LLED_PORT, LLED_BIT );//turn LED 
			PORT_ON(RLED_PORT, RLED_BIT );//turn LED 
			//PORT_OFF(SLED_PORT, SLED_BIT );//turn LED
			break;

		case 'h':		
			PORT_ON(LLED_PORT, LLED_BIT );//turn LED 
			PORT_ON(RLED_PORT, RLED_BIT );//turn LED 
			//PORT_ON(SLED_PORT, SLED_BIT );//turn LED
			break;

		default:
			break;
	}



}

