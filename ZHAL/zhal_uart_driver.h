/*
 * ZHAL UART
 *
 */

#ifndef ZHAL_UART_DRIVER_H // include guard
#define ZHAL_UART_DRIVER_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

#define ZHAL_UART_FIFO_SIZE     75

/*
 * Typedefs
 */

typedef struct {
    uint16_t BaudRate;
    void (* TxCallback) (void);
    void (* RxCallback) (void);
} ZHAL_UART_Driver_Config_t;

/*
 * Function prototypes
 */

uint8_t ZHAL_UART_Driver_Init (ZHAL_UART_Driver_Config_t * config);
void ZHAL_UART_Driver_Close ();
uint8_t ZHAL_UART_Driver_Put_Data (void * data, uint8_t bytes);
void ZHAL_UART_Driver_Send_Data ();
uint8_t ZHAL_UART_Driver_Get_Data (void * data, uint8_t bytes);
uint8_t ZHAL_UART_Driver_Peek (void * data);


#endif // ZHAL_UART_DRIVER_H
