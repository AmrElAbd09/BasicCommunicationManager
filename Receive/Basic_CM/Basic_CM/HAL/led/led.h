/*
 * led.h
 *
 * Created: 27-May-23 2:13:19 AM
 *  Author: Amr
 */ 



#ifndef led_H_
#define led_H_
/***************************includes**************************************/
#include "../../MCAL/dio/dio.h"
/*************************************************************************/


/****************************enums****************************************/
typedef enum {
	VALID_LED,
	INVALID_LED_PORT,
	INVLAID_LED_PIN_NUMBER
}LED_ERROR_TYPE;
/*********************************************************************/

/**********************func_prototypes********************************/
LED_ERROR_TYPE LED_INIT(DIO_Pin_type PIN);
LED_ERROR_TYPE LED_ON(DIO_Pin_type PIN);
LED_ERROR_TYPE LED_OFF(DIO_Pin_type PIN);





#endif /* led_H_ */