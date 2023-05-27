/*
 * app.c
 *
 * Created: 5/16/2023 11:50:50 PM
 *  Author: Amr
 */ 
#include "app.h"




void app_sendComplete (void)
{
	LED_ON(PINA0);
}

void app_receiveComplete (void)
{
	LED_ON(PINA1);
}


void app_init()
{
		u8 arr[30]="BCM Operating";
		u8 arrx[30] = {0};


		LED_INIT(PINA0);
		LED_INIT(PINA1);
		bcm_init(ID_0);
		GLOBALE_ENABLE();
		bcm_sendComplete_callback(app_sendComplete);
		bcm_receiveComplete_callback(app_receiveComplete);
		
		bcm_send_n(arr);
		bcm_receive(arrx);
}


void app_start()
{	
	bcm_receive_dispatcher();
	bcm_send_dispatcher();
}

