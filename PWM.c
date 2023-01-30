#include "lib/include.h"

extern void Configura_Reg_PWM1(uint16_t freq)
{
    SYSCTL->RCGCPWM |= (1<<0); // Habilita el reloj de modulo PWM0 pag 354
    SYSCTL->RCGCGPIO |= (1<<1); // Habilita el puerto B pág 340
    SYSCTL->RCC &= ~(1<<20);  // Deshabilita del divisor 
    GPIOB->AFSEL |= (1<<4); // Habilita el pin PB4 pag 672
    GPIOB->DIR = (1<<4); //Habilita PB4, 1 -- salida, 0 -- entrada
    GPIOB->PCTL |= (GPIOB->PCTL&0xFFF0FFFF) | 0x00040000; // Indica que se trata del PWM
    GPIOB->DEN |= (1<<4); // Habilitación como pin digital
    PWM0->_1_CTL = (0<<0);
    PWM0->_1_GENA = 0x0000008C; // Modo de trabajo de cuenta regresiva pag 1282
    PWM0->_1_LOAD = 5000; // Cuentas = (Frecuencia de reloj/Frecuencia PWM) Cuentas = (50000000/10000) = 5000
    PWM0->_1_CMPA = 4000; // 80% del ciclo de trabajo
    PWM0->_1_CTL = (1<<0);// Se activa el generador 1 del PWM0
    // Motion Control Module 0 PWM 2. This signal is
    // controlled by Module 0 PWM Generator 1.
    PWM0->ENABLE = (1<<2); // Enciende el PWM2 para trabajar con generador 1 según la tabla de la pág 1233 
}