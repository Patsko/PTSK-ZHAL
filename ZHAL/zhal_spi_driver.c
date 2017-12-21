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

static ZHAL_Driver_t ZHAL_SPI_Driver_Data;
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
        ZHAL_SPI_Driver_Data.ISR_NewDataFlag = 1;
        break;
    case TRANSMISSION_COMPLETE:
        if (ZHAL_FIFO_Get_Bytes(&ZHAL_SPI_Tx_FIFO, &data, 1) != 0) {
            ZHAL_SPI_Send(ZHAL_SPI_0, data);
        } else {
            ZHAL_SPI_Driver_Data.ISR_TransmissionCompleteFlag = 1;
        }
        break;
    }
}


/*
 * ZHAL_SPI_Driver_Init
 * Inits the SPI driver and sets the lock id
 */
uint8_t ZHAL_SPI_Driver_Init (uint8_t * lock_id, ZHAL_SPI_Port_t port, ZHAL_SPI_Config_t * config, void (* callback) (uint8_t)) {
    uint8_t status = 0;
    ZHAL_GPIO_Config_t gpio_config = {
        ZHAL_GPIO_INPUT,
        ZHAL_GPIO_AF1,
        DISABLE,
        DISABLE,
        DISABLE,
        DISABLE
    };

    if (ZHAL_SPI_Driver_Data.DriverStatus == DRIVER_UNINITIALIZED) {
        switch (port) {
        case ZHAL_SPI_0:
            ZHAL_FIFO_Init(&ZHAL_SPI_Rx_FIFO, &ZHAL_SPI_Rx_FIFO_Buffer, ZHAL_SPI_FIFO_SIZE);
            ZHAL_FIFO_Init(&ZHAL_SPI_Tx_FIFO, &ZHAL_SPI_Tx_FIFO_Buffer, ZHAL_SPI_FIFO_SIZE);

            ZHAL_SPI_Driver_Data.DriverStatus = DRIVER_IN_USE;
            ZHAL_SPI_Driver_Data.LockID++;
            *lock_id = ZHAL_SPI_Driver_Data.LockID;
            ZHAL_SPI_Driver_Data.ISR_NewDataFlag = 0;
            ZHAL_SPI_Driver_Data.ISR_TransmissionCompleteFlag = 0;
            ZHAL_SPI_Driver_Data.Callback = callback;

            config->Callback = ZHAL_SPI_Driver_ISR_Callback;
            ZHAL_SPI_Config(port, config);
            ZHAL_SPI_Baud_Rate_Config (port, config, SYSTEM_CLOCK);

            (void) ZHAL_SPI_Read(ZHAL_SPI_0);   // Reads any garbage that data register may contain

            ZHAL_GPIO_Config_Pin(ZHAL_GPIO_C,  GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, &gpio_config); // MISO, MOSI, SCK, respectively

            ZHAL_Set_Interrupts(ZHAL_IRQ_SPI, ZHAL_IRQ_NORMAL_PRIORITY);

            status = 1;
            break;
        }
    }

    return (status);
}

/*
 * ZHAL_SPI_Driver_Close
 * Closes the SPI and frees the driver
 */
void ZHAL_SPI_Driver_Close (uint8_t lock_id, ZHAL_SPI_Port_t port) {
    ZHAL_GPIO_Config_t gpio_config = {
        ZHAL_GPIO_INPUT,
        ZHAL_GPIO_NORMAL,
        DISABLE,
        DISABLE,
        DISABLE,
        DISABLE
    };

    if (ZHAL_SPI_Driver_Data.LockID == lock_id) {
        switch (port) {
        case ZHAL_SPI_0:
            ZHAL_SPI_Disable(port);
            ZHAL_Set_Interrupts(ZHAL_IRQ_SPI, ZHAL_IRQ_DISABLED);

            ZHAL_GPIO_Config_Pin(ZHAL_GPIO_C,  GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, &gpio_config); // MISO, MOSI, SCK, respectively

            ZHAL_SPI_Driver_Data.ISR_NewDataFlag = 0;
            ZHAL_SPI_Driver_Data.ISR_TransmissionCompleteFlag = 0;
            ZHAL_SPI_Driver_Data.Callback = NULL;

            ZHAL_SPI_Driver_Data.DriverStatus = DRIVER_UNINITIALIZED;
            break;
        }
    }
}


/*
 * ZHAL SPI Driver
 * Task/function that controls the Driver
 */
void ZHAL_SPI_Driver () {

    switch (ZHAL_SPI_Driver_Data.DriverStatus) {
    case DRIVER_UNINITIALIZED:
        break;
    case DRIVER_IN_USE:
        if (ZHAL_SPI_Driver_Data.ISR_TransmissionCompleteFlag) {
            ZHAL_SPI_Driver_Data.ISR_TransmissionCompleteFlag = 0;
            if (ZHAL_SPI_Driver_Data.Callback != NULL) {
                (*ZHAL_SPI_Driver_Data.Callback)(2);
            }
        }
        break;
    case DRIVER_ERROR:
        break;
    default:
        ZHAL_SPI_Driver_Data.DriverStatus = DRIVER_UNINITIALIZED;
        break;
    }
}

/*
 * ZHAL_SPI_Driver_Put_Data
 * Put data into SPI Driver transmitter FIFO and returns the quantity of bytes inserted
 */
uint8_t ZHAL_SPI_Driver_Put_Data (uint8_t lock_id, void * data, uint8_t bytes) {

    if (ZHAL_SPI_Driver_Data.LockID == lock_id) {
        bytes = ZHAL_FIFO_Put_Bytes(&ZHAL_SPI_Tx_FIFO, data, bytes);
    }
    return (bytes);
}

/*
 * ZHAL_SPI_Driver_Get_Data
 * Get data from SPI Driver receiver FIFO and returns the quantity of bytes get
 */
uint8_t ZHAL_SPI_Driver_Get_Data (uint8_t lock_id, void * data, uint8_t bytes) {

    if (ZHAL_SPI_Driver_Data.LockID == lock_id) {
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
uint8_t ZHAL_SPI_Driver_Peek (uint8_t lock_id, void * data) {
	uint8_t bytes;

    if (ZHAL_SPI_Driver_Data.LockID == lock_id) {
        bytes = ZHAL_FIFO_Peek(&ZHAL_SPI_Rx_FIFO, data);
    } else {
        bytes = 0;
    }
    return (bytes);
}



/*
 * ZHAL_SPI_Driver_Control
 *
 */
void ZHAL_SPI_Driver_Control (uint8_t lock_id, uint8_t control) {
    uint8_t data;

    if (ZHAL_SPI_Driver_Data.LockID == lock_id) {
        switch (control) {
        case 1:
            if (ZHAL_FIFO_Get_Bytes(&ZHAL_SPI_Tx_FIFO, &data, 1) != 0) {
                ZHAL_SPI_Send(ZHAL_SPI_0, data);
            } else {
                ZHAL_SPI_Driver_Data.ISR_TransmissionCompleteFlag = 1;
            }
            break;
        }
    }
}
