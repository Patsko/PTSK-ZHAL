/*
 * ZHAL
 *
 */

#ifndef ZHAL_H  // include guard
#define ZHAL_H

/*
 *
 */



#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned int
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

#define ZHAL_GPIO
#ifdef ZHAL_GPIO
#include "zhal_gpio.h"
#endif



#endif // ZHAL_H
