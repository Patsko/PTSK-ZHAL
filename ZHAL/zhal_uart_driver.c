/*
 * ZHAL UART
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_uart.h"
#include "zhal_uart_driver.h"
#include "zhal_drivers.h"

static ZHAL_Driver_t ZHAL_UART_Driver_Data;
static ZHAL_FIFO_t ZHAL_UART_Rx_FIFO;
static ZHAL_FIFO_t ZHAL_UART_Tx_FIFO;
static uint8_t ZHAL_UART_Rx_FIFO_Buffer[ZHAL_UART_FIFO_SIZE];
static uint8_t ZHAL_UART_Tx_FIFO_Buffer[ZHAL_UART_FIFO_SIZE];

/*
 * ZHAL UART Driver
 */
void ZHAL_UART_Driver () {

    switch (ZHAL_UART_Driver_Data.DriverStatus) {
    case 0:
        ZHAL_FIFO_Init(&ZHAL_UART_Rx_FIFO, &ZHAL_UART_Rx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);
        ZHAL_FIFO_Init(&ZHAL_UART_Tx_FIFO, &ZHAL_UART_Tx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);
        break;
    case 1:
        if (ZHAL_UART_Driver_Data.ISR_NewDataFlag) {
            if (ZHAL_UART_Driver_Data.Callback != NULL) {
                (*ZHAL_UART_Driver_Data.Callback)(1);
            }
        }
        if (ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag) {
            if (ZHAL_UART_Driver_Data.Callback != NULL) {
                (*ZHAL_UART_Driver_Data.Callback)(2);
            }
        }
        break;
    case 2:


        break;
    default:

        break;
    }
}

/*
 * ZHAL_UART_Driver_ISR_Callback
 *
 */
void ZHAL_UART_Driver_ISR_Callback (uint8_t isr) {
    uint8_t data;

    if (isr == 1) {
        data = ZHAL_UART_Read(ZHAL_UART_0);
        ZHAL_FIFO_Put_Bytes(&ZHAL_UART_Rx_FIFO, &data, 1);
        ZHAL_UART_Driver_Data.ISR_NewDataFlag = 1;
    } else if (isr == 2) {
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Tx_FIFO, &data, 1) != 0) {
            ZHAL_UART_Send(ZHAL_UART_0, data);
        } else {
            ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag = 1;
        }
    }
}
