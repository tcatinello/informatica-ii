#include <stdio.h>

#define ARCHIVO "productos.dat"
int copiar_archivo(const char *, const char *) ;

int main() {
    if(copiar_archivo(ARCHIVO, "copiado.dat"))
        printf("Copia finalizada con exito.\n");
    return 0;
}

int copiar_archivo(const char *origen, const char *destino) {
    FILE *fin  = fopen(origen, "rb");
    if (!fin) {
        perror("Error abriendo archivo origen");
        return 0;
    }
    FILE *fout = fopen(destino, "wb");
    if (!fout) {
        perror("Error abriendo archivo destino");
        fclose(fin);
        return 0;
    }

    unsigned char buffer[4096];

    size_t bytesLeidos;
    size_t bytesEscritos;
    while ((bytesLeidos = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        // Escribir solo los bytes efectivamente leídos:
        bytesEscritos = fwrite(buffer, 1, bytesLeidos, fout);
        if (bytesEscritos < bytesLeidos) {
            perror("Error escribiendo en archivo destino");
            fclose(fin);
            fclose(fout);
            return 0;
        }
    }
    if (ferror(fin)) {
        perror("Error leyendo del archivo origen");
    }
    fclose(fin);
    fclose(fout);
    return 1;
}
