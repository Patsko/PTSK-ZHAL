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
 * ZHAL_UART_Driver_ISR_Callback
 *
 */
static void ZHAL_UART_Driver_ISR_Callback (ZHAL_UART_ISR_Callback_Arg_t isr) {
    uint8_t data;

    switch (isr) {
    case DATA_RECEIVED:
        data = ZHAL_UART_Read(ZHAL_UART_0);
        ZHAL_FIFO_Put_Bytes(&ZHAL_UART_Rx_FIFO, &data, 1);
        ZHAL_UART_Driver_Data.ISR_NewDataFlag = 1;
        break;
    case TRANSMISSION_COMPLETE:
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Tx_FIFO, &data, 1) != 0) {
            ZHAL_UART_Send(ZHAL_UART_0, data);
        } else {
            ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag = 1;
        }
        break;
    }
}


/*
 * ZHAL_UART_Driver_Init
 */
void ZHAL_UART_Driver_Init (ZHAL_UART_Port_t port, ZHAL_UART_Config_t * config) {

    if (ZHAL_UART_Driver_Data.DriverStatus == DRIVER_UNINITIALIZED) {
        ZHAL_FIFO_Init(&ZHAL_UART_Rx_FIFO, &ZHAL_UART_Rx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);
        ZHAL_FIFO_Init(&ZHAL_UART_Tx_FIFO, &ZHAL_UART_Tx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);
        ZHAL_UART_Driver_Data.DriverStatus = DRIVER_IDLE;

        config->Callback = ZHAL_UART_Driver_ISR_Callback;
        ZHAL_UART_Config(port, config);

        ZHAL_UART_Receiver_Control (port, ENABLE);
        ZHAL_UART_Transmitter_Control (port, ENABLE);

        switch (port) {
        case ZHAL_UART_0:
            ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_RX, ZHAL_IRQ_NORMAL_PRIORITY);
            ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_TX, ZHAL_IRQ_NORMAL_PRIORITY);
            break;
        }
    }

}

/*
 * ZHAL_UART_Driver_Close
 */
void ZHAL_UART_Driver_Close (ZHAL_UART_Port_t port) {
    switch (port) {
    case ZHAL_UART_0:
        ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_RX, ZHAL_IRQ_DISABLED);
        ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_TX, ZHAL_IRQ_DISABLED);
        break;
    }

    ZHAL_UART_Receiver_Control (port, DISABLE);
    ZHAL_UART_Transmitter_Control (port, DISABLE);

    ZHAL_UART_Driver_Data.DriverStatus = DRIVER_UNINITIALIZED;
}


/*
 * ZHAL UART Driver
 */
void ZHAL_UART_Driver () {

    switch (ZHAL_UART_Driver_Data.DriverStatus) {
    case DRIVER_UNINITIALIZED:
    case DRIVER_IDLE:
        break;
    case DRIVER_IN_USE:
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
    case DRIVER_ERROR:
        break;
    default:
        ZHAL_UART_Driver_Data.DriverStatus = DRIVER_UNINITIALIZED;
        break;
    }
}
