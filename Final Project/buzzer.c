/******************************************************************************
<<<<<<< HEAD
 * Lab 4 starter code
 * ECE 266, Spring 2025
 *
 * buzzer.c: This file contains the initialization function for the buzzer.
 *
 * Created by Zhao Zhang
=======
 * ECE 266, Spring 2025
 *
 * buzzer.c: This file contains the initialization function for the buzzer.

>>>>>>> 0f63c906993a21ef29b5ec8f8c3b224e791b019b
 *****************************************************************************/

#include "buzzer.h"

/******************************************************************************
 * Initialize the buzzer
 * Pin usage: Grove base port J17, Tiva C PC5 (Port C, Pin 5)
 *****************************************************************************/
void BuzzerInit()
{
    // Enable Port C peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC5 as output pin
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
}
