# Clase 06

**Tema:** Manejo avanzado de archivos en C — ordenamiento interno vs externo, búsqueda de palabras en texto, búsqueda binaria con puntero a struct, copia binaria con buffer, transformación de datos al vuelo, y un **ejercicio** de benchmark del impacto del tamaño de buffer en el throughput.

## Ejemplos

**Tema:** Ordenamiento sobre archivos (interno vs externo)

* **ejemplo01.c** — Ordenamiento *interno* (en RAM): lee todos los productos de `productos_original.dat` a un vector con un solo `fread`, los ordena en memoria con bubble sort por `precio`, y reescribe el resultado a `productos2.dat`. Usa `fseek(fp, 0, SEEK_END)` + `ftell` para calcular cuántos registros hay antes de cargar.
  Probar: ejecutar y verificar que `productos2.dat` está ordenado por precio. Cambiar `MAX_PRODUCTOS` a un valor menor que la cantidad real de registros y observar el mensaje de advertencia. Cambiar el criterio de orden a `codigo` (línea 68) y ver el resultado.

* **ejemplo02.c** — Ordenamiento *externo* (sobre el archivo): no carga nada en RAM. Abre `productos.dat` en modo `"rb+"` (lectura y escritura simultánea), recorre el archivo con `fseek`/`fread`/`fwrite` aplicando bubble sort directamente sobre disco. Útil cuando el archivo no entra en memoria.
  Probar: ejecutar y notar que `productos.dat` queda modificado *in-place* (no genera un archivo nuevo). Comparar el tiempo con ejemplo01 (intuitivamente debería ser más lento por la cantidad de syscalls — buen disparador para discusión). Para repetir el experimento, restaurar el archivo desde `productos_original.dat` (`cp productos_original.dat productos.dat`).

**Tema:** Búsqueda de palabra en archivo de texto

* **ejemplo03.c** — Lee `logs_ups.txt` carácter por carácter con `fgetc`, acumula caracteres alfanuméricos en un buffer, y al encontrar un separador (espacio, signo, etc.) compara la palabra con `"UPM"` usando `strcmp`. Implementa `esLetraONum()` como tokenizador manual.
  Probar: ejecutar tal cual y ver cuántas veces aparece `"UPM"`. Cambiar el `objetivo` (línea 17) por otra palabra del log (`"Bypass"`, `"alarma"`). Notar que `strcmp` distingue mayúsculas — probar con `"upm"` para ver que devuelve 0 ocurrencias.

**Tema:** Búsqueda binaria con puntero a struct

* **ejemplo04.c** — Función `buscarProd()` que recibe el nombre del archivo, un código a buscar, y un puntero a `Producto` donde escribir el resultado. Recorre `productos.dat` con `fread` registro por registro, y al encontrar coincidencia copia la struct al puntero recibido y devuelve `1`.
  Probar: ejecutar y ver que encuentra el producto con código 45. Cambiar el código buscado (línea 16) por uno que no exista (ej. `9999`) y ver el mensaje de no encontrado. Discutir por qué es importante que la función reciba un *puntero* y no que devuelva un `Producto` por valor (eficiencia + posibilidad de señalar "no encontrado" sin valor centinela).

**Tema:** Copia binaria con buffer

* **ejemplo05.c** — Copia `productos.dat` a `copiado.dat` leyendo en bloques de 4096 bytes (`fread` + `fwrite`). Maneja correctamente el caso de lectura parcial (último bloque) escribiendo solo `bytesLeidos`. Verifica `ferror` al final.
  Probar: ejecutar y comparar `copiado.dat` con `productos.dat` usando `cmp productos.dat copiado.dat` (sin output = idénticos) o `md5sum`.

* **ejemplo06.c** — Variante de copia que *transforma* el contenido al vuelo: lee `OrderLine_v4.csv` en bloques, reemplaza cada `,` por `;` dentro del buffer en memoria, y escribe el resultado a `csvcopiado2.csv`. Demuestra que el "stream de copia" es un buen lugar para aplicar filtros sin cargar el archivo entero.
  Probar: ejecutar y abrir `csvcopiado2.csv` en LibreOffice/Excel para confirmar que reconoce `;` como separador. Modificar la transformación: por ejemplo, pasar todas las letras a mayúsculas (`if (buffer[i] >= 'a' && buffer[i] <= 'z') buffer[i] -= 32;`).

## Ejercicio — Benchmark del tamaño de buffer

Tomando como base el patrón del `ejemplo05.c`, escribir un programa `benchmark_copy.c` que **copie un archivo origen variando el tamaño del buffer** (por ejemplo: 1 B, 64 B, 512 B, 1 KB, 4 KB, 16 KB, 64 KB, 256 KB, 1 MB, 4 MB, 16 MB) y mida cuánto tarda cada copia. Imprimir una tabla con buffer, tiempo y throughput (MB/s).

### Tips para resolverlo

**Generar archivos de prueba de distintos tamaños** (no committear estos archivos al repo, son locales):
```bash
# Linux / Git Bash
dd if=/dev/urandom of=test.bin bs=1M count=10     # 10 MB
dd if=/dev/urandom of=test50.bin bs=1M count=50   # 50 MB
dd if=/dev/urandom of=test100.bin bs=1M count=100 # 100 MB
```
```powershell
# Windows PowerShell
fsutil file createnew test.bin 10485760           # 10 MB de ceros
```

**Medir tiempo con buena resolución:** `clock_gettime(CLOCK_MONOTONIC, ...)` de `<time.h>` da resolución de nanosegundos y no se ve afectada por cambios del reloj del sistema. La diferencia entre dos lecturas da el tiempo real transcurrido.

**Cuidado con el cache del sistema operativo:** si corrés el benchmark dos veces seguidas, la segunda corrida puede ser mucho más rápida porque el archivo ya está cacheado en RAM (page cache). Para medir "en frío" en Linux, vaciar el cache entre corridas (requiere sudo):
```bash
sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'
```

**Resultado esperado:** la curva de throughput suele subir rápido hasta 4 KB y después aplanarse. Buffers muy chicos (1 B, 64 B) son notoriamente lentos porque generan muchas llamadas a la librería estándar. Buffers muy grandes (16 MB) no aportan mejora y solo gastan memoria.

**Tip extra (optativo):** la familia `fread`/`fwrite` ya hace buffering interno por defecto (~4 KB en glibc). Por debajo, usa las llamadas POSIX `read`/`write` (`<unistd.h>`), que copian bytes directamente sin buffer intermedio. Si tienen curiosidad, pueden probar reemplazar `fread`/`fwrite` por `read`/`write` y comparar la curva.
