/*
 * ZHAL SPI Driver
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_spi.h"
#include "zhal_spi_driver.h"
#include "zhal_drivers.h"

static struct {
    bool_t IsLocked;
    uint16_t BaudRate;
    void (* TxCallback) (void);
    void (* RxCallback) (void);
} ZHAL_SPI_Driver_Data;

static ZHAL_FIFO_t ZHAL_SPI_Rx_FIFO;
static ZHAL_FIFO_t ZHAL_SPI_Tx_FIFO;
static uint8_t ZHAL_SPI_Rx_FIFO_Buffer[ZHAL_SPI_FIFO_SIZE];
static uint8_t ZHAL_SPI_Tx_FIFO_Buffer[ZHAL_SPI_FIFO_SIZE];


/*
 * ZHAL_SPI_Driver_ISR_Callback
 *
 */
static void ZHAL_SPI_Driver_ISR_Callback (ZHAL_SPI_ISR_Callback_Arg_t isr) {
    uint8_t data;

    switch (isr) {
    case DATA_RECEIVED:
        data = ZHAL_SPI_Read(ZHAL_SPI_0);
        ZHAL_FIFO_Put_Bytes(&ZHAL_SPI_Rx_FIFO, &data, 1);
        if (ZHAL_SPI_Driver_Data.RxCallback != NULL) {
            (*ZHAL_SPI_Driver_Data.RxCallback)();
        }
        break;
    case TRANSMISSION_COMPLETE:
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_SPI_Tx_FIFO, &data, 1) != 0) {
            ZHAL_SPI_Send(ZHAL_SPI_0, data);
        } else {
            if (ZHAL_SPI_Driver_Data.TxCallback != NULL) {
                (*ZHAL_SPI_Driver_Data.TxCallback)();
            }
        }
        break;
    }
}


/*
 * ZHAL_SPI_Driver_Init
 * Inits the SPI driver and sets the lock id
 */
uint8_t ZHAL_SPI_Driver_Init (ZHAL_SPI_Driver_Handle_t * handle, ZHAL_SPI_Driver_Config_t * config) {
    uint8_t status = 0;
    ZHAL_GPIO_Config_t gpio_config = {
        ZHAL_GPIO_INPUT,
        ZHAL_GPIO_AF1,
        DISABLE,
        DISABLE,
        DISABLE,
        DISABLE
    };
    ZHAL_SPI_Config_t spi_config = {
        ZHAL_SPI_MODE_DEFAULT,
        ZHAL_SPI_MODE_MASTER,
        ZHAL_SPI_POL_PHASE_0,
        DISABLE,
        50000,
        ZHAL_SPI_Driver_ISR_Callback
    };

    if ((handle == NULL) || (config == NULL)) {
        status = 0;
    } else if ((ZHAL_SPI_Driver_Data.IsLocked == TRUE) && (handle->IsOwner == FALSE)) {
        status = 0;
    } else {
        ZHAL_SPI_Driver_Data.IsLocked = TRUE;
        handle->IsOwner = TRUE;

        ZHAL_SPI_Driver_Data.RxCallback = config->RxCallback;
        ZHAL_SPI_Driver_Data.TxCallback = config->TxCallback;
        ZHAL_SPI_Driver_Data.BaudRate = config->BaudRate;

        ZHAL_FIFO_Init(&ZHAL_SPI_Rx_FIFO, &ZHAL_SPI_Rx_FIFO_Buffer, ZHAL_SPI_FIFO_SIZE);
        ZHAL_FIFO_Init(&ZHAL_SPI_Tx_FIFO, &ZHAL_SPI_Tx_FIFO_Buffer, ZHAL_SPI_FIFO_SIZE);

        spi_config.BaudRate = config->BaudRate;
        ZHAL_SPI_Config(ZHAL_SPI_0, &spi_config);
        ZHAL_SPI_Baud_Rate_Config (ZHAL_SPI_0, &spi_config, SYSTEM_CLOCK);

        (void) ZHAL_SPI_Read(ZHAL_SPI_0);   // Reads any garbage that data register may contain

        ZHAL_GPIO_Config_Pin(ZHAL_GPIO_C,  GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, &gpio_config); // MISO, MOSI, SCK, respectively

        ZHAL_Set_Interrupts(ZHAL_IRQ_SPI, ZHAL_IRQ_NORMAL_PRIORITY);

        status = 1;
    }

    return (status);
}

/*
 * ZHAL_SPI_Driver_Close
 * Closes the SPI and frees the driver
 */
uint8_t ZHAL_SPI_Driver_Close (ZHAL_SPI_Driver_Handle_t * handle) {
    uint8_t status = 0;
    ZHAL_GPIO_Config_t gpio_config = {
        ZHAL_GPIO_INPUT,
        ZHAL_GPIO_NORMAL,
        DISABLE,
        DISABLE,
        DISABLE,
        DISABLE
    };

    if ((handle != NULL) && (handle->IsOwner)) {
        ZHAL_SPI_Disable(ZHAL_SPI_0);
        ZHAL_Set_Interrupts(ZHAL_IRQ_SPI, ZHAL_IRQ_DISABLED);

        ZHAL_GPIO_Config_Pin(ZHAL_GPIO_C,  GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, &gpio_config); // MISO, MOSI, SCK, respectively

        handle->IsOwner = FALSE;
        ZHAL_SPI_Driver_Data.IsLocked = FALSE;

        status = 1;
    }
    return (status);
}


/*
 * ZHAL_SPI_Driver_Put_Data
 * Put data into SPI Driver transmitter FIFO and returns the quantity of bytes inserted
 */
uint8_t ZHAL_SPI_Driver_Put_Data (ZHAL_SPI_Driver_Handle_t * handle, void * data, uint8_t bytes) {

    if ((handle != NULL) && (handle->IsOwner)) {
        bytes = ZHAL_FIFO_Put_Bytes(&ZHAL_SPI_Tx_FIFO, data, bytes);
    } else {
        bytes = 0;
    }
    return (bytes);
}

/*
 * ZHAL_SPI_Driver_Send_Data
 *
 */
void ZHAL_SPI_Driver_Send_Data (ZHAL_SPI_Driver_Handle_t * handle) {
    uint8_t data;

    if ((handle != NULL) && (handle->IsOwner)) {
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_SPI_Tx_FIFO, &data, 1) != 0) {
            ZHAL_SPI_Send(ZHAL_SPI_0, data);
        }
    }
}

/*
 * ZHAL_SPI_Driver_Get_Data
 * Get data from SPI Driver receiver FIFO and returns the quantity of bytes get
 */
uint8_t ZHAL_SPI_Driver_Get_Data (ZHAL_SPI_Driver_Handle_t * handle, void * data, uint8_t bytes) {

    if ((handle != NULL) && (handle->IsOwner)) {
        bytes = ZHAL_FIFO_Get_Bytes(&ZHAL_SPI_Rx_FIFO, data, bytes);
    } else {
        bytes = 0;
    }
    return (bytes);
}

/*
 * ZHAL_SPI_Driver_Peek
 * Get the last byte inserted into SPI FIFO, returns the quantity of bytes available
 */
uint8_t ZHAL_SPI_Driver_Peek (ZHAL_SPI_Driver_Handle_t * handle, void * data) {
    uint8_t bytes;

    if ((handle != NULL) && (handle->IsOwner)) {
        bytes = ZHAL_FIFO_Peek(&ZHAL_SPI_Rx_FIFO, data);
    } else {
        bytes = 0;
    }
    return (bytes);
}


