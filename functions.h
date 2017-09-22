/*
 * -> Universidad de Costa Rica
 * -> Escuela de Ingenieria Electrica
 * -> IE -1119 Laboratorio de Programacion de Sistemas Incrustados
 * -> Estudiantes: Luisa Quesada Camacho [B35427], Jose Martinez Hernandez [B34024]
 * -> Laboratorio 1 : Introduccion a la Tarjeta de Desarrollo
 */

#ifndef __functions_H_
#define __functions_H_

/* VARIABLES GLOBALES DEL PROGRAMA */
extern float g_flux;
extern signed int g_u16ADC14Result;
extern float g_fADCResult;
extern int g_imuestras_counter;
extern int g_isonido_counter;
extern float g_gmuestras_avg;
extern float g_fpromedio;
extern float g_vfsonido[5];
extern float g_vfmuestras[10];
extern int g_iverify;
extern float g_fmemorizeavg;
extern int g_ilamps;
extern float g_fstillcrying;


/* DECLARACION DE FUNCIONES */
void ParpadeoOn(void);
void EstadoInicial();
void BufferMuestras();
void BufferSonido();
void StillCrying();


#endif /* __functions_H_ */
