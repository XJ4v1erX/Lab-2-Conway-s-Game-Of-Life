# Lab-2-Conway-s-Game-Of-Life
Este es un laboratorio2, donde el programa es el Conway's Game of Life, una simulación de autómatas celulares desarrollada en C++ con SDL2.

## GIFT
![Conway's Game of Life](Conway.gif)


## Reglas del Juego de la Vida
El juego sigue las siguientes reglas básicas:
1. Cualquier célula viva con menos de dos vecinos vivos muere, ya que se encuentra "sola".
2. Cualquier célula viva con dos o tres vecinos vivos sobrevive.
3. Cualquier célula viva con más de tres vecinos vivos muere, debido a la "sobrepoblación".
4. Cualquier célula muerta con exactamente tres vecinos vivos se convierte en una célula viva por "reproducción".

## Estructura del Proyecto

- `main.cpp`: Archivo principal que contiene la función `main` y la lógica principal del juego.
- `CMakeLists.txt`: Archivo de configuración de CMake para el proyecto.

## Uso
1. Clone este repositorio en su máquina local.
2. Asegúrese de tener SDL2 instalado en su sistema.
3. Compile el programa con CMake.
4. Ejecute el programa resultante.
5. Interactúe con el juego de la vida de Conway observando cómo evolucionan las células en la cuadrícula.


## Autor
- [Javier Ramírez]
