/*
 * led.c
 *
 * Created: 27-May-23 2:13:09 AM
 *  Author: Amr
 */ 


#include "LED.h"


/**DESCRIPTION:-
it intializes a pin to be an output pin
**/
LED_ERROR_TYPE LED_INIT(DIO_Pin_type PIN)
{
	DIO_Pin_type PIN_NUM;
	DIO_Port_type PORT;
	PORT=PIN/8;
	PIN_NUM=PIN%8;
	if(PORT>4)
	{
		return INVALID_LED_PORT;
	}
	else if(PIN_NUM>32)
	{
		return INVLAID_LED_PIN_NUMBER;
	}
	else
	{
		DIO_initpin(PIN,OUTPUT);
		return VALID_LED;
	}
}
/**************************************************************************************************************************/
/**DESCRIPTION:-
it sets a certain pin with a high state so that the led will be on
**/
LED_ERROR_TYPE LED_ON(DIO_Pin_type PIN)
{
	DIO_Pin_type PIN_NUM;
	DIO_Port_type PORT;
	PORT=PIN/8;
	PIN_NUM=PIN%8;
	if(PORT>4)
	{
		return INVALID_LED_PORT;
	}
	else if(PIN_NUM>32)
	{
		return INVLAID_LED_PIN_NUMBER;
	}
	else
	{
		DIO_writepin(PIN,HIGH);
		return VALID_LED;
	}
}
/*************************************************************************************************************************/
/**DESCRIPTION:-
it sets a sertatin pin with a low voltage so that the led will be off
**/

LED_ERROR_TYPE LED_OFF(DIO_Pin_type PIN)
{
	DIO_Pin_type PIN_NUM;
	DIO_Port_type PORT;
	PORT=PIN/8;
	PIN_NUM=PIN%8;
	if(PORT>4)
	{
		return INVALID_LED_PORT;
	}
	else if(PIN_NUM>32)
	{
		return INVLAID_LED_PIN_NUMBER;
	}
	else
	{
		DIO_writepin(PIN,LOW);
		return VALID_LED;
	}
}