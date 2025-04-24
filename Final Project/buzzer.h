/*
 * buzzer.h: Header file for buzzer-related functions
 *
<<<<<<< HEAD
 * Lab 4 starter code
 * ECE 266, Spring 2025
 *
 * Created by Zhao Zhang
 *
=======
 * ECE 266, Spring 2025
>>>>>>> 0f63c906993a21ef29b5ec8f8c3b224e791b019b
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

// Initialize the buzzer
void BuzzerInit();

// Turn on the buzzer
void BuzzerOn();

// Turn off the buzzer
void BuzzerOff();


//


#endif /* BUZZER_H_ */
