/*
 * ZHAL GPIO
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_gpio.h"


/*
 * ZHAL_GPIO_Config_Pin
 */
void ZHAL_GPIO_Config_Pin (ZHAL_GPIO_Port_t port, uint8_t pin, ZHAL_GPIO_Config_t * config) {
    uint8_t volatile * PxADDR_p = NULL;
    uint8_t volatile * PxCTL_p = NULL;

    switch (port) {
    case ZHAL_GPIO_A:
        PxADDR_p = &PAADDR;
        PxCTL_p = &PACTL;
        break;
    case ZHAL_GPIO_B:
        PxADDR_p = &PBADDR;
        PxCTL_p = &PBCTL;
        break;
    case ZHAL_GPIO_C:
        PxADDR_p = &PCADDR;
        PxCTL_p = &PCCTL;
        break;
    case ZHAL_GPIO_D:
        PxADDR_p = &PDADDR;
        PxCTL_p = &PDCTL;
        break;
    }

    if (PxADDR_p != NULL) {
        *PxADDR_p = ZHAL_GPIO_REG_DATA_DIRECTION;
        if (config->Direction == ZHAL_GPIO_OUTPUT) {
            *PxCTL_p &= (~pin);
        } else {
            *PxCTL_p |= pin;
        }

        switch (config->Alternate_Function) {
        case ZHAL_GPIO_NORMAL:
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p &= (~pin);
            break;
        case ZHAL_GPIO_AF1:
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p &= (~pin);

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_1;
            *PxCTL_p &= (~pin);
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_2;
            *PxCTL_p &= (~pin);

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p |= pin;
            break;
        case ZHAL_GPIO_AF2:
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p &= (~pin);

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_1;
            *PxCTL_p |= pin;
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_2;
            *PxCTL_p &= (~pin);

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p |= pin;
            break;
        case ZHAL_GPIO_AF3:
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p &= (~pin);

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_1;
            *PxCTL_p &= (~pin);
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_2;
            *PxCTL_p |= pin;

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p |= pin;
            break;
        case ZHAL_GPIO_AF4:
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p &= (~pin);

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_1;
            *PxCTL_p |= pin;
            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION_SET_2;
            *PxCTL_p |= pin;

            *PxADDR_p = ZHAL_GPIO_REG_ALT_FUNCTION;
            *PxCTL_p |= pin;
            break;
        }

        *PxADDR_p = ZHAL_GPIO_REG_OPEN_DRAIN;
        if (config->Open_Drain == ENABLE) {
            *PxCTL_p &= (~pin);
        } else {
            *PxCTL_p |= pin;
        }

        *PxADDR_p = ZHAL_GPIO_REG_HIGH_DRIVE;
        if (config->High_Drive == ENABLE) {
            *PxCTL_p &= (~pin);
        } else {
            *PxCTL_p |= pin;
        }

        *PxADDR_p = ZHAL_GPIO_REG_PULL_UP;
        if (config->Pull_Up == ENABLE) {
            *PxCTL_p &= (~pin);
        } else {
            *PxCTL_p |= pin;
        }

        *PxADDR_p = ZHAL_GPIO_REG_STOP_MODE;
        if (config->Stop_Mode_Recovery == ENABLE) {
            *PxCTL_p &= (~pin);
        } else {
            *PxCTL_p |= pin;
        }
    }
}


/*
 * ZHAL_GPIO_Read_Input
 */
uint8_t ZHAL_GPIO_Read_Input (ZHAL_GPIO_Port_t port, uint8_t pin) {
    unsigned char data = 0x00;
           
    switch (port) {
    case ZHAL_GPIO_A:
        data = (PAIN & pin);  
        break;
    case ZHAL_GPIO_B:
        data = (PBIN & pin);
        break;
    case ZHAL_GPIO_C:
        data = (PCIN & pin);
        break;
    case ZHAL_GPIO_D:
        data = (PDIN & pin);
        break;    
    }         
    
    if (data != 0) {
        data = 1;
    }
    
    return (data);
}

/*
 * ZHAL_GPIO_Set_Output
 */
void ZHAL_GPIO_Set_Output (ZHAL_GPIO_Port_t port, uint8_t pin) {
        
    switch (port) {
    case ZHAL_GPIO_A:
        PAOUT = (PAOUT | pin);
        break;
    case ZHAL_GPIO_B:
        PBOUT = (PBOUT | pin);    
        break;
    case ZHAL_GPIO_C:
        PCOUT = (PCOUT | pin);
        break;
    case ZHAL_GPIO_D:
        PDOUT = (PDOUT | pin);
        break;    
    }     
}

/*
 * ZHAL_GPIO_Reset_Output
 */
void ZHAL_GPIO_Reset_Output (ZHAL_GPIO_Port_t port, uint8_t pin) {
        
    pin = ~pin;
    
    switch (port) {
    case ZHAL_GPIO_A:
        PAOUT = (PAOUT & pin);
        break;
    case ZHAL_GPIO_B:
        PBOUT = (PBOUT & pin);    
        break;
    case ZHAL_GPIO_C:
        PCOUT = (PCOUT & pin);
        break;
    case ZHAL_GPIO_D:
        PDOUT = (PDOUT & pin);
        break;    
    }     
}
