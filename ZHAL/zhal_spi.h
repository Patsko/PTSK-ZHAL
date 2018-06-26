/*
 * ZHAL SPI
 *
 */

#ifndef ZHAL_SPI_H // include guard
#define ZHAL_SPI_H

#include "zhal.h"
#include <ez8.h>

/*
 * Application defines
 */

#define ZHAL_SPI_0_BASE_REG        (&ESPIDATA)

/*
 * Typedefs
 */

typedef enum ZHAL_SPI_Port {
    ZHAL_SPI_0 = 0x00,
    ZHAL_SPI_MAX,
} ZHAL_SPI_Port_t;

typedef enum ZHAL_SPI_Mode {
    ZHAL_SPI_MODE_DEFAULT = 0,
    ZHAL_SPI_MODE_I2S,
} ZHAL_SPI_Mode_t;

typedef enum ZHAL_SPI_MasterSlave {
    ZHAL_SPI_MODE_MASTER = 0,
    ZHAL_SPI_MODE_SLAVE,
} ZHAL_SPI_MasterSlave_t;

typedef enum ZHAL_SPI_PolPhase {
    ZHAL_SPI_POL_PHASE_0 = 0,
    ZHAL_SPI_POL_PHASE_1,
    ZHAL_SPI_POL_PHASE_2,
    ZHAL_SPI_POL_PHASE_3,
} ZHAL_SPI_PolPhase_t;

typedef enum ZHAL_SPI_ISR_Callback_Arg {
    ZHAL_SPI_ISR_TX = 1,
    ZHAL_SPI_ISR_RX,
} ZHAL_SPI_ISR_Callback_Arg_t;

typedef struct ZHAL_SPI_Config {
    ZHAL_SPI_Mode_t Mode;
    ZHAL_SPI_MasterSlave_t MasterSlave;
    ZHAL_SPI_PolPhase_t PolarityPhase;
    enable_t WiredOR;
    uint32_t BaudRate;
    void (* Callback) (ZHAL_SPI_ISR_Callback_Arg_t);
} ZHAL_SPI_Config_t;



/*
 * Function prototypes
 */

void ZHAL_SPI_Config (ZHAL_SPI_Port_t port, ZHAL_SPI_Config_t * config);
void ZHAL_SPI_Disable (ZHAL_SPI_Port_t port);
void ZHAL_SPI_Baud_Rate_Config (ZHAL_SPI_Port_t port, ZHAL_SPI_Config_t * config, uint32_t clock);
uint8_t ZHAL_SPI_Read (ZHAL_SPI_Port_t port) ;
void ZHAL_SPI_Send (ZHAL_SPI_Port_t port, uint8_t data);


#endif // ZHAL_SPI_H
