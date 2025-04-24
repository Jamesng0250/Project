;******************************************************************************
; Assembly functions related to motion sensor
;
;******************************************************************************

        ; To include names declared in C
        .cdecls "motion.h"

        .text
		.ref motion_sensor
		;read from motion_sensor state variable
_motion_sensor .field motion_sensor
;******************************************************************************
; Sensor signal pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
;******************************************************************************

; Declare a field to store GPIO_PORTC_BASE (32-bit value) in program memory


    .global MotionSensorRead

        .asmfunc
MotionSensorRead
        PUSH    {LR}            ; save the return address

        ; 1 to the GPIO pin that the sensor uses:
        ; GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4)
        LDR     r0, _motion_sensor ; load the address of motion sensor to r0
        LDRB	r1, [r0,#8] ;  move 8 bits address of r0 to r1
        CMP r1,#0; compare r1 to 0
        BEQ newinput;

        LDR r0,[r0,#4]
        POP {PC}; return

newinput
  		MOV r0,#0; move 0 to r0
  		POP {PC}

		.endasmfunc

