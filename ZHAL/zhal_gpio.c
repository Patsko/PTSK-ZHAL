/*
 * ZHAL GPIO
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_gpio.h"

/*
 * ZHAL_GPIO_Set_Direction
 */
static void ZHAL_GPIO_Set_Direction (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t * reg = NULL;

    switch (port) {
    case GPIO_PORTA:
        reg = &PADD;
        break;
    case GPIO_PORTB:
        reg = &PBDD;
        break;
    case GPIO_PORTC:
        reg = &PCDD;
        break;
    case GPIO_PORTD:
        reg = &PDDD;
        break;
    }

    if (reg != NULL) {
        if (config->Direction == GPIO_OUTPUT) {
            *reg &= (~pin);
        } else {
            *reg |= pin;
        }
    }
}

/*
 * ZHAL_GPIO_Set_Alternate_Function
 */
static void ZHAL_GPIO_Set_Alternate_Function (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t * PxAF_p = NULL;
    uint8_t * PxAFS1_p = NULL;
    uint8_t * PxAFS2_p = NULL;

    switch (port) {
    case GPIO_PORTA:
        PxAF_p = &PAAF;
        PxAFS1_p = &PAAFS1;
        PxAFS2_p = &PAAFS2;
        break;
    case GPIO_PORTB:
        PxAF_p = &PBAF;
        PxAFS1_p = &PBAFS1;
        PxAFS2_p = &PBAFS2;
        break;
    case GPIO_PORTC:
        PxAF_p = &PCAF;
        PxAFS1_p = &PCAFS1;
        PxAFS2_p = &PCAFS2;
        break;
    case GPIO_PORTD:
        PxAF_p = &PDAF;
        // Alternate Function subregisters are not implemented for Port D
        break;
    }

    if (PxAF_p != NULL) {
        switch (config->Alternate_Function) {
        case GPIO_NORMAL:
            *PxAF_p &= (~pin);
            break;
        case GPIO_AF1:
            *PxAF_p &= (~pin); // disable
            *PxAFS1_p &= (~pin);
            *PxAFS2_p &= (~pin);
            *PxAF_p |= pin;
            break;
        case GPIO_AF2:
            *PxAF_p &= (~pin);
            *PxAFS1_p |= pin;
            *PxAFS2_p &= (~pin);
            *PxAF_p |= pin;
            break;
        case GPIO_AF3:
            *PxAF_p &= (~pin);
            *PxAFS1_p &= (~pin);
            *PxAFS2_p |= pin;
            *PxAF_p |= pin;
            break;
        case GPIO_AF4:
            *PxAF_p &= (~pin);
            *PxAFS1_p |= pin;
            *PxAFS2_p |= pin;
            *PxAF_p |= pin;
            break;
        }
    }
}

/*
 * ZHAL_GPIO_Set_Open_Drain
 */
static void ZHAL_GPIO_Set_Open_Drain (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t * reg = NULL;

    switch (port) {
    case GPIO_PORTA:
        reg = &PAOC;
        break;
    case GPIO_PORTB:
        reg = &PBOC;
        break;
    case GPIO_PORTC:
        reg = &PCOC;
        break;
    case GPIO_PORTD:
        reg = &PDOC;
        break;
    }

    if (reg != NULL) {
        if (config->Open_Drain == ENABLE) {
            *reg |= pin;
        } else {
            *reg &= (~pin);
        }
    }
}

/*
 * ZHAL_GPIO_Set_High_Drive
 */
static void ZHAL_GPIO_Set_High_Drive (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t * reg = NULL;

    switch (port) {
    case GPIO_PORTA:
        reg = &PAHDE;
        break;
    case GPIO_PORTB:
        reg = &PBHDE;
        break;
    case GPIO_PORTC:
        reg = &PCHDE;
        break;
    case GPIO_PORTD:
        reg = &PDHDE;
        break;
    }

    if (reg != NULL) {
        if (config->High_Drive == ENABLE) {
            *reg |= pin;
        } else {
            *reg &= (~pin);
        }
    }
}

