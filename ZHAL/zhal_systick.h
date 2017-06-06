/*
 * ZHAL Systick
 *
 */

#ifndef ZHAL_SYSTICK_H // include guard
#define ZHAL_SYSTICK_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */


/*
 * Typedefs
 */



/*
 * Function prototypes
 */

void ZHAL_Systick_Init ();
uint16_t ZHAL_Systick_Get_Tick ();
void ZHAL_Systick_Delay (uint16_t ticks_to_wait);


#endif // ZHAL_TIMER_H
