/*
 * UART_ReusableDriver.c
 *
 * Created: 4/10/2020 5:29:04 AM
 *  Author: Hamdy
 */ 

#include "uc_Configuration.h"
#include <avr/io.h>
#include <util/delay.h>
#include "MP3_Module.h"



int main(void)
{
	Mp3_Init();
	Mp3_SelectVolume(30);
	Mp3_PlayTrack(2);

	
    while(1)
    {

    }
}
