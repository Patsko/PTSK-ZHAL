/*
 * ZHAL SPI
 *
 */

#ifndef ZHAL_SPI_DRIVER_H // include guard
#define ZHAL_SPI_DRIVER_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

#define ZHAL_SPI_FIFO_SIZE     50

/*
 * Typedefs
 */

typedef struct {
    uint16_t BaudRate;
    void (* TxCallback) (void);
    void (* RxCallback) (void);
} ZHAL_SPI_Driver_t;
/*
 * Function prototypes
 */

uint8_t ZHAL_SPI_Driver_Init (ZHAL_SPI_Driver_t * spi_handle);
void ZHAL_SPI_Driver_Close (ZHAL_SPI_Driver_t * spi_handle);
uint8_t ZHAL_SPI_Driver_Put_Data (ZHAL_SPI_Driver_t * spi_handle, void * data, uint8_t bytes);
void ZHAL_SPI_Driver_Send_Data (ZHAL_SPI_Driver_t * spi_handle);
uint8_t ZHAL_SPI_Driver_Get_Data (ZHAL_SPI_Driver_t * spi_handle, void * data, uint8_t bytes);
void ZHAL_SPI_Driver_Control (ZHAL_SPI_Driver_t * spi_handle, uint8_t control);
uint8_t ZHAL_SPI_Driver_Peek (ZHAL_SPI_Driver_t * spi_handle, void * data);


#endif // ZHAL_SPI_DRIVER_H
