/*
 * motion.c
 *
 *  Created on: Feb 27, 2025
 *      Author: jamesnguyen
 */
/******************************************************************************
 * motion.c: This file contains the initialization function for the motion sensor.
 *
 *****************************************************************************/

#include "motion.h"
#include "event.h"
#include "launchpad.h"
#include <driverlib/rom_map.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>


//MotionSensor input state
typedef struct {
    Event *callback_event; //#0 offset
    uint32_t pin_value;    // #4 offset
    bool new_input;        //#8 offset
} MotionSensorState;

MotionSensorState motion_sensor;
/******************************************************************************
 * Initialize the motion sensor
 * Pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
 *****************************************************************************/

//declare ISR function before MotionSensorInit
static void MotionSensorISR();


void MotionSensorInit()
{
    //initialize push button state
    motion_sensor.callback_event = NULL; //set to NULL
    motion_sensor.new_input = false; // set to false

    // Enable Port C peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC4 as input pin
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    //set interrupt on Port C, pin 4
    GPIOIntRegister(GPIO_PORTC_BASE, MotionSensorISR); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES); // ***interrupt on both edges, note that SW1 and SW2 are active low

    IntPrioritySet(INT_GPIOC, 0); // set interrupt level to 0 (0 is the highest for programmable interrupts)
    GPIOIntEnable(GPIO_PORTC_BASE,GPIO_PIN_4); // enable interrupts on SW1 and SW2 input
}

void MotionSensorEventRegister(Event *callback_event){
    assert(EventInitialized(callback_event)); // make sure event has properly initialized
    motion_sensor.callback_event = callback_event;// store the event callback in the motion sensor structure
}

static void MotionSensorISR()
{


    // Read Port F. SW1 and SW2 are active high
    uint32_t pin_value = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4); //pin value is 0 or not 0
    if (pin_value != motion_sensor.pin_value) { // if motion sensor pin value is not equals to pin value make it equal
        motion_sensor.pin_value = pin_value;
        motion_sensor.new_input = true;
    }

    // Schedule callback event
    if (motion_sensor.callback_event != NULL)
           EventSchedule(motion_sensor.callback_event, EventGetCurrentTime());

    // IMPORTANT: Clear interrupt flag
    GPIOIntClear(GPIO_PORTC_BASE,GPIO_PIN_4);
}

