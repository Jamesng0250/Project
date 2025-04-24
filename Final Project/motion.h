/*
 * motion.h
 *
 *  Created on: Feb 27, 2025
 *      Author: jamesnguyen
 */

#ifndef  MOTIONSENSOR_H_
#define  MOTIONSENSOR_H_
/*
 * motion.h: Header file for sensor-related functions
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>
#include <inc/hw_adc.h>
#include <inc/hw_timer.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/adc.h>
#include <driverlib/timer.h>
#include <assert.h>
#include "event.h"


// Initialize the sensor
void MotionSensorInit();

// Turn on the sensor
bool MotionSensorRead(); // change to boolean

//function for registering an event object with the motion sensor
void MotionSensorEventRegister(Event *callback_event);



#endif /* MOTIONSENSOR_H_ */
