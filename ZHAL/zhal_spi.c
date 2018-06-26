/*
 * ZHAL SPI
 *
 */

#include <ez8.h>
#include <stdio.h>
#include "zhal.h"
#include "zhal_spi.h"

static void (* ZHAL_Callback_fp[ZHAL_SPI_MAX]) (ZHAL_SPI_ISR_Callback_Arg_t);

/*
 * ZHAL_SPI_Config
 */
void ZHAL_SPI_Config (ZHAL_SPI_Port_t port, ZHAL_SPI_Config_t * config) {
    uint8_t volatile * base_reg_p = NULL;
    uint8_t volatile * SPIxCTL_p = NULL;
    uint8_t volatile * SPIxMODE_p = NULL;

    switch (port) {
    case ZHAL_SPI_0:
        base_reg_p = ZHAL_SPI_0_BASE_REG;
        break;
    default:
        break;
    }

    if (base_reg_p != NULL) {
        SPIxCTL_p = base_reg_p + 0x02;
        SPIxMODE_p = base_reg_p + 0x03;
        *SPIxCTL_p = 0x00;
        *SPIxMODE_p = 0x00;

        *SPIxCTL_p |= 0xC1;     // enables receive and transmit interrupt, enables transmit/receive mode

        switch (config->PolarityPhase) {
        case ZHAL_SPI_POL_PHASE_0:  // CPOL = 0, CPHA = 0
            // bits are already zero
            break;
        case ZHAL_SPI_POL_PHASE_1:  // CPOL = 0, CPHA = 1
            *SPIxCTL_p |= 0x10;
            break;
        case ZHAL_SPI_POL_PHASE_2:  // CPOL = 1, CPHA = 0
            *SPIxCTL_p |= 0x08;
            break;
        case ZHAL_SPI_POL_PHASE_3:  // CPOL = 1, CPHA = 1
            *SPIxCTL_p |= 0x18;
            break;
        }

        if (config->WiredOR == ENABLE) {
            *SPIxCTL_p |= 0x04;
        }

        if (config->MasterSlave == ZHAL_SPI_MODE_MASTER) {
            *SPIxMODE_p |= 0x02;    // SS pin as an output
            *SPIxCTL_p |= 0x02;     // Master mode
        }

        if (config->Mode == ZHAL_SPI_MODE_I2S) {
            *SPIxMODE_p |= 0x40;
        }

        ZHAL_Callback_fp[port] = config->Callback;

    }
}


/*
 * ZHAL_SPI_Disable
 */
void ZHAL_SPI_Disable (ZHAL_SPI_Port_t port) {
    uint8_t data;

    switch (port) {
    case ZHAL_SPI_0:
        ESPITDCR = 0x80;    // clears data register and resets SSV bit
        ESPICTL = 0x00;
        ESPISTAT |= 0x78;   // writes 1 to clear TUND, COL, ABT, ROVR bits
        data = ESPIDATA;    // reads any garbage which ESPIDATA may contain
        break;
    default:
        break;
    }
}


/*
 * ZHAL_SPI_Baud_Rate_Config
 */
void ZHAL_SPI_Baud_Rate_Config (ZHAL_SPI_Port_t port, ZHAL_SPI_Config_t * config, uint32_t clock) {

    clock /= 2;
    clock /= config->BaudRate;

    switch (port) {
    case ZHAL_SPI_0:
        ESPIBR = (uint16_t) clock;
        break;
    default:
        break;
    }
}


/*
 * ZHAL_SPI_Read
 */
uint8_t ZHAL_SPI_Read (ZHAL_SPI_Port_t port) {
    uint8_t data = 0;
    
    switch (port) {
    case ZHAL_SPI_0:
        data = ESPIDATA;
        break;
    }
    return (data);
}

/*
 * ZHAL_SPI_Send
 */
void ZHAL_SPI_Send (ZHAL_SPI_Port_t port, uint8_t data) {
        
    switch (port) {
    case ZHAL_SPI_0:
        ESPIDATA = data;
        break;
    }     
}



void interrupt ZHAL_SPI_0_ISR () _At SPI {

    if ((ESPISTAT & 0x04) != 0) {   // RDRNE bit set
        if (ZHAL_Callback_fp[ZHAL_SPI_0] != NULL) {
            (*ZHAL_Callback_fp)(ZHAL_SPI_ISR_RX);
        }
    }
    if ((ESPISTAT & 0x80) != 0) {   // TDRE bit set
        if (ZHAL_Callback_fp[ZHAL_SPI_0] != NULL) {
            (*ZHAL_Callback_fp)(ZHAL_SPI_ISR_TX);
        }
    }
    // error statuses (overrun, underrun, etc) are not treated yet
}

