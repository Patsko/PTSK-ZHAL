/*
 * ZHAL DRIVERS
 * Common code for all ZHAL drivers
 */

#ifndef ZHAL_DRIVERS_H // include guard
#define ZHAL_DRIVERS_H

#include "zhal.h"
#include <ez8.h>


/*
 * Typedefs
 */

typedef struct {
    uint8_t Head;
    uint8_t Tail;
    uint8_t Size;
    uint8_t * Data;
} ZHAL_FIFO_t;

typedef enum ZHAL_Driver_Status {
    DRIVER_UNINITIALIZED = 0,
    DRIVER_IDLE,
    DRIVER_IN_USE,
    DRIVER_ERROR,
} ZHAL_Driver_Status_t;

/*
 * Function prototypes
 */

void ZHAL_FIFO_Init (ZHAL_FIFO_t * fifo, void * buffer, uint8_t size);
uint8_t ZHAL_FIFO_Put_Bytes (ZHAL_FIFO_t * fifo, void * data, uint8_t bytes);
uint8_t ZHAL_FIFO_Get_Bytes (ZHAL_FIFO_t * fifo, void * data, uint8_t bytes);


#endif // ZHAL_DRIVERS_H
