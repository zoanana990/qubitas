#include <qubitas/exti.h>
#include <qubitas/type.h>
#include <qubitas/uart.h>
#include <qubitas/io.h>
#include <qubitas/dma.h>
#include <qubitas/string.h>

#define RCC_BASE                    (0x40023800UL)
#define RCC_AHB1ENR(base)           (base + 0x30UL)
#define RCC_APB2ENR(base)           (base + 0x44UL)

#define SYSCFG_BASE                 (0x40013800UL)
#define SYSCFG_EXTICR4(base)        (base + 0x14UL)

#define EXTI_BASE                   (0x040013C00UL)
#define EXTI_IMR(base)              (base + 0x00UL)
#define EXTI_FTSR(base)             (base + 0x0CUL)
#define EXTI_PR(base)               (base + 0x14UL)

#define USART3_BASE                 (0x40004800UL)
#define USART_CR3(base)             (base + 0x08UL)
#define USART_CR3_DMAT_BIT          (7)

extern char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];

void syscfg_init(void) {
    u32 base = RCC_BASE;
    u32 data = io_read(RCC_APB2ENR(base));
    data |= (1 << 14);
    io_write(RCC_APB2ENR(base), data);
}

void clear_exti_pending_bit(int pin_number) {
    u32 addr = EXTI_PR(EXTI_BASE);

    if (io_read(addr) & (1 << pin_number)) {
        /* write 1 clear */
        io_write(addr, 1 << pin_number);
    }
}

void EXTI15_10_IRQHandler(void) {
    char str[] = "User Button Interrupt\r\n";
    strncpy(DMA_TX_DATA_STREAM, str, 60);
    io_writeMask(USART_CR3(USART3_BASE), 1 << USART_CR3_DMAT_BIT,
                 1 << USART_CR3_DMAT_BIT);

    clear_exti_pending_bit(13);
}