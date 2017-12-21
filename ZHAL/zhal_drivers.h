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
    DRIVER_IN_USE,
    DRIVER_ERROR,
} ZHAL_Driver_Status_t;

typedef struct {
    uint8_t DriverStatus;
    uint8_t LockID;
    uint8_t ISR_NewDataFlag;
    uint8_t ISR_TransmissionCompleteFlag;

    void (* Callback) (uint8_t);
} ZHAL_Driver_t;

/*
 * Function prototypes
 */

void ZHAL_FIFO_Init (ZHAL_FIFO_t * fifo, uint8_t * buffer, uint8_t size);
uint8_t ZHAL_FIFO_Put_Bytes (ZHAL_FIFO_t * fifo, uint8_t * data, uint8_t bytes);
uint8_t ZHAL_FIFO_Get_Bytes (ZHAL_FIFO_t * fifo, uint8_t * data, uint8_t bytes);
uint8_t ZHAL_FIFO_Peek (ZHAL_FIFO_t * fifo, uint8_t * data);

#endif // ZHAL_DRIVERS_H
