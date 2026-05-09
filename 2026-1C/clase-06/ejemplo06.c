#include <stdio.h>

#define ARCHIVO "OrderLine_v4.csv"

int xcopiar_archivo(const char *, const char *) ;

int main() {
    if(xcopiar_archivo(ARCHIVO, "csvcopiado2.csv"))
        printf("Copia finalizada con exito.\n");
}

int xcopiar_archivo(const char *origen, const char *destino) {
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
    size_t bytesLeidos, bytesEscritos;

    while ((bytesLeidos = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        // Modificar buffer: reemplazar ',' por ';'
        for (size_t i = 0; i < bytesLeidos; ++i) {
            if (buffer[i] == ',') {
                buffer[i] = ';';
            }
        }
        bytesEscritos = fwrite(buffer, 1, bytesLeidos, fout);
        if (bytesEscritos < bytesLeidos) {
            perror("Error escritura");
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
