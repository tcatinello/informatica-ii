# Clase 07

**Tema:** Gestión de memoria dinámica en C — pedir memoria en tiempo de ejecución con `malloc`, `calloc` y `realloc`, y por qué `free` importa de verdad.

## Ejemplos

**Tema:** Asignación de un vector dinámico

* **malloc.c** — Pide al usuario una cantidad `n` de elementos, reserva un vector de `n` enteros con `malloc(n * sizeof(int))`, lo carga con `1..n`, lo imprime y libera con `free`. Si `malloc` devuelve `NULL`, corta con un mensaje.
  Probar: compilar con `gcc -Wall malloc.c -o malloc` y ejecutar con un `n` chico (ej. 5). Probar con `n = 0` y con un `n` muy grande para ver cuándo `malloc` falla.

* **calloc.c** — Igual que `malloc.c`, pero reserva con `calloc(n, sizeof(int))`, que además **inicializa la memoria en cero**. El bucle de carga está comentado a propósito: como `calloc` ya puso ceros, el vector se imprime como `0, 0, 0, ...`.
  Probar: ejecutar y comparar la salida con la de `malloc.c` (donde, sin inicializar, se vería basura). Descomentar la línea `//ptr[i] = i + 1;` y ver que ahí sí se carga `1..n`.

**Tema:** Redimensionar un bloque ya asignado

* **realloc.c** — Reserva un vector con `malloc`, lo carga e imprime, después pide un **nuevo** tamaño y usa `realloc(ptr, n * sizeof(int))` para redimensionar el mismo bloque conservando los datos previos. Libera con `free` al final.
  Probar: ejecutar agrandando el tamaño (ej. de 5 a 10) y ver que los primeros valores se conservan. Repetir achicando (de 10 a 3) y ver que se trunca.

**Tema:** Para qué sirve realmente `free`

* **free.c** — Demo de fuga de memoria. Pide bloques de 300 MB en un bucle (con tope duro de 2000 MB), tocando una posición por página para forzar al SO a entregar RAM física, y frena con Enter en cada bloque. Al final de cada vuelta hace `free(p)`.
  Probar: compilar con `gcc -Wall -Wextra -O0 free.c -o free` y ejecutar `./free`. En **otra terminal** mirar la RAM con `watch -n 0.5 free -h` y dar Enter bloque a bloque: con `free(p)` activo la RAM usada se mantiene estable. Después **comentar la línea `free(p)`**, recompilar y volver a correr: la RAM trepa ~300 MB por bloque hasta el tope. Esa diferencia es lo que hace `free`. El tope de 2000 MB y el Enter por bloque son la red de seguridad para que la demo no cuelgue la máquina.
