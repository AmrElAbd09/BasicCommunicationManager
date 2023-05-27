/*
 * uart.h
 *
 * Created: 11/5/2023 10:33:46 AM
 *  Author: Amr
 */ 


#ifndef UART_H_
#define UART_H_

// Includes

#include "../dio/dio.h"

extern volatile u8 u8_g_receive_arr[20];

// register bits config
#define  RXEN  4
#define  TXEN  3
#define  URSEL 7
#define  UCSZ0 1
#define  UDRE  5
#define  RXC   7
#define  UCSZ1 2

#define USBS	3
#define UPM0	4
#define UPM1	5
#define UMSEL	6

#define UCSZ0	1
#define UCSZ1	2
#define UCSZ2	2

#define U2X		1

#define RXCIE	7
#define TXCIE	6
#define UDRIE	5




typedef enum{
	normal_speed =0,
	double_speed,
	total_speed
}enu_speed_mode_selector_t;



typedef enum{
	transmit_enable =0,
	receive_enable,
	transmit_receive_enable,
	total_enable
}enu_role_selector_t;

typedef enum{
	no_parity =0,
	even_parity,
	odd_parity,
	total_parity
}enu_parity_mode_selector_t;


typedef enum{
	synchronous =0,
	asynchronous,
	total_synch
}enu_sync_mode_selector_t;


typedef enum{
	one_stop_bit =0,
	two_stop_bit,
	total_stop
}enu_number_stopBits_selector_t;


typedef enum{
	_5_data_bits =0,
	_6_data_bits,
	_7_data_bits,
	_8_data_bits,
	_9_data_bits,
	total_bits
}enu_number_dataBits_selector_t;


typedef struct
{
	enu_speed_mode_selector_t			    enu_speed_config;
	enu_role_selector_t						enu_role_config;
	enu_parity_mode_selector_t			    enu_parity_config;
	enu_sync_mode_selector_t			    enu_mode_config;
	enu_number_stopBits_selector_t		    enu_stop_bits_config;
	enu_number_dataBits_selector_t		    enu_data_size_config;
}str_uart_config_t;

typedef enum{
	valid =0,
	invalid,
	failed
}enu_return_error_t;


// Functions prototypes
enu_return_error_t uart_init(str_uart_config_t* ptr_str_uart_config);
void uart_transmit(u8 data);
u8 uart_receive(void);
void uart_Transmit_string (u8 * str);
u8 uart_receive_string (u8 *string);

void uart_RX_SetCallBack (void(*local_fptr)(void));
void uart_TX_SetCallBack (void(*local_fptr)(void));

void uart_RX_intEnable (void);
void uart_RX_intDisable(void);

void uart_TX_intEnable (void);
void uart_TX_intDisable(void);

#endif /* UART_H_ */

