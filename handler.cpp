/*
 * -> Universidad de Costa Rica
 * -> Escuela de Ingenieria Electrica
 * -> IE -1119 Laboratorio de Programacion de Sistemas Incrustados
 * -> Estudiantes: Luisa Quesada Camacho [B35427], Jose Martinez Hernandez [B34024]
 * -> Laboratorio 1 : Introduccion a la Tarjeta de Desarrollo
 */

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <C:/ti/simplelink_msp432_sdk_1_40_01_00/source/ti/grlib/grlib.h>
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "setup.h"
#include "handler.h"
#include "functions.h"


/* Manejador de interrupciones */
extern "C"
{
/* Resetear la lectura del ADC */
void T32_INT1_IRQHandler(void)
{
    __disable_irq();
    TIMER32_1->INTCLR = 0U;
    P2->OUT ^= BIT1 + BIT2;
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
    __enable_irq();
    return;
}

/* Interrupcion del ADC que indica cuando hay una nueva muestra*/
void ADC14_IRQHandler(void)
{
    __disable_irq();
    g_u16ADC14Result = 8191 - ADC14->MEM[0];
    g_u16ADC14Result = abs(g_u16ADC14Result);
    g_fADCResult = float(g_u16ADC14Result);
    BufferMuestras();
    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;
    __enable_irq();
    return;
}

/* Interrupcion de tipo OneShot para apagar la(s) lampara(s)
 luego de un tiempo determinado. */
void T32_INT2_IRQHandler(void)
{
    __disable_irq();
    TIMER32_2->INTCLR = 0U;
    __enable_irq();
    if (g_fstillcrying > g_fmemorizeavg)
    {
        if (g_flux < 15)
        {
            TIMER32_2->LOAD = 0x055D4A80; //~30s ---> a 48Mhz
            NVIC_SetPriority(T32_INT2_IRQn, 2);
            NVIC_EnableIRQ(T32_INT2_IRQn);
            return;
        }
        else
        {
            g_iverify = 0;
            MemoryLightsOff();
            return;
        }
    }
    g_iverify = 0;
    MemoryLightsOff();
    return;
}

/* Interrupcion del boton del Joystick que permite encender
 y apagar la(s) lamparas manualmente por el usuario.*/
void PORT4_IRQHandler(void)
{
    __disable_irq();
    P4->IFG &= ~BIT1;
    __enable_irq();

    /* Si las lamparas estan encendidas se espera un tiempo corto y se apagan*/
    if (P5->OUT & BIT6 || P2->OUT & BIT4 || P2->OUT & BIT6)
    {
        g_fstillcrying = 0.0;
        TIMER32_2->LOAD = 0x000004001;
        NVIC_SetPriority(T32_INT2_IRQn, 2);
        NVIC_EnableIRQ(T32_INT2_IRQn);
    }
    else
    {
      /* Si las lamparas estan apagadas se encienden por 30 segundos*/
        MemoryLightsOn();
        TIMER32_2->LOAD = 0x055D4A80;
        NVIC_SetPriority(T32_INT2_IRQn, 2);
        NVIC_EnableIRQ(T32_INT2_IRQn);
    }
}
}
