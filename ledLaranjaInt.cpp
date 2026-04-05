#include "mbed.h"

int main(void) {
    void delayMs(int n);

    int pulseWidth = 0;

    SIM->SCGC5 |= 0x400; // Enable the clock to GPIO pin
    PORTB->PCR[18] |= 0x100;
    PTB->PDDR |= 0x40000;
    PTB->PCOR = 0x40000;
    PORTB->PCR[19] |= 0x300; // Assign the TPM2 signals the pin

    SIM->SCGC6 |= 0x4000000; // Enable clock to TPM2 module in SIM_SCGC6
    SIM->SOPT2 |= 0x1000000; // Select counter clock MCGFLLCLK
    TPM2->SC = 0; // Disable timer while the configuration is being modified
    TPM2->CONTROLS[1].CnSC = 0x20 | 0x4; // Set the mode for edge-aligned PWM
    TPM2->MOD = 43702; // Load the value into TPM2_MOD to set frequency = 60kHz
    TPM2->CONTROLS[1].CnV = 0; // Load the value into TPM2_CnV to set the desired duty cycle
    TPM2->SC |= 0x0C; // Enable timer with prescalar = 128

    while(1) {
        pulseWidth += 437;
        if (pulseWidth > 43702) pulseWidth = 0;
        TPM2->CONTROLS[1].CnV = pulseWidth;
        delayMs(20);
    }
}

void delayMs(int n)	{
    for(int i =	0; i < n; i++)
        for	(int j = 0;j < 7000; j++) {
            __asm("nop");
        }
}
