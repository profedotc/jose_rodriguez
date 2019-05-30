#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

int main()
{
    int i = 0;

    // Reserva estática de memoria: el compilador reserva espacio en la memoria
    // en la declaración del tipo struct gol. Conoce el espacio porque conoce
    // la definición de la estructura (el tipo) por la cabecera.
    struct gol g;

    // inicializa el mundo
    gol_init(&g);

    do
    {
        printf("\033cIteration %d\n", i++);
        // Imprime el mundo
        gol_print(&g);

        // Itera
        // Paso por referencia: las estructuras pueden copiarse por valor, pero
        // se usa el paso por referencia porque si la estructura contiene muchos
        // campos, la copia desperdicia memoria.
        gol_step(&g);

    }
    while (getchar() != 'q');

    return EXIT_SUCCESS;
}
