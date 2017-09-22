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


/* VARIABLES GLOBALES DEL PROGRAMA */
float g_flux = 0.0;
signed int g_u16ADC14Result = 0U;
float g_fADCResult = 0.0;
int g_imuestras_counter = 0;
int g_isonido_counter = 0;
float g_gmuestras_avg = 0.0;
float g_fpromedio = 0.0;
float g_vfsonido[5];
float g_vfmuestras[10];
int g_iverify = 0;
float g_fmemorizeavg = 400.0;
int g_ilamps = 1;
float g_fstillcrying = 0.0;


int main(void)
{
    /* Estableciendo las condiciones iniciales */
    P2->OUT = 0x0000;
    P2->OUT = 0x0000;
    P5->OUT = 0x0000;
    Setup();
    __disable_irq();
    SelectLamps(); /* El usuario elije la cantiad de lamparas*/
    __enable_irq();

    ParpadeoOn(); /* Parpadeo para indicar que esta listo para operar*/
    g_flux = OPT3001_getLux();
    EstadoInicial(); /* Establecer el estado inicial (Noche/Dia) */

    NVIC_SetPriority(PORT4_IRQn, 0);
    NVIC_EnableIRQ(PORT4_IRQn);

    /*Jose comentar este while*/
    while (1)
    {
        g_flux = OPT3001_getLux();

        if (g_flux > 15)
        {
            TIMER32_1->LOAD = 0x000249F0; //0.05s ---> a 3Mhz
            NVIC_SetPriority(T32_INT1_IRQn, 3);
            NVIC_EnableIRQ(T32_INT1_IRQn);
            NVIC_SetPriority(ADC14_IRQn, 3);
            NVIC_EnableIRQ(ADC14_IRQn);
        }
    }
}
