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
 *
 */
void ZHAL_FIFO_Init (ZHAL_FIFO_t * fifo, void * buffer, uint8_t size) {
    fifo->Head = 0;
    fifo->Tail = 0;
    fifo->Size = 0;
    fifo->Data = (uint8_t *) buffer;
}


/*
 * ZHAL_FIFO_Put_Bytes
 *
 */
uint8_t ZHAL_FIFO_Put_Bytes (ZHAL_FIFO_t * fifo, void * data, uint8_t bytes) {
    uint8_t i;

    for (i = 0; (i < bytes) && (i < fifo->Size); i++) {
        fifo->Data[fifo->Head] = *(uint8_t *) data;
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
 *
 */
uint8_t ZHAL_FIFO_Get_Bytes (ZHAL_FIFO_t * fifo, void * data, uint8_t bytes) {
    uint8_t i;

    for (i = 0; (i < bytes) && (i < fifo->Size); i++) {
        *(uint8_t *) data = fifo->Data[fifo->Tail++];
        data++;
        fifo->Tail++;
        if (fifo->Tail >= fifo->Size) {
            fifo->Tail = 0;
        }
    }
    return (i);
}

