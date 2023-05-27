/*
 * bcm.h
 *
 * Created: 25-May-23 4:11:14 AM
 *  Author: Zerconium
 */ 


#ifndef BCM_H_
#define BCM_H_

#include "../../MCAL/uart/uart_config.h"

#define OVER '&'

typedef enum
{
	system_success,
	system_fail
}enu_system_status_t;

typedef enum  
{
	ID_0,
	ID_1,
	ID_2	
}enu_mediumID_t;

typedef enum
{
	BCM_idle,
	UART_needs_to_send,
	UART_send_complete,
	UART_needs_to_receive,
	UART_receive_complete,	
	//to be added
	SPI_states,
	I2C_states
	
}enu_bcm_state_t;



enu_system_status_t bcm_init (enu_mediumID_t enu_protocol);
enu_system_status_t bcm_deinit (enu_mediumID_t enu_protocol);
void bcm_send(u8 a_u8_data);
void bcm_send_n (u8 *u8_a_ptr_data);
void bcm_receive (u8 * u8_ptr);

void bcm_send_dispatcher (void);
void bcm_receive_dispatcher (void);

void bcm_sendComplete_callback(void(*local_fptr)(void));
void bcm_receiveComplete_callback(void(*local_fptr)(void));


#endif /* BCM_H_ */