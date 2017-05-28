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

typedef enum ZHAL_GPIO_Port {
    GPIO_PORTA = 0x00,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD
} ZHAL_GPIO_Port_t;

typedef enum ZHAL_GPIO_Direction {
    GPIO_OUTPUT = 0,
    GPIO_INPUT
} ZHAL_GPIO_Direction_t;

typedef enum ZHAL_GPIO_Alternate_Function {
    GPIO_NORMAL = 0,
    GPIO_AF1,
    GPIO_AF2,
    GPIO_AF3,
    GPIO_AF4
} ZHAL_GPIO_Alternate_Function_t;

typedef struct ZHAL_GPIO_Config {
    ZHAL_GPIO_Direction_t Direction;
    uint8_t Alternate_Function;
    uint8_t Open_Drain;
    uint8_t High_Drive;
    uint8_t Pull_Up;
    uint8_t Stop_Mode_Recovery;
} ZHAL_GPIO_Config_t;


/*
 * Function prototypes
 */

void ZHAL_GPIO_Config_Pin (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config);
uint8_t ZHAL_GPIO_ReadInputPin (ZHAL_GPIO_Port_t port, uint8_t pin);
void ZHAL_GPIO_SetOutputPin (ZHAL_GPIO_Port_t port, uint8_t pin);
void ZHAL_GPIO_ResetOutputPin (ZHAL_GPIO_Port_t port, uint8_t pin);



#endif // ZHAL_GPIO_H
