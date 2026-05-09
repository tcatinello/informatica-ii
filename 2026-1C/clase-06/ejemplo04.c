#include <stdio.h>

#define ARCHIVO "productos.dat"

typedef struct {
    int codigo;
    char nombre[31];
    float precio;
} Producto;

int buscarProd(const char*, int , Producto*);

int main() {

    Producto p;
    if (buscarProd(ARCHIVO, 45, &p)) {
        printf("Producto %d: %s, precio=%.2f\n", p.codigo, p.nombre, p.precio);
    } else {
        printf("Producto con CODIGO 45 no encontrado.\n");
        return 0;
    }
}

int buscarProd(const char *nom, int codBuscado, Producto *resultado) {
    FILE *fp = fopen(nom, "rb");
    if (!fp) {
        perror("Error al abrir el archivo");
        return 0;
    }
    Producto temp;
    while (fread(&temp, sizeof(Producto), 1, fp) == 1) {
        if (temp.codigo == codBuscado) {
            *resultado = temp;
            fclose(fp);
            return 1; // encontrado
        }
    }
    fclose(fp);
    return 0; // no encontrado
}
