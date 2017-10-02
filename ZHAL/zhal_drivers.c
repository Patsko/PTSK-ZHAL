/*
 * ZHAL DRIVERS
 * Common code for all ZHAL drivers
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_drivers.h"

/*
 * ZHAL_FIFO_Init
 * Initializes the FIFO structure
 */
void ZHAL_FIFO_Init (ZHAL_FIFO_t * fifo, uint8_t * buffer, uint8_t size) {
    fifo->Head = 0;
    fifo->Tail = 0;
    fifo->Size = size;
    fifo->Data = buffer;
}


/*
 * ZHAL_FIFO_Put_Bytes
 * Put data into FIFO and returns the quantity of bytes inserted
 */
uint8_t ZHAL_FIFO_Put_Bytes (ZHAL_FIFO_t * fifo, uint8_t * data, uint8_t bytes) {
    uint8_t i;

    for (i = 0; (i < bytes) && (i < fifo->Size); i++) {
        fifo->Data[fifo->Head] = *data;
        data++;
        fifo->Head++;
        if (fifo->Head >= fifo->Size) {
            fifo->Head = 0;
        }
    }
    return (i);
}


/*
 * ZHAL_FIFO_Get_Bytes
 * Get data from FIFO and returns the quantity of bytes get
 */
uint8_t ZHAL_FIFO_Get_Bytes (ZHAL_FIFO_t * fifo, uint8_t * data, uint8_t bytes) {
    uint8_t i;
    uint8_t bytes_available;

    // Protection
    if (bytes > fifo->Size) {
        bytes = fifo->Size;
    }

    // Calculate the number of bytes available in the FIFO
    if (fifo->Tail <= fifo->Head) {
        bytes_available = fifo->Head - fifo->Tail;
    } else {
        bytes_available = (fifo->Size - fifo->Tail) + fifo->Head;
    }

    for (i = 0; (i < bytes) && (i < bytes_available); i++) {
        *data = fifo->Data[fifo->Tail];
        data++;
        fifo->Tail++;
        if (fifo->Tail >= fifo->Size) {
            fifo->Tail = 0;
        }
    }
    return (i);
}

