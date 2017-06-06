/*
 * ZHAL Timer
 *
 */

#ifndef ZHAL_TIMER_H // include guard
#define ZHAL_TIMER_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

/*
 * Typedefs
 */

typedef enum ZHAL_Timer {
    ZHAL_TIMER_0 = 0,
    ZHAL_TIMER_1,
    ZHAL_TIMER_2,
    ZHAL_TIMER_MAX,
} ZHAL_Timer_t;

typedef enum ZHAL_Timer_Mode {
    TIMER_MODE_ONESHOT = 0,
    TIMER_MODE_CONTINUOUS,
    TIMER_MODE_COUNTER,
    TIMER_MODE_PWM_SINGLE_OUTPUT,
    TIMER_MODE_CAPTURE,
    TIMER_MODE_COMPARE,
    TIMER_MODE_GATED,
    TIMER_MODE_CAPTURE_COMPARE,
    TIMER_MODE_PWM_DUAL_OUTPUT,
    TIMER_MODE_CAPTURE_RESTART,
    TIMER_MODE_COMPARATOR_COUNTER,
    TIMER_MODE_TRIGGERED_ONESHOT,
    TIMER_MODE_DEMODULATION,
} ZHAL_Timer_Mode_t;

typedef enum ZHAL_Timer_Prescaler {
    TIMER_PRESCALER_1 = 0,
    TIMER_PRESCALER_2,
    TIMER_PRESCALER_4,
    TIMER_PRESCALER_8,
    TIMER_PRESCALER_16,
    TIMER_PRESCALER_32,
    TIMER_PRESCALER_64,
    TIMER_PRESCALER_128,
} ZHAL_Timer_Prescaler_t;

typedef struct ZHAL_Timer_Config {
    ZHAL_Timer_t Timer;
    ZHAL_Timer_Mode_t Mode;
    uint16_t Reload;
    uint16_t PWM;
    uint8_t Prescaler;
    uint8_t Polarity;
    uint8_t PWM_Update;
    void (* Callback) (void *);
    void * Callback_Arg;
} ZHAL_Timer_Config_t;


/*
 * Function prototypes
 */

void ZHAL_Timer_Stop (ZHAL_Timer_t timer);
void ZHAL_Timer_Start (ZHAL_Timer_t timer);
void ZHAL_Timer_Config (ZHAL_Timer_Config_t * config);


#endif // ZHAL_TIMER_H
