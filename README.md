# Laboratorio_1
Introducción a la tarjeta de desarrollo. 
Autores: José Pablo Martínez y Luisa Fernanda Quesada

Estre proyecto consiste en un prototipo para una lámpara de niños activada por sonido implementado en un MSP432401R y un BoosterPack MKII. Cuenta con las siguientes características:
-> Selección de 1, 2 o 3 lámparas.
-> Activación por un sonido (de 1 segundo) superior al 10% del ruido base de los últimos 4 segundos. 
-> La lámpara tiene un tiempo de encendido de 30 segundos, simulando 30 minutos del dispositivo real.
-> Implementa la modalidad de que si el ruido base se sigue superando a cinco segundos de terminar el tiempo de encendido de la lámpara esta se mantiene encendida por 30 segundos más y así recursivamente. 
-> Botón de control manual que permite encender la lámpara (se activa por 30 segundos y se apaga) o apagar la lámpara.

Archivos adjuntos:

main.cpp: contiene el código principal del laboratorio 

headers.h y headers.cpp: declaración y definición de las interrupciones que maneja el programa principal

setup.h y setup.cpp: contiene la declaración y definición de función de Setup principal del programa además de las funciones que manejan la cantidad de lámparas a través del programa. 

functions.h y functions.cpp: declaración y definición de funciones que manejan las muestras del micrófono, las condiciones de encendido de las lámparas, el blink de confirmación de encendido, la función del estado inicial del dispositivo y una función que determina si el niño sigue llorando cuando el tiempo de encendido va a finalizar.
