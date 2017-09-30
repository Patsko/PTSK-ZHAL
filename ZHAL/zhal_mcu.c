/*
 * ZHAL MCU
 * MCU specifics
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_mcu.h"

/*
 * ZHAL_Set_Interrupt
 * Define interrupt priority
 */
void ZHAL_Set_Interrupts (ZHAL_IRQ_t interr, ZHAL_IRQ_Priority_t priority) {
    uint8_t volatile * IRQxENH = NULL;
    uint8_t volatile * IRQxENL = NULL;
    uint8_t volatile * IRQx = NULL;
    uint8_t mask = 0;

    DI();

    switch (interr) {
    case ZHAL_IRQ_TIMER2:
    case ZHAL_IRQ_TIMER1:
    case ZHAL_IRQ_TIMER0:
    case ZHAL_IRQ_UART0_RX:
    case ZHAL_IRQ_UART0_TX:
    case ZHAL_IRQ_I2C:
    case ZHAL_IRQ_SPI:
    case ZHAL_IRQ_ADC:
        mask = 0x80 >> interr;
        IRQxENH = &IRQ0ENH;
        IRQxENL = &IRQ0ENL;
        IRQx = &IRQ0;
        break;
    case ZHAL_IRQ_PA7_LV:
    case ZHAL_IRQ_PA6_C1:
    case ZHAL_IRQ_PA5_C0:
    case ZHAL_IRQ_PA4_PD4:
    case ZHAL_IRQ_PA3_PD3:
    case ZHAL_IRQ_PA2_PD2:
    case ZHAL_IRQ_PA1_PD1:
    case ZHAL_IRQ_PA0:
        interr -= ZHAL_IRQ_PA7_LV;
        mask = 0x80 >> interr;
        IRQxENH = &IRQ1ENH;
        IRQxENL = &IRQ1ENL;
        IRQx = &IRQ1;
        break;
    case ZHAL_IRQ_MCT:
    case ZHAL_IRQ_UART1_RX:
    case ZHAL_IRQ_UART1_TX:
    case ZHAL_IRQ_PC3:
    case ZHAL_IRQ_PC2:
    case ZHAL_IRQ_PC1:
    case ZHAL_IRQ_PC0:
        interr -= ZHAL_IRQ_MCT;
        mask = 0x80 >> interr;
        IRQxENH = &IRQ2ENH;
        IRQxENL = &IRQ2ENL;
        IRQx = &IRQ2;
        break;
    }

    if ((IRQxENH != NULL) && (IRQxENL != NULL)) {
        switch (priority) {
        case ZHAL_IRQ_DISABLED:
            *IRQxENL &= (~mask);
            *IRQxENH &= (~mask);
            break;
        case ZHAL_IRQ_LOW_PRIORITY:
            *IRQxENL |= mask;
            *IRQxENH &= (~mask);
            break;
        case ZHAL_IRQ_NORMAL_PRIORITY:
            *IRQxENL &= (~mask);
            *IRQxENH |= mask;
            break;
        case ZHAL_IRQ_HIGH_PRIORITY:
            *IRQxENL |= mask;
            *IRQxENH |= mask;
            break;
        }

        // clears any pending interrupt
        *IRQx &= (~mask);
    }

    EI();
}


/*
 * ZHAL_Clear_Interrupt
 */
void ZHAL_Clear_Interrupt (ZHAL_IRQ_t interr) {
    uint8_t volatile * IRQx = NULL;
    uint8_t mask = 0;

    switch (interr) {
    case ZHAL_IRQ_TIMER2:
    case ZHAL_IRQ_TIMER1:
    case ZHAL_IRQ_TIMER0:
    case ZHAL_IRQ_UART0_RX:
    case ZHAL_IRQ_UART0_TX:
    case ZHAL_IRQ_I2C:
    case ZHAL_IRQ_SPI:
    case ZHAL_IRQ_ADC:
        mask = 0x80 >> interr;
        IRQx = &IRQ0;
        break;
    case ZHAL_IRQ_PA7_LV:
    case ZHAL_IRQ_PA6_C1:
    case ZHAL_IRQ_PA5_C0:
    case ZHAL_IRQ_PA4_PD4:
    case ZHAL_IRQ_PA3_PD3:
    case ZHAL_IRQ_PA2_PD2:
    case ZHAL_IRQ_PA1_PD1:
    case ZHAL_IRQ_PA0:
        interr -= ZHAL_IRQ_PA7_LV;
        mask = 0x80 >> interr;
        IRQx = &IRQ1;
        break;
    case ZHAL_IRQ_MCT:
    case ZHAL_IRQ_UART1_RX:
    case ZHAL_IRQ_UART1_TX:
    case ZHAL_IRQ_PC3:
    case ZHAL_IRQ_PC2:
    case ZHAL_IRQ_PC1:
    case ZHAL_IRQ_PC0:
        interr -= ZHAL_IRQ_MCT;
        mask = 0x80 >> interr;
        IRQx = &IRQ2;
        break;
    }

    if (IRQx != NULL) {
        // clears any pending interrupt
        *IRQx &= (~mask);
    }
}

