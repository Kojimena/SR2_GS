# SR2_GS

Proyecto de gráficos basado en la biblioteca SDL2, y escrito utilizando las funcionalidades del estándar C++20.

![Demo](https://media.giphy.com/media/zs1nMIuxudF94lGJPl/giphy.gif)

## Requisitos

Para poder construir y ejecutar este proyecto, necesitas tener instalado lo siguiente en tu sistema:

- CMake versión 3.25 o superior.
- Un compilador que soporte C++20.
- SDL2

## Estructura del proyecto

El proyecto está organizado en las siguientes carpetas:

- `src/`: Donde se encuentran todos los archivos de código fuente (.cpp).
- `include/`: Contiene todos los archivos de cabecera (.h o .hpp).

## Compilación

Este proyecto usa CMake para su construcción. Puedes compilar el proyecto siguiendo estos pasos:

1. Crear un directorio de compilación (p.ej., `build/`):

    ```bash
    mkdir build
    cd build
    ```

2. Configurar el proyecto con CMake. Desde el directorio de compilación que acabas de crear, ejecuta:

    ```bash
    cmake ..
    ```

3. Compilar el proyecto:

    ```bash
    make
    ```

## Ejecución

Después de haber compilado el proyecto, puedes ejecutar el programa utilizando el ejecutable `SR2_GS` en el directorio de compilación:

```bash
./SR2_GS
