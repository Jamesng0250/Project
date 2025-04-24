
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <assert.h>
#include "launchpad.h"
#include "seg7.h"
#include "motion.h"
#include "buzzer.h" 
#include <stdbool.h>
#include "buzzer.h"


/********************************************************************************
 * Global declarations
 ******************************************************************************/
static uint8_t motion_count = 0;
static int timer = 0;
static int press = 3;


Seg7Display seg7 = { { 0, 0, 0, 0 }, true }; // display 00:00 on seg 7



// The data structure for the system state
typedef struct {
    bool activated;
    bool alerted;
} SysState;

// The system state
static SysState sys =
{
    false /* not activated */,
    false /* not alerted */,
};
Event check_push_button_event;
Event motion_check_event;
Event clock_update_event;





void ClockUpdate(Event *event)
{

    static int count = 0;
    count++;
    if (count == 2){
    count =0;

           if(press == 3){
               LedTurnOnOff(true /* red */, false /* blue */, false /* green */);
           }
           else if (press == 2){
               LedTurnOnOff(false /* red */, true /* blue */, false /* green */);
           }
           else if (press == 1){
               LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
           }
           else if (press == 0){
               LedTurnOnOff(false /* red */, false /* blue */, false /* green */);
           }
           press--;


                   if(press<=0 && timer > 0){
                    timer--;
                        if (timer == 0){
                            uprintf("Timer finished\n");
                            LedTurnOnOff(true /* red */, false /* blue */, false /* green */);


                            //Beep when timer hits 0

                            BuzzerOn();
                            SysCtlDelay(SysCtlClockGet() / 3 / 10);  // ~100 ms
                            BuzzerOff();
                        }
                       }
                   }
    seg7.digit[1] = timer / 10;
    seg7.digit[0] = timer % 10;
    // Update the 7-segment
    Seg7Update(&seg7);
    // Schedule a callback event after 0.5 seconds
    EventSchedule(event, event->time + 500);
}




void MotionCheck(Event *event)
{
    static int last_motion = 0;    // Store the previous motion sensor reading
    uint32_t delay = 1000;         // check every second
    int motion = MotionSensorRead();

    if (motion != 0 && last_motion == 0 && sys.activated == true)
    {
        uprintf("Motion detected!\r\n");
        LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
        motion_count++;  // Increment only on rising edge (0 to 1)


        //buzzer beep if motion detected

        BuzzerOn();
        SysCtlDelay(SysCtlClockGet() / 3 / 20);  // ~50 ms
        BuzzerOff();
    }
    else if (motion == 0 && sys.activated == true)
    {
        LedTurnOnOff(false /* red */, false /* blue */, false /* green */);
    }

    last_motion = motion; // Update for next check

    seg7.digit[3] = motion_count / 10;
    seg7.digit[2] = motion_count % 10;
    Seg7Update(&seg7);

    EventSchedule(event, event->time + delay);
}

void CheckPushButton(Event *event)
{

    int code = PushButtonRead();        // read the pushbutton
    int motion = MotionSensorRead();      // get 0 or 1 value and store in Read

    switch (code)
    {
    case 1:                     // SW1 is pushed
           sys.activated = true;
           timer +=5;
           press = 4;
           LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
           printf("System Active\n");

        break;

    case 2:                     // SW2 is pushed

            sys.activated = false;
            timer = 0;
            motion_count = 0;

            LedTurnOnOff(false /* red */, false /* blue */, false /* green */);
            uprintf("System reset\n");

        break;
    }
    Seg7Update(&seg7);
    EventSchedule(event, event->time + 500);
}

int main(void)
{
    // Initialize the Tiva C LaunchPad and 7-seg
    LaunchPadInit();
    MotionSensorInit();
    Seg7Init();
    BuzzerInit();

    uprintf("%s\n\r", "");

    // Initialize the event objects
    EventInit(&check_push_button_event, CheckPushButton);
    EventInit(&motion_check_event, MotionCheck);
    EventInit(&clock_update_event, ClockUpdate);
    EventSchedule(&clock_update_event, 1000);
    // Register ISR event
    PushButtonEventRegister(&check_push_button_event);

    // Schedule time events
    EventSchedule(&motion_check_event, 1000);

    // Run the callback scheduler
    while (true)
    {
        // Wait for interrupt events (include callback events)
        asm("   wfi");

        // Run callback events
        EventExecute();
    }
}
