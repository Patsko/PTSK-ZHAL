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
 * Function prototypes
 */

uint8_t ZHAL_UART_Driver_Init (uint8_t * lock_id, ZHAL_UART_Port_t port, ZHAL_UART_Config_t * config, void (* callback) (uint8_t));
void ZHAL_UART_Driver_Close (uint8_t lock_id, ZHAL_UART_Port_t port);
void ZHAL_UART_Driver();
uint8_t ZHAL_UART_Driver_Put_Data (uint8_t lock_id, void * data, uint8_t bytes);
uint8_t ZHAL_UART_Driver_Get_Data (uint8_t lock_id, void * data, uint8_t bytes);
void ZHAL_UART_Driver_Control (uint8_t lock_id, uint8_t control);
uint8_t ZHAL_UART_Driver_Peek (uint8_t lock_id, void * data);


#endif // ZHAL_UART_DRIVER_H
