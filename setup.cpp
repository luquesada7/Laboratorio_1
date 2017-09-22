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



/* FUNCION DE CONFIGURACION DEL PROGRAMA */
void Setup()
{
    /* Detener el watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Inicializando vector de sonido con valor de ruido base*/
    for (int j = 0; j < 5; j++)
    {
        g_vfsonido[j] = 400.0;
    }

    /* Inicializando pines (OUTPUT)*/
    P2->DIR = BIT1 + BIT2 + BIT4 + BIT6;
    P5->DIR = BIT6;
    //P1->DIR = BIT0;

    /* Seleccionar pin del micrófono como entrada analógica*/
    P4->SEL0 = BIT3;
    P4->SEL1 = BIT3;
    P4->DIR &= ~BIT3;

    /* Configurando P3.5, P4.1 y P5.1 como entradas (botones)*/
    MAP_GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN5);
    MAP_GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN1);
    MAP_GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN1);

    /*Habilita el PIN 4.1 para recibir interrupciones*/
    P4->IE |= BIT1; // P4.1 interrupt enabled
    P4->IES |= BIT1; // P4.1 Hi/lo edge
    P4->IFG &= ~BIT1;

    /*Inicializa el TIMER32_1 en modo repetitivo*/
    TIMER32_1->CONTROL =
            TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0
                    | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE
                    | TIMER32_CONTROL_ENABLE;

    /*Inicializa el TIMER32_2 en modo one shot*/
    TIMER32_2->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE
            | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE
            | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;

    /* Configurando el ADC */
    ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7  | ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON | ADC14_CTL0_SHP;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC;
    ADC14->IER0 = ADC14_IER0_IE0;

    /* Inicializando la comuniación del I2C */
    Init_I2C_GPIO();
    I2C_init();

    /* Inicializando el sensor digital de luz ambiental OPT3001  */
    OPT3001_init();

    __delay_cycles(100000);

}

void SelectLamps()
/* Funcion que permite elegir la cantidad de lamparas que el
 usuario desea utilizar. El RGB se enciende según las siguientes
 condiciones: Verde = 1, Rojo = 2, Azul = 3. Con el boton S1 se
 elije la cantidad y con el boton S2 se sale de la función para
 seguir con la secuencia del programa. */
{
    int l_icounter = 0;
    bool l_bon = true;
    while (l_bon)
    {
        /* Boton 1 */
        if (!(P5IN & GPIO_PIN1))
        {
            /* Delay del boton */
            while (l_icounter < 1000)
            {
                l_icounter++;
            }
            l_icounter = 0;
            g_ilamps++;
            switch (g_ilamps)
            {
            case 1: /* Verde -> 5 Watts */
                P2->OUT = BIT4;
                __delay_cycles(1100000);
                P2->OUT ^= BIT4;
                break;
            case 2: /* Rojo -> 10 Watts */
                P2->OUT = BIT6;
                __delay_cycles(1100000);
                P2->OUT ^= BIT6;
                break;
            case 3: /* Azul -> 15 Watts */
                P5->OUT = BIT6;
                __delay_cycles(1100000);
                P5->OUT ^= BIT6;
                break;
            case 4: /* Inicializa la cantidad */
                g_ilamps = 0;
                break;
            }
        }
        /* Boton 2 */
        if (!(P3IN & GPIO_PIN5))
        {
            l_bon = false;
        }
    }
}

void MemoryLightsOn()
{
    /* Funcion que enciende la cantidad de lámparas elegidas por
     el usuario. */
    switch (g_ilamps)
    {
    case 1:
        P2->OUT = BIT4;
        break;
    case 2:
        P2->OUT = BIT6;
        break;
    case 3:
        P5->OUT = BIT6;
        break;
    }
}

void MemoryLightsOff()
/* Funcion que apaga la cantidad de lámparas elegidas por
 el usuario. */
{
    switch (g_ilamps)
    {
    case 1:
        P2->OUT = 0x0000;
        break;
    case 2:
        P2->OUT = 0x0000;
        break;
    case 3:
        P5->OUT = 0x0000;
        break;
    }
}

void MemoryLightsToggle()
{
    /* Funcion que cambia el estado de la cantidad de lámparas
     elegidas por  el usuario. */
    switch (g_ilamps)
    {
    case 1:
        P2->OUT ^= BIT4;
        break;
    case 2:
        P2->OUT ^= BIT6;
        break;
    case 3:
        P5->OUT ^= BIT6;
        break;
    }
}
