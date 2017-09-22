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

/* Confirmacion de encendido */
void ParpadeoOn(void)
{
    /* Funcion que enciende y apaga la lampara tres veces para
     mostrar que esta encendido y que esta listo para operar.*/
    //MemoryLightsOn();
    int l_icounter_1 = 0;
    while (l_icounter_1 < 6)
    {
        __delay_cycles(1100000);
        MemoryLightsToggle();

        l_icounter_1++;
    }
    l_icounter_1 = 0;
}

void EstadoInicial()
{
    /* Funcion que obtiene el valor de luz ambiental, si es
     noche (<15) se activa la interrupcion que enciende la(s)
     lampara(s) por 30 segundos. */
    if (g_flux < 15)
    {
        MemoryLightsOn();
        TIMER32_2->LOAD = 0x055D4A80; //30s ---> a 3Mhz
        NVIC_SetPriority(T32_INT2_IRQn, 2);
        NVIC_EnableIRQ(T32_INT2_IRQn);
        TIMER32_1->LOAD = 0x000249F0; //0.05s ---> a 3Mhz
        NVIC_SetPriority(T32_INT1_IRQn, 3);
        NVIC_EnableIRQ(T32_INT1_IRQn);
        NVIC_SetPriority(ADC14_IRQn, 3);
        NVIC_EnableIRQ(ADC14_IRQn);
    }
    return;
}

void BufferMuestras()
{
    /* Funcion encargada de tomar cada nueva muestra del ADC y colocarla
     en un vector de 20 datos, al llenarse calcula el promedio de estos*/
    g_vfmuestras[g_imuestras_counter] = g_fADCResult;
    g_imuestras_counter++;
    g_gmuestras_avg += g_fADCResult;

    if (g_imuestras_counter > 19)
    {
        g_gmuestras_avg = g_gmuestras_avg / 20;
        g_imuestras_counter = 0;
        StillCrying();
        BufferSonido();
    }
}

void BufferSonido()
{
    /* Funcion encargada de tomar el promedio de BufferMuestras() y
     guardar los valores en un vector de 5 datos. Por cada nuevo dato
     ingresado se elimina el dato mas viejo. Se calcula el promedio
     de los 4 datos mas viejos y si el ultimo dato ingresado es mayor
     que estos y es de noche se activa la interrupcion del ADC.*/
    for (g_isonido_counter = 0; g_isonido_counter < 4; g_isonido_counter++)
    {
        g_vfsonido[4 - g_isonido_counter] = g_vfsonido[3 - g_isonido_counter];
        g_fpromedio += g_vfsonido[4 - g_isonido_counter];
    }
    g_vfsonido[0] = g_gmuestras_avg;
    g_fpromedio = g_fpromedio / 5.0;

    if (g_vfsonido[0] > 1.5 * g_fpromedio)
    {
        MemoryLightsOn();

        if (g_iverify == 0) /* Si la bandera esta en bajo se activa la lampara 30 segundos */
        {
            g_fmemorizeavg = 1.5 * g_fpromedio;
            TIMER32_2->LOAD = 0x055D4A80; //~30s ---> a 48Mhz
            NVIC_SetPriority(T32_INT2_IRQn, 2);
            NVIC_EnableIRQ(T32_INT2_IRQn);
        }
        g_iverify++;
    }
}

void StillCrying()
{
    /* Funcion que lee los ultimos 5 segundos para determinar
     si el bebe aun se encuentra llorando aun despues de haber
     cumplido el tiempo minimo de encedido. */
    for (int j = 0; j < 5; j++)
    {
        g_fstillcrying += g_vfsonido[j];
    }
    g_fstillcrying = g_fstillcrying / 5.0;
}
