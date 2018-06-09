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

typedef enum {
    UART_TX_IDLE = 0,
    UART_TX_IN_PROGRESS
} UART_Tx_Status;


static struct {
    uint16_t BaudRate;
    void (* TxCallback) (void);
    void (* RxCallback) (void);
    UART_Tx_Status TxStatus;
} ZHAL_UART_Driver_Data;

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
        if (ZHAL_UART_Driver_Data.RxCallback != NULL) {
            (*ZHAL_UART_Driver_Data.RxCallback)();
        }
        break;
    case TRANSMISSION_COMPLETE:
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Tx_FIFO, &data, 1) != 0) {
            ZHAL_UART_Send(ZHAL_UART_0, data);
        } else {
            ZHAL_UART_Driver_Data.TxStatus = UART_TX_IDLE;
            if (ZHAL_UART_Driver_Data.TxCallback != NULL) {
                (*ZHAL_UART_Driver_Data.TxCallback)();
            }
        }
        break;
    }
}


/*
 * ZHAL_UART_Driver_Init
 * Inits the UART driver and sets the lock id
 */
uint8_t ZHAL_UART_Driver_Init (ZHAL_UART_Driver_Config_t * config) {
    uint8_t status = 0;
    ZHAL_GPIO_Config_t gpio_config = {
        ZHAL_GPIO_INPUT,
        ZHAL_GPIO_AF1,
        DISABLE,
        DISABLE,
        DISABLE,
        DISABLE
    };
    ZHAL_UART_Config_t uart_config = {
        ZHAL_UART_NO_PARITY,
        DISABLE,
        DISABLE,
        9600,
        ZHAL_UART_Driver_ISR_Callback
    };

    if (config != NULL) {
        ZHAL_UART_Driver_Data.RxCallback = config->RxCallback;
        ZHAL_UART_Driver_Data.TxCallback = config->TxCallback;
        ZHAL_UART_Driver_Data.BaudRate = config->BaudRate;
        ZHAL_UART_Driver_Data.TxStatus = UART_TX_IDLE;

        ZHAL_FIFO_Init(&ZHAL_UART_Rx_FIFO, &ZHAL_UART_Rx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);
        ZHAL_FIFO_Init(&ZHAL_UART_Tx_FIFO, &ZHAL_UART_Tx_FIFO_Buffer, ZHAL_UART_FIFO_SIZE);

        ZHAL_GPIO_Config_Pin(ZHAL_GPIO_A, GPIO_PIN_4 | GPIO_PIN_5, &gpio_config);

        uart_config.BaudRate = config->BaudRate;
        ZHAL_UART_Config(ZHAL_UART_0, &uart_config);
        ZHAL_UART_Baud_Rate_Config(ZHAL_UART_0, &uart_config, SYSTEM_CLOCK);

        ZHAL_UART_Receiver_Control(ZHAL_UART_0, ENABLE);
        ZHAL_UART_Transmitter_Control(ZHAL_UART_0, ENABLE);

        ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_RX, ZHAL_IRQ_NORMAL_PRIORITY);
        ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_TX, ZHAL_IRQ_NORMAL_PRIORITY);

        status = 1;
    }

    return (status);
}

/*
 * ZHAL_UART_Driver_Close
 * Closes the UART and frees the driver
 */
void ZHAL_UART_Driver_Close () {

    ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_RX, ZHAL_IRQ_DISABLED);
    ZHAL_Set_Interrupts(ZHAL_IRQ_UART0_TX, ZHAL_IRQ_DISABLED);

    ZHAL_UART_Receiver_Control(ZHAL_UART_0, DISABLE);
    ZHAL_UART_Transmitter_Control(ZHAL_UART_0, DISABLE);
}


/*
 * ZHAL_UART_Driver_Put_Data
 * Put data into UART Driver transmitter FIFO and returns the quantity of bytes inserted
 */
uint8_t ZHAL_UART_Driver_Put_Data (void * data, uint8_t bytes) {

    if (ZHAL_UART_Driver_Data.TxStatus == UART_TX_IDLE) {
        bytes = ZHAL_FIFO_Put_Bytes(&ZHAL_UART_Tx_FIFO, data, bytes);
    } else {
        bytes = 0;
    }
    return (bytes);
}

/*
 * ZHAL_UART_Driver_Send_Data
 *
 */
void ZHAL_UART_Driver_Send_Data () {
    uint8_t data;

    if (ZHAL_UART_Driver_Data.TxStatus == UART_TX_IDLE) {
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Tx_FIFO, &data, 1) != 0) {
            ZHAL_UART_Driver_Data.TxStatus = UART_TX_IN_PROGRESS;
            ZHAL_UART_Send(ZHAL_UART_0, data);
        }
    }
}

/*
 * ZHAL_UART_Driver_Get_Data
 * Get data from UART Driver receiver FIFO and returns the quantity of bytes get
 */
uint8_t ZHAL_UART_Driver_Get_Data (void * data, uint8_t bytes) {

    bytes = ZHAL_FIFO_Get_Bytes(&ZHAL_UART_Rx_FIFO, data, bytes);
    return (bytes);
}

/*
 * ZHAL_UART_Driver_Peek
 * Get the last byte inserted into UART FIFO, returns the quantity of bytes available
 */
uint8_t ZHAL_UART_Driver_Peek (void * data) {
	uint8_t bytes;

    bytes = ZHAL_FIFO_Peek(&ZHAL_UART_Rx_FIFO, data);
    return (bytes);
}


