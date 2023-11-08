#include <qubitas/string.h>
#include <qubitas/uart.h>
#include <qubitas/dma.h>
#include <qubitas/crc.h>
#include <qubitas/gpio.h>
#include <qubitas/printk.h>
#include <qubitas/kernel.h>

extern char DMA_TX_DATA_STREAM[DMA_MAX_STRLEN];

int main() {
    usart_init();
    crc_init();
    button_init();

    dma_initUart3Tx();
    dma_configIntr(DMA1, DMA_STREAM3);
    dma_enStreamX(DMA1, DMA_STREAM3);

    printk(INFO "Kernel Compile time %s\r\n", __DATE__);
    printk(WARNING "Start kernel... \r\n");
    printk(DEBUG "Qubitas Bootloader @Copyright\r\n");

    while (1);

    /* not expect to go here */
    return 0;
}