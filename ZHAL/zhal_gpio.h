/*
 * ZHAL GPIO
 *
 */

#ifndef ZHAL_GPIO_H // include guard
#define ZHAL_GPIO_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

#define GPIO_PIN_0  0x01
#define GPIO_PIN_1  0x02
#define GPIO_PIN_2  0x04
#define GPIO_PIN_3  0x08
#define GPIO_PIN_4  0x10
#define GPIO_PIN_5  0x20
#define GPIO_PIN_6  0x40
#define GPIO_PIN_7  0x80

/*
 * Typedefs
 */

typedef enum ZHAL_GPIO_Subregister {
    ZHAL_GPIO_REG_DATA_DIRECTION = 1,
    ZHAL_GPIO_REG_ALT_FUNCTION,
    ZHAL_GPIO_REG_OPEN_DRAIN,
    ZHAL_GPIO_REG_HIGH_DRIVE,
    ZHAL_GPIO_REG_STOP_MODE,
    ZHAL_GPIO_REG_PULL_UP,
    ZHAL_GPIO_REG_ALT_FUNCTION_SET_1,
    ZHAL_GPIO_REG_ALT_FUNCTION_SET_2,
} ZHAL_GPIO_Subregister_t;

typedef enum ZHAL_GPIO_Port {
    ZHAL_NO_GPIO = 0,
    ZHAL_GPIO_A,
    ZHAL_GPIO_B,
    ZHAL_GPIO_C,
    ZHAL_GPIO_D
} ZHAL_GPIO_Port_t;

typedef enum ZHAL_GPIO_Direction {
    ZHAL_GPIO_OUTPUT = 0,
    ZHAL_GPIO_INPUT
} ZHAL_GPIO_Direction_t;

typedef enum ZHAL_GPIO_Alternate_Function {
    ZHAL_GPIO_NORMAL = 0,
    ZHAL_GPIO_AF1,
    ZHAL_GPIO_AF2,
    ZHAL_GPIO_AF3,
    ZHAL_GPIO_AF4
} ZHAL_GPIO_Alternate_Function_t;

typedef struct ZHAL_GPIO_Config {
    ZHAL_GPIO_Direction_t Direction;
    ZHAL_GPIO_Alternate_Function_t Alternate_Function  :2;
    enable_t Open_Drain          :1;
    enable_t High_Drive          :1;
    enable_t Pull_Up             :1;
    enable_t Stop_Mode_Recovery  :1;
} ZHAL_GPIO_Config_t;


/*
 * Function prototypes
 */

void ZHAL_GPIO_Config_Pin (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config);
uint8_t ZHAL_GPIO_Read_Input (ZHAL_GPIO_Port_t port, uint8_t pin);
void ZHAL_GPIO_Set_Output (ZHAL_GPIO_Port_t port, uint8_t pin);
void ZHAL_GPIO_Reset_Output (ZHAL_GPIO_Port_t port, uint8_t pin);


#endif // ZHAL_GPIO_H