/*
 * ZHAL_GPIO_Set_Pull_Up
 */
static void ZHAL_GPIO_Set_Pull_Up (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t * reg = NULL;

    switch (port) {
    case GPIO_PORTA:
        reg = &PAPUE;
        break;
    case GPIO_PORTB:
        reg = &PBPUE;
        break;
    case GPIO_PORTC:
        reg = &PCPUE;
        break;
    case GPIO_PORTD:
        reg = &PDPUE;
        break;
    }

    if (reg != NULL) {
        if (config->Pull_Up == ENABLE) {
            *reg |= pin;
        } else {
            *reg &= (~pin);
        }
    }
}

/*
 * ZHAL_GPIO_Set_Stop_Mode_Recovery
 */
static void ZHAL_GPIO_Set_Stop_Mode_Recovery (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t * reg = NULL;

    switch (port) {
    case GPIO_PORTA:
        reg = &PASMRE;
        break;
    case GPIO_PORTB:
        reg = &PBSMRE;
        break;
    case GPIO_PORTC:
        reg = &PCSMRE;
        break;
    case GPIO_PORTD:
        reg = &PDSMRE;
        break;
    }

    if (reg != NULL) {
        if (config->Stop_Mode_Recovery == ENABLE) {
            *reg |= pin;
        } else {
            *reg &= (~pin);
        }
    }
}


/*
 * ZHAL_GPIO_Config_Pin
 */
void ZHAL_GPIO_Config_Pin (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {

    ZHAL_GPIO_Set_Direction(port, pin, config);
    ZHAL_GPIO_Set_Alternate_Function(port, pin, config);
    ZHAL_GPIO_Set_Open_Drain(port, pin, config);
    ZHAL_GPIO_Set_High_Drive(port, pin, config);
    ZHAL_GPIO_Set_Pull_Up(port, pin, config);
    ZHAL_GPIO_Set_Stop_Mode_Recovery(port, pin, config);
}


/*
 * ZHAL_GPIO_ReadInputPin
 */
uint8_t ZHAL_GPIO_ReadInputPin (ZHAL_GPIO_Port_t port, uint8_t pin) {
    unsigned char data = 0x00;
           
    switch (port) {
    case GPIO_PORTA:
        data = (PAIN & pin);  
        break;
    case GPIO_PORTB:
        data = (PBIN & pin);
        break;
    case GPIO_PORTC:
        data = (PCIN & pin);
        break;
    case GPIO_PORTD:
        data = (PDIN & pin);
        break;    
    }         
    
    if (data != 0) {
        data = 1;
    }
    
    return (data);
}

/*
 * ZHAL_GPIO_SetOutputPin
 */
void ZHAL_GPIO_SetOutputPin (ZHAL_GPIO_Port_t port, uint8_t pin) {
        
    switch (port) {
    case GPIO_PORTA:
        PAOUT = (PAOUT | pin);
        break;
    case GPIO_PORTB:
        PBOUT = (PBOUT | pin);    
        break;
    case GPIO_PORTC:
        PCOUT = (PCOUT | pin);
        break;
    case GPIO_PORTD:
        PDOUT = (PDOUT | pin);
        break;    
    }     
}

/*
 * ZHAL_GPIO_ResetOutputPin
 */
void ZHAL_GPIO_ResetOutputPin (ZHAL_GPIO_Port_t port, uint8_t pin) {
        
    pin = ~pin;
    
    switch (port) {
    case GPIO_PORTA:
        PAOUT = (PAOUT & pin);
        break;
    case GPIO_PORTB:
        PBOUT = (PBOUT & pin);    
        break;
    case GPIO_PORTC:
        PCOUT = (PCOUT & pin);
        break;
    case GPIO_PORTD:
        PDOUT = (PDOUT & pin);
        break;    
    }     
}
