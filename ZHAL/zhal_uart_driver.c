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
 * Inits the UART driver and sets the lock id
 */
uint8_t ZHAL_UART_Driver_Init (uint8_t * lock_id, ZHAL_UART_Port_t port, ZHAL_UART_Config_t * config, void (* callback) (uint8_t)) {
    uint8_t status = 0;
    ZHAL_GPIO_Config_t gpio_config = {
        ZHAL_GPIO_INPUT,
        ZHAL_GPIO_AF1,
        DISABLE,
        DISABLE,
        DISABLE,
        DISABLE
    };

    if (ZHAL_UART_Driver_Data.DriverStatus == DRIVER_UNINITIALIZED) {
        switch (port) {
        case ZHAL_UART_0:
            ZHAL_FIFO_Init(&ZHAL_UART_Rx_FIFO, &ZHAL_UART_Rx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);
            ZHAL_FIFO_Init(&ZHAL_UART_Tx_FIFO, &ZHAL_UART_Tx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);

            ZHAL_UART_Driver_Data.DriverStatus = DRIVER_IN_USE;
            ZHAL_UART_Driver_Data.LockID++;
            *lock_id = ZHAL_UART_Driver_Data.LockID;
            ZHAL_UART_Driver_Data.ISR_NewDataFlag = 0;
            ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag = 0;
            ZHAL_UART_Driver_Data.Callback = callback;

            ZHAL_GPIO_Config_Pin(ZHAL_GPIO_A, GPIO_PIN_4 | GPIO_PIN_5, &gpio_config);

            config->Callback = ZHAL_UART_Driver_ISR_Callback;
            ZHAL_UART_Config(port, config);
            ZHAL_UART_Baud_Rate_Config (port, config, SYSTEM_CLOCK);

            ZHAL_UART_Receiver_Control (port, ENABLE);
            ZHAL_UART_Transmitter_Control (port, ENABLE);

            ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_RX, ZHAL_IRQ_NORMAL_PRIORITY);
            ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_TX, ZHAL_IRQ_NORMAL_PRIORITY);

            status = 1;
            break;
        }
    }

    return (status);
}

/*
 * ZHAL_UART_Driver_Close
 * Closes the UART and frees the driver
 */
void ZHAL_UART_Driver_Close (uint8_t lock_id, ZHAL_UART_Port_t port) {

    if (ZHAL_UART_Driver_Data.LockID == lock_id) {
        switch (port) {
        case ZHAL_UART_0:
            ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_RX, ZHAL_IRQ_DISABLED);
            ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_TX, ZHAL_IRQ_DISABLED);

            ZHAL_UART_Receiver_Control (port, DISABLE);
            ZHAL_UART_Transmitter_Control (port, DISABLE);

            ZHAL_UART_Driver_Data.DriverStatus = DRIVER_UNINITIALIZED;
            break;
        }
    }
}


/*
 * ZHAL UART Driver
 * Task/function that controls the Driver
 */
void ZHAL_UART_Driver () {

    switch (ZHAL_UART_Driver_Data.DriverStatus) {
    case DRIVER_UNINITIALIZED:
        break;
    case DRIVER_IN_USE:
        if (ZHAL_UART_Driver_Data.ISR_NewDataFlag) {
            ZHAL_UART_Driver_Data.ISR_NewDataFlag = 0;
            if (ZHAL_UART_Driver_Data.Callback != NULL) {
                (*ZHAL_UART_Driver_Data.Callback)(1);
            }
        }
        if (ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag) {
            ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag = 0;
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

/*
 * ZHAL_UART_Driver_Put_Data
 * Put data into UART Driver transmitter FIFO and returns the quantity of bytes inserted
 */
uint8_t ZHAL_UART_Driver_Put_Data (uint8_t lock_id, void * data, uint8_t bytes) {

    if (ZHAL_UART_Driver_Data.LockID == lock_id) {
        bytes = ZHAL_FIFO_Put_Bytes(&ZHAL_UART_Tx_FIFO, data, bytes);
    }
    return (bytes);
}

/*
 * ZHAL_UART_Driver_Get_Data
 * Get data from UART Driver receiver FIFO and returns the quantity of bytes get
 */
uint8_t ZHAL_UART_Driver_Get_Data (uint8_t lock_id, void * data, uint8_t bytes) {

    if (ZHAL_UART_Driver_Data.LockID == lock_id) {
        bytes = ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Rx_FIFO, data, bytes);
    } else {
        bytes = 0;
    }
    return (bytes);
}

/*
 * ZHAL_UART_Driver_Control
 *
 */
void ZHAL_UART_Driver_Control (uint8_t lock_id, uint8_t control) {
    uint8_t data;

    if (ZHAL_UART_Driver_Data.LockID == lock_id) {
        switch (control) {
        case 1:
            if (ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Tx_FIFO, &data, 1) != 0) {
                ZHAL_UART_Send(ZHAL_UART_0, data);
            } else {
                ZHAL_UART_Driver_Data.ISR_TransmissionCompleteFlag = 1;
            }
            break;
        }
    }
}
