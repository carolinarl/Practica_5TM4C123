// ESTE ES UN ARCHIVO FUENTE

#include "lib/include.h"
// ESTE PROGRAMA ES DECLARADO EN SU LIBRERÍA COMO PROTOTIPO DE FUNCIÓN

extern void Configurar_UART5(void)
{
    //-------------------------------------CONFIGURACIÓN DEL PIN

    SYSCTL->RCGCUART  = (1<<5);   //Paso 1 (RCGCUART) pag.344 UART/modulo5 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port E
    
    GPIOE->LOCK= 0x4C4F434B;
    GPIOE->CR= 0xF0;
    
    //(GPIOAFSEL) pag.671 Enable alternate function
    //Pág. 895 para conocer los puertos asociados
    GPIOE->AFSEL = (1<<4) | (1<<5); //PE4 y PE5

    //GPIO Port Control (GPIOPCTL) PMC4-> U5Rx PMC5-> U5Tx pag.689
    // (1<<20) | (1<<16); (por el 1 en los pines 4 y 5, vía campo de bits)
    //0x00110000 en hexadecimal
    GPIOE->PCTL = (GPIOE->PCTL&0xFF00FFFF) | 0x00110000;
    
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE->DEN = (1<<4) | (1<<5);//PE4 PE5 como pines digitales
    

    //----------------------------------HABILITACIÓN Y CONFIG. DEL PROTOCOLO DE COMUNICACIÓN

    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART5->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    /*
    
    // 33MHZ 19200
    

    BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
    BRD = 33,000,000 / (16 * 19200) = 107.421875
    
    UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
    UARTFBRD[DIVFRAC] = integer(0.421875 * 64 + 0.5) = 27.5
    */
    UART5->IBRD = 107;
    UART5->FBRD = 27;

    //  UART Line Control (UARTLCRH) pag.916
    UART5->LCRH = (0x3<<5)|(1<<4);
    // 8 bits de longitud
    // habilitación de FIFOs

    //  UART Clock Configuration(UARTCC) pag.939
    UART5->CC =(0<<0);
    //UART0 UART Control (UARTCTL) pag.918 HABILITADO!!
    UART5->CTL = (1<<0) | (1<<8) | (1<<9);
    // Habilitación del UART
    // Habiltación de la transmisión UART
    // Habilitación de la recepción UART
    
    
}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    char c;
    while((UART5->FR & (1<<4)) != 0 ); //Evaluación de memoria FIFO
    c = UART5->DR;
    return (unsigned char) c;
}


extern void printChar(char c)
{
    while((UART5->FR & (1<<5)) != 0 );
    UART5->DR = c;
}


extern void printString(char* string)
{
    while(*(string)){
       printChar(*(string++));
    }
}


extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}