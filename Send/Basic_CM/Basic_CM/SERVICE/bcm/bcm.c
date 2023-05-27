/*
 * bcm.c
 *
 * Created: 25-May-23 4:11:03 AM
 *  Author: Amr
 */ 

#include "bcm.h"

static u8 u8_send_flag=0, u8_receive_flag=0;
u8 *arr_ptrs_send [100] = {0};
u8 *arr_ptrs_receive [100] = {0};
enu_bcm_state_t bcm_send_state = BCM_idle;
enu_bcm_state_t bcm_receive_state = BCM_idle ;


static void(*bcm_TXC_fptr)(void) = NULLPTR;
static void(*bcm_RXC_fptr)(void) = NULLPTR;



void bcm_sendComplete_callback(void(*local_fptr)(void))
{
	bcm_TXC_fptr = local_fptr;
}
void bcm_receiveComplete_callback(void(*local_fptr)(void))
{
	bcm_RXC_fptr = local_fptr;
}

enu_system_status_t bcm_init (enu_mediumID_t mediumID)
{
	enu_system_status_t system_status = system_success;
	switch (mediumID)
	{
		case ID_0:
		{
			uart_init(&g_str_uart_config);
			break;		
		}
		case ID_1:
		//to be added
		break;
		case ID_2:
		//to be added
		break;
		default:
		{
			system_status = system_fail ;
			break;
		}
	}
	return system_status;
}

enu_system_status_t bcm_deinit(enu_mediumID_t mediumID)
{
	enu_system_status_t system_status = system_success;
	switch (mediumID)
	{
		case ID_0:
		{
			bcm_send_state = BCM_idle;
			bcm_receive_state = BCM_idle;
			uart_RX_SetCallBack(NULLPTR);
			uart_TX_SetCallBack(NULLPTR);
			uart_TX_intDisable();
			uart_RX_intDisable();
		}
		
		
		break;
		case ID_1:
		//to be added
		break;
		case ID_2:
		//to be added
		break;
		default:
		system_status = system_fail ;
	}
	return system_status;
}

/*****************************************************Sending*********************************************************************/

void bcm_send_dispatcher (void)
{
	
	static u8 index_i =1, index_j=0 ;
	if (bcm_send_state == UART_needs_to_send)
	{
			/* check that the element being sent exists */
		if(arr_ptrs_send[index_j][index_i] != 0)				
		{
			uart_transmit(arr_ptrs_send[index_j][index_i]);
			index_i++;
		}
	
		/* Check if the next element in queue exists? and Terminate the sent string */
		else if (arr_ptrs_send[index_j+1] != 0)
		{
			uart_transmit(OVER);
			index_i = 0;
			index_j++;
		}

		/* Terminate the sent string and reset the sending parameters */
		else
		{
			uart_transmit(OVER);
			index_i = 1;
			u8_send_flag = 0;
			uart_TX_intDisable();
			if (bcm_TXC_fptr != NULLPTR)
			{
				bcm_TXC_fptr();
			}
			else
			{
				//do nothing
			}
		}
		bcm_send_state = UART_send_complete;
	}
	
	
}

void bcm_TX_ISR (void)
{
	bcm_send_state = UART_needs_to_send;
}

void bcm_send_n (u8 *u8_a_ptr_data)
{
	static u8 index_i =0 ;
	
	/* Check if it's the first send or no */
	if (u8_send_flag == 0)
	{
		uart_TX_intEnable();
		uart_transmit(u8_a_ptr_data[index_i]);		
		arr_ptrs_send[index_i] = u8_a_ptr_data;
		uart_TX_SetCallBack(bcm_TX_ISR);
		index_i++ ;
		u8_send_flag = 1;
	}
	else
	{
		arr_ptrs_send[index_i] = u8_a_ptr_data;
		index_i++ ;
	}
}


void bcm_send(u8 a_u8_data)
{
	uart_transmit(a_u8_data);
}

/*****************************************************Receiving*********************************************************************/

void bcm_RX_ISR (void)
{
	bcm_receive_state = UART_needs_to_receive;
}

void bcm_receive_dispatcher (void)
{
	static u8 index_i =0, index_j=0 ;
	
	if (bcm_receive_state == UART_needs_to_receive)
	{
			/* Start receiving*/
		arr_ptrs_receive[index_j][index_i] = uart_receive();
	
		/* Check if the string is terminated? */
		if (arr_ptrs_receive[index_j][index_i] != OVER)
		{	
			index_i++;
		}
		else if (arr_ptrs_receive[index_j][index_i] == OVER)
		{
			index_j++;
			index_i = 0;
		
		
			
		/*receiving is done so reset receiving parameters */
			uart_RX_intDisable();
			if (bcm_RXC_fptr != NULLPTR)
			{
				bcm_RXC_fptr();
			}
			else
			{
				//do nothing
			}
		}
		bcm_receive_state = UART_receive_complete;	
	}
		
}

void bcm_receive (u8 * u8_ptr)
{
	static u8 index_j =0;
	
	/* Check if the it's the first receive or not */
	if (u8_receive_flag == 0)
	{
		uart_RX_intEnable();
		arr_ptrs_receive[index_j] = u8_ptr;
		uart_RX_SetCallBack(bcm_RX_ISR);
		index_j++;
		
	}
	else
	{
		arr_ptrs_receive[index_j] = u8_ptr;
		index_j++;
	}
}


