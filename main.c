#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

int main()
{
	int i = 0;

	bool world_a[TAM_X][TAM_Y];
	bool world_b[TAM_X][TAM_Y];
	bool *current = &world_a[0][0];
	bool *next = &world_b[0][0];
	bool *tmp_world_p;

	// inicializa el mundo
	gol_init(current);

	do {
		printf("\033cIteration %d\n", i++);
		// Imprime el mundo
		gol_print(current);

		// Itera
		gol_step(current, next);

		// Intercambiamos mundos
        tmp_world_p = current;
        current = next;
        next = tmp_world_p;

	} while (getchar() != 'q');

	return EXIT_SUCCESS;
}
