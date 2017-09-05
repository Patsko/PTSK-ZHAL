/*
 * ZHAL Timer
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_timer.h"
#include "zhal_mcu.h"


static void (* ZHAL_Callback_fp[ZHAL_TIMER_MAX]) (void *);
static void * ZHAL_Callback_Arg_p[ZHAL_TIMER_MAX];

/*
 * ZHAL_Timer_Stop
 */
void ZHAL_Timer_Stop (ZHAL_Timer_t timer) {

    switch (timer) {
    case ZHAL_TIMER_0:
        T0CTL1 &= 0x7F;
        break;
    case ZHAL_TIMER_1:
        T1CTL1 &= 0x7F;
        break;
    case ZHAL_TIMER_2:
        T2CTL1 &= 0x7F;
        break;
    }
}

/*
 * ZHAL_Timer_Start
 */
void ZHAL_Timer_Start (ZHAL_Timer_t timer) {

    switch (timer) {
    case ZHAL_TIMER_0:
        T0CTL1 |= 0x80;
        break;
    case ZHAL_TIMER_1:
        T1CTL1 |= 0x80;
        break;
    case ZHAL_TIMER_2:
        T2CTL1 |= 0x80;
        break;
    }
}


/*
 * ZHAL_Timer_Config
 */
void ZHAL_Timer_Config (ZHAL_Timer_t timer, ZHAL_Timer_Config_t * config) {
    uint8_t volatile * TxCTL0_p = NULL;
    uint8_t volatile * TxCTL1_p = NULL;
    uint8_t volatile * TxCTL2_p = NULL;
    uint16_t volatile * Tx = NULL;
    uint16_t volatile * TxR = NULL;
    uint16_t volatile * TxPWM0 = NULL;
    uint8_t aux = 0;

    switch (timer) {
    case ZHAL_TIMER_0:
        TxCTL0_p = &T0CTL0;
        TxCTL1_p = &T0CTL1;
        TxCTL2_p = &T0CTL2;
        Tx = &T0;
        TxR = &T0R;
        TxPWM0 = &T0PWM0;
        break;
    case ZHAL_TIMER_1:
        TxCTL0_p = &T1CTL0;
        TxCTL1_p = &T1CTL1;
        TxCTL2_p = &T1CTL2;
        Tx = &T1;
        TxR = &T1R;
        TxPWM0 = &T1PWM0;
        break;
    case ZHAL_TIMER_2:
        TxCTL0_p = &T2CTL0;
        TxCTL1_p = &T2CTL1;
        TxCTL2_p = &T2CTL2;
        Tx = &T2;
        TxR = &T2R;
        TxPWM0 = &T2PWM0;
        break;
    }

    if (TxCTL0_p != NULL) {
        *TxCTL0_p = 0;
        *TxCTL1_p = 0;
        *TxCTL2_p = 0;

        switch (config->Mode) {
        case TIMER_MODE_ONESHOT:
            break;
        case TIMER_MODE_CONTINUOUS:
            *TxCTL1_p |= 0x01;
            break;
        case TIMER_MODE_COUNTER:
            *TxCTL1_p |= 0x02;
            break;
        case TIMER_MODE_PWM_SINGLE_OUTPUT:
            *TxCTL1_p |= 0x03;
            break;
        case TIMER_MODE_CAPTURE:
            *TxCTL1_p |= 0x04;
            break;
        case TIMER_MODE_COMPARE:
            *TxCTL1_p |= 0x05;
            break;
        case TIMER_MODE_GATED:
            *TxCTL1_p |= 0x06;
            break;
        case TIMER_MODE_CAPTURE_COMPARE:
            *TxCTL1_p |= 0x07;
            break;
        case TIMER_MODE_PWM_DUAL_OUTPUT:
            *TxCTL0_p |= 0x80;
            break;
        case TIMER_MODE_CAPTURE_RESTART:
            *TxCTL0_p |= 0x80;
            *TxCTL1_p |= 0x01;
            break;
        case TIMER_MODE_COMPARATOR_COUNTER:
            *TxCTL0_p |= 0x80;
            *TxCTL1_p |= 0x02;
            break;
        case TIMER_MODE_TRIGGERED_ONESHOT:
            *TxCTL0_p |= 0x80;
            *TxCTL1_p |= 0x03;
            break;
        case TIMER_MODE_DEMODULATION:
            *TxCTL0_p |= 0x80;
            *TxCTL1_p |= 0x04;
            break;
        }

        *TxR = config->Reload;
        *TxPWM0 = config->PWM;

        if (config->Prescaler <= TIMER_PRESCALER_128) {
            aux = (config->Prescaler << 3);
            *TxCTL1_p |= aux;
        }

        if (config->Polarity == 1) {
            *TxCTL1_p |= 0x40;
        }

        if (config->PWM_Update == 1) {
            *TxCTL2_p |= 0x20;
        }

        ZHAL_Callback_fp[timer] = config->Callback;
        ZHAL_Callback_Arg_p[timer] = config->Callback_Arg;
    }
}


void interrupt ZHAL_Timer_0_ISR () _At TIMER0 {
    IRQ0 &= IRQ_TIMER0_MASK;

    if (ZHAL_Callback_fp[ZHAL_TIMER_0] != NULL) {
        (*ZHAL_Callback_fp)(ZHAL_Callback_Arg_p[ZHAL_TIMER_0]);
    }
}


void interrupt ZHAL_Timer_1_ISR () _At TIMER1 {
    IRQ0 &= IRQ_TIMER1_MASK;

    if (ZHAL_Callback_fp[ZHAL_TIMER_1] != NULL) {
        (*ZHAL_Callback_fp)(ZHAL_Callback_Arg_p[ZHAL_TIMER_1]);
    }
}


void interrupt ZHAL_Timer_2_ISR () _At TIMER2 {
    IRQ0 &= IRQ_TIMER2_MASK;

    if (ZHAL_Callback_fp[ZHAL_TIMER_2] != NULL) {
        (*ZHAL_Callback_fp)(ZHAL_Callback_Arg_p[ZHAL_TIMER_2]);
    }
}


