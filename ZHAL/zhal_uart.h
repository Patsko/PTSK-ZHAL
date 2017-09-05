/*
 * ZHAL UART
 *
 */

#ifndef ZHAL_UART_H // include guard
#define ZHAL_UART_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

#define ZHAL_UART_0_BASE_REG        (&U0D)
#define ZHAL_UART_1_BASE_REG        (&U1D)


/*
 * Typedefs
 */

typedef enum ZHAL_UART_Port {
    ZHAL_UART_0 = 0x00,
    ZHAL_UART_MAX,
} ZHAL_UART_Port_t;

typedef enum ZHAL_UART_Parity {
    ZHAL_UART_NO_PARITY = 0,
    ZHAL_UART_ODD_PARITY,
    ZHAL_UART_EVEN_PARITY,
} ZHAL_UART_Parity_t;


typedef struct ZHAL_UART_Config {
    ZHAL_UART_Parity_t Parity    :2;
    enable_t CTS                 :1;
    enable_t StopBitSelect       :1;

    void (* Callback) (void *);
    void * Callback_Arg;
} ZHAL_UART_Config_t;


/*
 * Function prototypes
 */

void ZHAL_UART_Config (ZHAL_UART_Port_t port, ZHAL_UART_Config_t * config);
void ZHAL_UART_Receiver_Control (ZHAL_UART_Port_t port, enable_t control);
void ZHAL_UART_Transmitter_Control (ZHAL_UART_Port_t port, enable_t control);
uint8_t ZHAL_UART_Read (ZHAL_UART_Port_t port) ;
void ZHAL_UART_Send (ZHAL_UART_Port_t port, uint8_t data);


#endif // ZHAL_UART_H
