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

#define ZHAL_UART_FIFO_SIZE     50

/*
 * Typedefs
 */

typedef struct {
    bool_t IsOwner;
    uint8_t Status;
} ZHAL_UART_Driver_Handle_t;

typedef struct {
    uint16_t BaudRate;
    void (* TxCallback) (void);
    void (* RxCallback) (void);
} ZHAL_UART_Driver_Config_t;

/*
 * Function prototypes
 */

uint8_t ZHAL_UART_Driver_Init (ZHAL_UART_Driver_Handle_t * handle, ZHAL_UART_Driver_Config_t * config);
uint8_t ZHAL_UART_Driver_Close (ZHAL_UART_Driver_Handle_t * handle);
uint8_t ZHAL_UART_Driver_Put_Data (ZHAL_UART_Driver_Handle_t * handle, void * data, uint8_t bytes);
void ZHAL_UART_Driver_Send_Data (ZHAL_UART_Driver_Handle_t * handle);
uint8_t ZHAL_UART_Driver_Get_Data (ZHAL_UART_Driver_Handle_t * handle, void * data, uint8_t bytes);
uint8_t ZHAL_UART_Driver_Peek (ZHAL_UART_Driver_Handle_t * handle, void * data);


#endif // ZHAL_UART_DRIVER_H
