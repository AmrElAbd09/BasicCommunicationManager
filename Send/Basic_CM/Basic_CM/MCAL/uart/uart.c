/*
 * uart.h
 *
 * Created: 11/5/2023 10:34:00 AM
 *  Author: AMR
 */ 

	
	
#include "uart.h"

volatile u8 u8_g_receive_arr[20];
volatile u8 *u8_g_send_ptr;
static u8 *u8_g_receive_ptr;
static u8 u8_flag;
static u8 *arr_ptrs [100] = {0};
	 
	 
static void(*uart_RX_fptr)(void) = NULLPTR;
static void(*uart_TX_fptr)(void) = NULLPTR;


enu_return_error_t uart_init(str_uart_config_t* ptr_str_uart_config)
{
	u16 baud;
	//Enable writing to UCSRC
	SET_BIT(UCSRC,URSEL);
	enu_return_error_t retval;

	if (ptr_str_uart_config != NULLPTR)
	{
			switch (ptr_str_uart_config ->enu_role_config)
			{
				case transmit_enable:
				{
					SET_BIT(UCSRB,TXEN);
					break;
				}
				case receive_enable:
				{
					SET_BIT(UCSRB,RXEN);
					break;
				}
				case transmit_receive_enable:
				{
					SET_BIT(UCSRB,RXEN);
					SET_BIT(UCSRB,TXEN);
					break;
				}
				default:
				{
					retval = invalid;
				}
			}	
				
			//speed config
			switch (ptr_str_uart_config ->enu_speed_config)
			{
				case normal_speed:
				{
					CLR_BIT(UCSRA, U2X);
					baud = 51;
					UBRRL = (u8)baud;
					UBRRH = (u8)(baud >> 8);
					break;
				}
				case double_speed:
				{
					SET_BIT(UCSRA, U2X);
					baud = 0;
					UBRRL = (u8)baud;
					UBRRH = (u8)(baud >> 8);
					break;
				}
				default:
				{
					retval = invalid;
				}
			}
		
			//parity config
			switch (ptr_str_uart_config -> enu_parity_config)
			{
				case no_parity:
				{
					CLR_BIT(UCSRC,UPM0);
					CLR_BIT(UCSRC,UPM1);
					break;
				}
				case even_parity:
				{
					CLR_BIT(UCSRC,UPM0);
					SET_BIT(UCSRC,UPM1);
					break;
				}
				case odd_parity:
				{
					SET_BIT(UCSRC,UPM0);
					SET_BIT(UCSRC,UPM1);
					break;
				}
				default:
				{
					retval = invalid;
				}
			}
			
			//Sync config		
			switch (ptr_str_uart_config -> enu_mode_config)
			{
				case synchronous:
				{
					SET_BIT(UCSRC, UMSEL);
					break;
				}
				case asynchronous:
				{
					CLR_BIT(UCSRC, UMSEL);
					break;
				}
				default:
				{
					retval = invalid;
				}
			}
			
			//Stop bits config
			switch (ptr_str_uart_config -> enu_stop_bits_config)
			{
				case one_stop_bit:
				{
					CLR_BIT(UCSRC, USBS);
					break;
				}
				case two_stop_bit:
				{
					SET_BIT(UCSRC, USBS);
					break;
				}
				default:
				{
					retval = invalid;
				}
			}
			
			//data bits config
			switch (ptr_str_uart_config ->enu_data_size_config)
			{
				case _5_data_bits:
				{
					CLR_BIT(UCSRC, UCSZ0);
					CLR_BIT(UCSRC, UCSZ1);
					CLR_BIT(UCSRB, UCSZ2);
					break;
				}
				case _6_data_bits:
				{
					SET_BIT(UCSRC, UCSZ0);
					CLR_BIT(UCSRC, UCSZ1);
					CLR_BIT(UCSRB, UCSZ2);
					break;
				}
				case _7_data_bits:
				{
					CLR_BIT(UCSRC, UCSZ0);
					SET_BIT(UCSRC, UCSZ1);
					CLR_BIT(UCSRB, UCSZ2);
					break;
				}
				case _8_data_bits:
				{
					SET_BIT(UCSRC, UCSZ0);
					SET_BIT(UCSRC, UCSZ1);
					CLR_BIT(UCSRB, UCSZ2);
					break;
				}
				case _9_data_bits:
				{
					SET_BIT(UCSRC, UCSZ0);
					SET_BIT(UCSRC, UCSZ1);
					SET_BIT(UCSRB, UCSZ2);
					break;
				}
				default:
				{
					retval = invalid;
				}
			}	
	}
	
	else					//Null pointer returns failed!
	{
		retval = failed;			
	}
	return retval;
}

/****************************************************receive*******************************************************/

void uart_RX_intEnable (void)
{
	SET_BIT(UCSRB,RXCIE);
}
void uart_RX_intDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}


ISR(UART_RXC_vect)
{
	if (uart_RX_fptr != NULLPTR)
	{
		uart_RX_fptr();
	}
}


// Receiving Frames with 5 to 8 Data Bits
u8 uart_receive(void)
{
	return UDR;		
}

u8 uart_receive_string (u8 *string) 
{
	u8_g_receive_ptr = string;
}

void uart_RX_SetCallBack (void(*local_fptr)(void))
{
	uart_RX_fptr = local_fptr;
}


/***************************************************send**********************************************************/


ISR(UART_TX_vect)
{
	if (uart_TX_fptr != NULLPTR)
	{
		uart_TX_fptr();
	}
}

void uart_TX_intEnable (void)
{
	SET_BIT(UCSRB,TXCIE);
}
void uart_TX_intDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}
	
void uart_TX_SetCallBack (void(*local_fptr)(void))
{
	uart_TX_fptr = local_fptr;
}	

// Data Transmission Sending Frames with 5 to 8 Data Bit
void uart_transmit( u8 data )
{										// Put data into buffer, sends the data
	UDR = data;	
}


/*
void uart_Transmit_string (u8 * str)
{
	u8 u8_local_index = 0;
	if (u8_flag == 0)
	{
		arr_ptrs[u8_local_index] = str;
		uart_transmit(str[0]);
		u8_flag = 1;
		u8_local_index++;
	}
	else
	{
		arr_ptrs[u8_local_index];
		u8_local_index++;
	}
	
}*/




