/*
 * free.c — Gestión de memoria dinámica: malloc, páginas y free
 *
 * Pide bloques de memoria en un bucle (tope duro de 2000 MB) y
 * libera cada bloque con free(). Frena con Enter en cada paso
 * para poder observar la RAM en otra terminal.
 *
 * Compilar:  gcc -Wall -Wextra -O0 free.c -o free
 * Ejecutar:  ./free
 *
 * En OTRA terminal, en paralelo, mirar la RAM:
 *     watch -n 0.5 free -h
 */

#include <stdio.h>
#include <stdlib.h>

#define BLOQUE_MB   300                 /* MB por iteracion            */
#define LIMITE_MB   2000                /* tope duro de la demo        */
#define PAGINA      4096                /* tamano de pagina de memoria */

int main(void)
{
    size_t mb = 0;
    size_t bloque = (size_t)BLOQUE_MB * 1024 * 1024;

    printf("%zu MB asignados. Enter para empezar...", mb);
    getchar();

    while (mb < LIMITE_MB) {
        char *p = malloc(bloque);
        if (!p) {
            printf("malloc fallo en %zu MB asignados\n", mb);
            return 1;
        }

        /* Tocar una posicion por pagina obliga al SO a entregar
         * RAM fisica de verdad (si no, la asignacion es perezosa). */
        for (size_t i = 0; i < bloque; i += PAGINA) {
            p[i] = 0;
        }

        mb += BLOQUE_MB;
        printf("%zu MB asignados. Enter para continuar...", mb);
        getchar();

        free(p);
    }

    printf("Llegamos al tope de la demo (%d MB).\n", LIMITE_MB);
    return 0;
}
