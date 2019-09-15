
#define NB_UART 2
#define UART0_BASE_ADDRESS 0xE0000000
#define UART1_BASE_ADDRESS 0xE0001000

/**
 * @{
 * @name UART control register.
 *
 * @see Zynq 7000 TRM, section B.33: UART controller.
 */

/**
 * Transmission disable bit. If set, disables transmission regardless
 * of the value of TXEN.
 */
#include <stdint-gcc.h>

#define UART_CONTROL_TXDIS (1u << 5)

/** Transmission enable bit. Enables transmission (if TXDIS is cleared). */
#define UART_CONTROL_TXEN (1u << 4)

/** @} */

/**
 * @{
 * @name UART status register.
 *
 * @see Zynq 7000 TRM, section 19.2.10: Status and Interrupts.
 */

/** Transmit FIFO full status bit. */
#define UART_STATUS_TFUL (1u << 4)

/** Transmit FIFO empty status bit. */
#define UART_STATUS_TEMPTY (1u << 3)

/** Receive FIFO full status bit. */
#define UART_STATUS_RFUL (1u << 2)

/** Receive FIFO empty status bit. */
#define UART_STATUS_REMPTY (1u << 1)

/** @} */

/**
 * @brief Zynq UART controller memory map.
 *
 * @see Zynq 7000 TRM Appendix B: Register details
 *      chapter B.33: UART controller.
 */
typedef volatile struct {
    /** Control register (R/W): 0x00. */
    uint32_t control;

    /** UART mode register (R/W): 0x04. */
    uint32_t mode;

    /** Interrupt enable register (R/W): 0x08. */
    uint32_t interruptEnable;

    /** Interrupt disable register (R/W): 0x0C. */
    uint32_t interruptDisable;

    /** Interrupt mask register (R-O): 0x10. */
    const uint32_t interruptMask;

    /** Channel interrupt status register (write to clear): 0x14. */
    uint32_t channelInterruptStatus;

    /** Baudrate generator register (R/W): 0x18. */
    uint32_t baudrateGenerator;

    /** Receiver timeout register (R/W): 0x1C. */
    uint32_t receiverTimeout;

    /** Receiver FIFO trigger level register (R/W): 0x20. */
    uint32_t receiverFifoTriggerLevel;

    /** Modem control register (R/W): 0x24. */
    uint32_t modemControl;

    /** Modem status register (R/W): 0x28. */
    uint32_t modemStatus;

    /** Channel status register (R-O): 0x2C. */
    const uint32_t channelStatus;

    /** Transmit and receive FIFO (R/W): 0x30. */
    uint32_t fifo;

    /** Baudrate divider register (R/W): 0x34. */
    uint32_t baudrateDivider;

    /** Flow control delay register (R/W): 0x38. */
    uint32_t flowControlDelay;

    /** Reserved space: 0x3C-0x40. */
    const uint32_t reserved[2];

    /** Transmitter FIFO trigger level register (R/W): 0x44. */
    uint32_t transmitterFifoTriggerLevel;
} ZynqUart_s;

/** @brief Represents the ID of a UART line, for convenience. */
typedef enum
{
    UART0 = 0x0u,
    UART1 = 0x1u,
} Zynq_UartLineId_e;

typedef Zynq_UartLineId_e _UartLineId_e;
typedef _UartLineId_e UartLineId_e;

static ZynqUart_s* uartLines[NB_UART] = {
        (ZynqUart_s*) UART0_BASE_ADDRESS,
        (ZynqUart_s*) UART1_BASE_ADDRESS,
};

static void zynq_uart_init(ZynqUart_s* uartPtr)
{
    uartPtr->control &= ~UART_CONTROL_TXDIS;
    uartPtr->control |= UART_CONTROL_TXEN;
}

static void zynq_uart_sendByte(ZynqUart_s* uartPtr, uint8_t byte)
{
    while (uartPtr->channelStatus & UART_STATUS_TFUL)
        ;

    uartPtr->fifo = byte;
}

void uart_init(void)
{
    zynq_uart_init(uartLines[0]);
}

void _uart_sendByte(UartLineId_e line, uint8_t byte)
{
    zynq_uart_sendByte(uartLines[line], byte);
}

void _uart_send(UartLineId_e line, const char* string, unsigned length)
{
    const char* charToSend = string;

    for (unsigned i = length; i != 0u; --i) {
        _uart_sendByte(line, *charToSend);
        ++charToSend;
    }
}

void uart_sendBytes(const char* string, unsigned length)
{
    _uart_send(UART1, string, length);
}
