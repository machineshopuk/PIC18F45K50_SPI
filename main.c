#include "mcc_generated_files/mcc.h"

void main(void)
{
    SYSTEM_Initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    char data2[20];
    static uint8_t buffer[2] = "data";
    float temperature = 0.0;
    
    TRISAbits.RA5 = 0;
    ANSELAbits.ANSA5 = 0;
    LATAbits.LA5 = 1;
    TRISD = 0;
    ANSELD = 0;
    
    if(!spi_master_open(MASTER0)){
        LATDbits.LATD2 = 1;
        while(1){}
    }
    while (1)
    {
        __delay_ms(100);
        if(USBUSARTIsTxTrfReady() == true)
        {
            LATDbits.LATD7 = !LATDbits.LATD7;
            LATAbits.LA5 = 0;
            SPI1_ExchangeBlock(buffer, sizeof(buffer));
            LATAbits.LA5 = 1;
            temperature = (buffer[0] << 8) + (buffer[1])>>2; 
            sprintf(data2,"Temperature: %f\r\n", temperature/4);
            putsUSBUSART(data2);
        }
        CDCTxService(); 
    }
}
