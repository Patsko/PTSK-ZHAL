/*
 * ZHAL
 *
 */

#ifndef ZHAL_H  // include guard
#define ZHAL_H

/*
 * Typedefs
 */

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned int uint16_t;
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

#define SYSTICK_TIMER   ZHAL_TIMER_0

#define ZHAL_USE_GPIO   1
#define ZHAL_USE_TIMER  1

#if ZHAL_USE_GPIO
#include "zhal_gpio.h"
#endif

#if ZHAL_USE_TIMER
#include "zhal_timer.h"
#endif

#include "zhal_mcu.h"
#include "zhal_systick.h"


/*
 * Function prototypes
 */

void ZHAL_Init();


#endif // ZHAL_H
