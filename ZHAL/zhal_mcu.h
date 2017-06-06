/*
 * ZHAL Interrupts
 *
 */

#ifndef ZHAL_IRQ_H  // include guard
#define ZHAL_IRQ_H

/*
 * Application defines
 */

#define IRQ_TIMER2_MASK     0x80
#define IRQ_TIMER1_MASK     0x40
#define IRQ_TIMER0_MASK     0x20
#define IRQ_UART0_RX_MASK   0x10
#define IRQ_UART0_TX_MASK   0x08
#define IRQ_I2C_MASK        0x04
#define IRQ_SPI_MASK        0x02
#define IRQ_ADC_MASK        0x01

#define IRQ_PA7_LV_MASK     0x80
#define IRQ_PA6_C1_MASK     0x40
#define IRQ_PA5_C0_MASK     0x20
#define IRQ_PA4_PD4_MASK    0x10
#define IRQ_PA3_PD3_MASK    0x08
#define IRQ_PA2_PD2_MASK    0x04
#define IRQ_PA1_PD1_MASK    0x02
#define IRQ_PA0_MASK        0x01

#define IRQ_MCT_MASK        0x40
#define IRQ_UART1_RX_MASK   0x20
#define IRQ_UART1_TX_MASK   0x10
#define IRQ_PC3_MASK        0x08
#define IRQ_PC2_MASK        0x04
#define IRQ_PC1_MASK        0x02
#define IRQ_PC0_MASK        0x01

#define SYSTEM_CLOCK        11059200    // MCU clock in Hz

/*
 * Typedefs
 */

typedef enum ZHAL_IRQ_Priority {
    ZHAL_IRQ_DISABLED,
    ZHAL_IRQ_LOW_PRIORITY,
    ZHAL_IRQ_NORMAL_PRIORITY,
    ZHAL_IRQ_HIGH_PRIORITY,
} ZHAL_IRQ_Priority_t;

typedef enum ZHAL_IRQ {
    ZHAL_IRQ_TIMER2 = 0,
    ZHAL_IRQ_TIMER1,
    ZHAL_IRQ_TIMER0,
    ZHAL_IRQ_UART0_RX,
    ZHAL_IRQ_UART0_TX,
    ZHAL_IRQ_I2C,
    ZHAL_IRQ_SPI,
    ZHAL_IRQ_ADC,
    ZHAL_IRQ_PA7_LV,
    ZHAL_IRQ_PA6_C1,
    ZHAL_IRQ_PA5_C0,
    ZHAL_IRQ_PA4_PD4,
    ZHAL_IRQ_PA3_PD3,
    ZHAL_IRQ_PA2_PD2,
    ZHAL_IRQ_PA1_PD1,
    ZHAL_IRQ_PA0,
    ZHAL_IRQ_MCT,
    ZHAL_IRQ_UART1_RX,
    ZHAL_IRQ_UART1_TX,
    ZHAL_IRQ_PC3,
    ZHAL_IRQ_PC2,
    ZHAL_IRQ_PC1,
    ZHAL_IRQ_PC0,
} ZHAL_IRQ_t;

#endif // ZHAL_IRQ_H
