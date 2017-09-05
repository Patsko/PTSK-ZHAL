/*
 * ZHAL UART
 *
 */

#ifndef ZHAL_UART_DRIVER_H // include guard
#define ZHAL_UART_DRIVER_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

#define ZHAL_UART_FIFO_SIZE     50

/*
 * Typedefs
 */

typedef struct {
    uint8_t DriverStatus;
    uint8_t LockStatus;
    uint8_t ISR_NewDataFlag;
    uint8_t ISR_TransmissionCompleteFlag;

    void (* Callback) (void *);
    void * Callback_Arg;
} ZHAL_Driver_t;

/*
 * Function prototypes
 */


#endif // ZHAL_UART_DRIVER_H
