/*
 * ZHAL
 *
 */

#ifndef ZHAL_H  // include guard
#define ZHAL_H

// you should provide a zhal_config header file, based on zhal_config_example.h
#include "zhal_config.h"

/*
 * Typedefs
 */

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned int uint16_t;
#endif

#ifndef uint16_t
typedef unsigned long uint32_t;
#endif

#ifndef bool_t
typedef enum Bool_enum {
    FALSE = 0,
    TRUE = 1,
} bool_t;
#endif

#ifndef enable_t
typedef enum Enable_enum {
    DISABLE = 0,
    ENABLE = 1,
} enable_t;
#endif


/*
 * Compilation defines
 */

#if ZHAL_USE_GPIO
#include "zhal_gpio.h"
#endif

#if ZHAL_USE_TIMER
#include "zhal_timer.h"
#endif

#if ZHAL_USE_UART
#include "zhal_uart.h"
#endif

#if ZHAL_USE_UART_DRIVER
#include "zhal_uart_driver.h"
#endif

#if ZHAL_USE_SPI
#include "zhal_spi.h"
#endif

#if ZHAL_USE_SPI_DRIVER
#include "zhal_spi_driver.h"
#endif

#include "zhal_mcu.h"
#include "zhal_systick.h"

/*
 * System defines
 */

#define MAX_UINT32          0xFFFFFFFF
#define MAX_UINT16          0xFFFF
#define MAX_UINT8           0xFF


/*
 * Function prototypes
 */

void ZHAL_Init();


#endif // ZHAL_H
