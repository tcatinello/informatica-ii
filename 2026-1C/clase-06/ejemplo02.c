#include <stdio.h>

#define ARCHIVO "productos.dat"

typedef struct {
    int codigo;
    char nombre[31];
    float precio;
} Producto;

void mostrarArchivo(const char *nombreArchivo);
void bubbleSortArchivo(const char *nombreArchivo);

int main() {
    printf("----- Archivo ORIGINAL -----\n");
    mostrarArchivo(ARCHIVO);

    getchar();  // Esperar antes de ordenar

    bubbleSortArchivo(ARCHIVO);

    printf("\n----- Archivo ORDENADO en disco -----\n");
    mostrarArchivo(ARCHIVO);

    return 0;
}

void mostrarArchivo(const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "rb");
    if (!f) {
        perror("Error al abrir el archivo");
        return;
    }

    Producto p;
    while (fread(&p, sizeof(Producto), 1, f) == 1) {
        printf("Codigo: %3d | Nombre: %-15s | Precio: $%7.2f\n", p.codigo, p.nombre, p.precio);
    }

    fclose(f);
}

void bubbleSortArchivo(const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "rb+");  // lectura y escritura en el mismo archivo
    if (!f) {
        perror("Error al abrir el archivo para ordenamiento");
        return;
    }
    fseek(f, 0, SEEK_END);
    long tam_bytes = ftell(f);
    int total = tam_bytes / sizeof(Producto);
    Producto a, b;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            // Leer producto en posición j
            fseek(f, j * sizeof(Producto), SEEK_SET);
            fread(&a, sizeof(Producto), 1, f);
            // Leer producto en posición j+1
            fread(&b, sizeof(Producto), 1, f);
            if (a.precio > b.precio) {
                // Intercambiar: escribir b en j, a en j+1
                fseek(f, j * sizeof(Producto), SEEK_SET);
                fwrite(&b, sizeof(Producto), 1, f);
                fwrite(&a, sizeof(Producto), 1, f);
            }
        }
    }
    fclose(f);
}
