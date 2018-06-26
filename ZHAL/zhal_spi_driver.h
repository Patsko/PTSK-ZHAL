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

#define ZHAL_SPI_FIFO_SIZE      50

#define ZHAL_SPI_BAUD_RATE      50000

/*
 * Typedefs
 */

typedef struct {
    ZHAL_GPIO_Port_t GPIO_Port;
    uint8_t GPIO_Pin;
    void far (* TxCallback) (void);
    void far (* RxCallback) (void);
} ZHAL_SPI_Driver_Config_t;

/*
 * Function prototypes
 */

void ZHAL_SPI_Driver_Init ();
bool_t ZHAL_SPI_Driver_Close ();
uint8_t ZHAL_SPI_Driver_Put_Data (void * data, uint8_t bytes);
void ZHAL_SPI_Driver_Send_Data (const ZHAL_SPI_Driver_Config_t * config);
uint8_t ZHAL_SPI_Driver_Get_Data (void * data, uint8_t bytes);
uint8_t ZHAL_SPI_Driver_Peek (void * data);
bool_t ZHAL_SPI_Driver_Is_Available ();


#endif // ZHAL_SPI_DRIVER_H
