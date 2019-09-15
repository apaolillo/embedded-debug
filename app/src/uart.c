
#define NB_UART 2
#define UART0_BASE_ADDRESS 0xE0000000
#define UART1_BASE_ADDRESS 0xE0001000

#include <stdint-gcc.h>

#define UART_CONTROL_TXDIS (1u << 5)
#define UART_CONTROL_TXEN (1u << 4)
#define UART_STATUS_TFUL (1u << 4)
#define UART_STATUS_TEMPTY (1u << 3)
#define UART_STATUS_RFUL (1u << 2)
#define UART_STATUS_REMPTY (1u << 1)

typedef volatile struct {
    uint32_t control;
    uint32_t mode;
    uint32_t interruptEnable;
    uint32_t interruptDisable;
    const uint32_t interruptMask;
    uint32_t channelInterruptStatus;
    uint32_t baudrateGenerator;
    uint32_t receiverTimeout;
    uint32_t receiverFifoTriggerLevel;
    uint32_t modemControl;
    uint32_t modemStatus;
    const uint32_t channelStatus;
    uint32_t fifo;
    uint32_t baudrateDivider;
    uint32_t flowControlDelay;
    const uint32_t reserved[2];
    uint32_t transmitterFifoTriggerLevel;
} ZynqUart_s;

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
    while (uartPtr->channelStatus & UART_STATUS_TFUL);

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
