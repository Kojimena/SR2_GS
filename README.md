# SR2_GS

Proyecto de gráficos basado en la biblioteca SDL2, y escrito utilizando las funcionalidades del estándar C++20.

![Demo](https://media.giphy.com/media/zs1nMIuxudF94lGJPl/giphy.gif)

## Renderizado BMP 

<img width="409" alt="Captura de pantalla 2023-08-18 a la(s) 17 56 07" src="https://github.com/Kojimena/SR2_GS/assets/77686378/7bdbca00-ec8b-417f-98ae-2099a395c4fe">
<img width="409" alt="Captura de pantalla 2023-08-18 a la(s) 18 03 52" src="https://github.com/Kojimena/SR2_GS/assets/77686378/8912fe0c-83ca-4c43-b325-6f301e8fa0a8">
<img width="433" alt="Captura de pantalla 2023-08-18 a la(s) 18 04 32" src="https://github.com/Kojimena/SR2_GS/assets/77686378/39d112de-c40d-4759-9723-ec9ff94dea60">

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
