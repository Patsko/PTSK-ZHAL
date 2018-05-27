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
    bool_t IsOwner;
    uint8_t Status;
} ZHAL_SPI_Driver_Handle_t;

typedef struct {
    uint16_t BaudRate;
    void (* TxCallback) (void);
    void (* RxCallback) (void);
} ZHAL_SPI_Driver_Config_t;

/*
 * Function prototypes
 */

uint8_t ZHAL_SPI_Driver_Init (ZHAL_SPI_Driver_Handle_t * handle, ZHAL_SPI_Driver_Config_t * config);
uint8_t ZHAL_SPI_Driver_Close (ZHAL_SPI_Driver_Handle_t * handle);
uint8_t ZHAL_SPI_Driver_Put_Data (ZHAL_SPI_Driver_Handle_t * handle, void * data, uint8_t bytes);
void ZHAL_SPI_Driver_Send_Data (ZHAL_SPI_Driver_Handle_t * handle);
uint8_t ZHAL_SPI_Driver_Get_Data (ZHAL_SPI_Driver_Handle_t * handle, void * data, uint8_t bytes);
uint8_t ZHAL_SPI_Driver_Peek (ZHAL_SPI_Driver_Handle_t * handle, void * data);


#endif // ZHAL_SPI_DRIVER_H
