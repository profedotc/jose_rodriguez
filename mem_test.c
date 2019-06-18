#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

int main()
{
    struct gol g;

    if(!gol_alloc(&g, TAM_X, TAM_Y)) {

        exit(EXIT_FAILURE);
    }

    gol_init(&g);

    for (int i = 0; i < ITERATIONS; i++)
        gol_step(&g);

    gol_free(&g);

    return EXIT_SUCCESS;
}
