/*
 * ZHAL
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"


/*
 * ZHAL_Init
 */
void ZHAL_Init () {

    ZHAL_Watchdog_Config();
    ZHAL_Systick_Init();
    EI();
}

