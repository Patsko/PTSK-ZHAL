/*
 * ZHAL UART
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_uart.h"

static void (* ZHAL_Callback_fp[ZHAL_UART_MAX]) (void *);
static void * ZHAL_Callback_Arg_p[ZHAL_UART_MAX];

/*
 * ZHAL_UART_Config
 */
void ZHAL_UART_Config (ZHAL_UART_Port_t port, ZHAL_UART_Config_t * config) {
    uint8_t volatile * base_reg_p = NULL;
    uint8_t volatile * UxCTL0_p = NULL;

    switch (port) {
    case ZHAL_UART_0:
        base_reg_p = ZHAL_UART_0_BASE_REG;
        break;
    default:
        break;
    }

    if (base_reg_p != NULL) {
        UxCTL0_p = base_reg_p + 0x02;
        *UxCTL0_p = 0x00;

        switch (config->Parity) {
        case ZHAL_UART_NO_PARITY:
            break;
        case ZHAL_UART_EVEN_PARITY:
            *UxCTL0_p |= 0x10;
            break;
        case ZHAL_UART_ODD_PARITY:
            *UxCTL0_p |= 0x18;
            break;
        }

        if (config->CTS == ENABLE) {
            *UxCTL0_p |= 0x20;
        }

        if (config->StopBitSelect == ENABLE) {
            *UxCTL0_p |= 0x02;
        }

        ZHAL_Callback_fp[port] = config->Callback;
        ZHAL_Callback_Arg_p[port] = config->Callback_Arg;
    }
}


/*
 * ZHAL_UART_Receiver_Control
 */
void ZHAL_UART_Receiver_Control (ZHAL_UART_Port_t port, enable_t control) {
    uint8_t data;

    switch (port) {
    case ZHAL_UART_0:
        if (control == ENABLE) {
            U0CTL0 |= 0x40;
        } else {
            U0CTL0 &= ~(0x40);
        }
        break;
    }
}

/*
 * ZHAL_UART_Transmitter_Control
 */
void ZHAL_UART_Transmitter_Control (ZHAL_UART_Port_t port, enable_t control) {
    uint8_t data;

    switch (port) {
    case ZHAL_UART_0:
        if (control == ENABLE) {
            U0CTL0 |= 0x80;
        } else {
            U0CTL0 &= ~(0x80);
        }
        break;
    }
}


/*
 * ZHAL_UART_Read
 */
uint8_t ZHAL_UART_Read (ZHAL_UART_Port_t port) {
    uint8_t data = 0;
    
    switch (port) {
    case ZHAL_UART_0:
        data = U0RXD;
        break;
    }
    return (data);
}

/*
 * ZHAL_UART_Send
 */
void ZHAL_UART_Send (ZHAL_UART_Port_t port, uint8_t data) {
        
    switch (port) {
    case ZHAL_UART_0:
        U0TXD = data;
        break;
    }     
}



void interrupt ZHAL_UART_0_RX_ISR () _At UART0_RX {
    IRQ0 &= IRQ_UART0_RX_MASK;

    if (ZHAL_Callback_fp[ZHAL_UART_0] != NULL) {
        (*ZHAL_Callback_fp)(ZHAL_Callback_Arg_p[ZHAL_UART_0]);
    }
}


void interrupt ZHAL_UART_0_TX_ISR () _At UART0_TX {
    IRQ0 &= IRQ_UART0_TX_MASK;

    if (ZHAL_Callback_fp[ZHAL_UART_0] != NULL) {
        (*ZHAL_Callback_fp)(ZHAL_Callback_Arg_p[ZHAL_UART_0]);
    }
}
