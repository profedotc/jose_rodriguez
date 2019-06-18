#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

/*
 * Macros para definir dinámicamente el tamaño de
 * los mundos
 */
#define TAM_X 8
#define TAM_Y 16

int main()
{
    int i = 0;
    struct gol g;


    if(!gol_alloc(&g, TAM_X, TAM_Y)) {

        fprintf(stderr, "ERROR: Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    gol_init(&g);

    do
    {
        printf("\033cIteration %d\n", i++);
        gol_print(&g);
        gol_step(&g);

    } while (getchar() != 'q');

    gol_free(&g);

    return EXIT_SUCCESS;
}
