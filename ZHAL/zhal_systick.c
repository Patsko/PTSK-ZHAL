/*
 * ZHAL Systick
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_systick.h"
#include "zhal_timer.h"
#include "zhal_mcu.h"



static struct {
    uint16_t Tick;
} ZHAL_Systick;


/*
 * ZHAL_Systick_Callback
 * Function that must be called from the Timer interrupt
 */
static void ZHAL_Systick_Callback (void * arg) {
    uint8_t teste;

    ZHAL_Systick.Tick++;
}


/*
 * ZHAL_Systick_Init
 */
void ZHAL_Systick_Init () {
    const ZHAL_Timer_Config_t timer_config = {
        SYSTICK_TIMER,
        TIMER_MODE_CONTINUOUS,
        (SYSTEM_CLOCK / 1000),
        0,
        TIMER_PRESCALER_1,
        0,
        (uint8_t) DISABLE,
        ZHAL_Systick_Callback,
        NULL
    };

    ZHAL_Systick.Tick = 0;

    ZHAL_Timer_Config(&timer_config);
    ZHAL_Timer_Start(SYSTICK_TIMER);
    ZHAL_Set_Interrupts(ZHAL_IRQ_TIMER0, ZHAL_IRQ_HIGH_PRIORITY);

}

/*
 * ZHAL_Systick_Get_Tick
 */
uint16_t ZHAL_Systick_Get_Tick () {

    return (ZHAL_Systick.Tick);
}


/*
 * ZHAL_Systick_Delay
 * Blocking delay
 */
void ZHAL_Systick_Delay (uint16_t ticks_to_wait) {
    uint16_t final_tick;

    final_tick = ticks_to_wait + ZHAL_Systick.Tick;

    while (final_tick != ZHAL_Systick.Tick);  // wait until final_tick is the same as ZHAL_Systick.Tick

}



