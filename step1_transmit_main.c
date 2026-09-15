//#############################################################################
//
// FILE:   main.c
//
// TITLE:  SCI-A Echo Program -- reads real bytes and sends them back
//
// BOARD:  LAUNCHXL-F28379D
// PINS:   GPIO42 (SCITXDA), GPIO43 (SCIRXDA)
//
// WHAT THIS PROGRAM DOES:
//   Waits for a byte to arrive on SCI-A (from the Pi, or PuTTY if you
//   type into it), then immediately sends that exact byte straight back.
//   Proves the link works in BOTH directions, not just DSP-to-Pi.
//
//#############################################################################

#include "driverlib.h"
#include "device.h"

volatile uint32_t rxByteCount = 0;
volatile uint16_t lastByte = 0;

void main(void)
{
    Device_init();
    Device_initGPIO();

    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);

    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 9600,
                  (SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE |
                   SCI_CONFIG_PAR_NONE));

    SCI_performSoftwareReset(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_enableFIFO(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);

    for(;;)
    {
        lastByte = SCI_readCharBlockingFIFO(SCIA_BASE);
        rxByteCount++;
        SCI_writeCharBlockingFIFO(SCIA_BASE, lastByte);
    }
}
