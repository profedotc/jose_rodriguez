#include <stdio.h>
#include "gol.h"

void gol_init(bool world[])
{
	// Poner el mundo a false
	/*
	 * Array: Reserva de memoria CONTINUA de forma estática
	 *
	 * Existen diferentes forma de recorrer un array.
	 * En las líneas de abajo se usa la ARITMÉTICA DE PUNTEROS
	 * Inicializamos un puntero con la dirección del primer elemento del array y iteramos sobre el
	 * hasta recorrer todo el array de TAM_X * TAM_Y elementos
	 */

	for (bool * ini = world; ini < world + TAM_X*TAM_Y; ini++)

        *ini = false;

	/* Inicializar con el patrón del glider:
	 *           . # .
	 *           . . #
	 *           # # #
	 */

	 /*
      * Nomenclatura world[i][j]. No funciona. El compilador no sabe donde en acaba el array
      * porque le falta el dato del tamaño
      * ERROR: subscripted value is neither array nor pointer nor vector
      *
      * Si este método recibiera como parámetro: bool world[][TAM_Y], sí valdría. El tamaño de
      * la fila lo deduce del tipo bool.
      */

	 *(world + 1 + 0*TAM_Y) = true;
	 *(world + 2 + 1*TAM_Y) = true;
	 *(world + 0 + 2*TAM_Y) = true;
	 *(world + 1 + 2*TAM_Y) = true;
	 *(world + 2 + 2*TAM_Y) = true;
}

void gol_print(bool w[])
{
	// Imprimir el mundo por consola. Sugerencia:
	/*
	 *     . # . . . . . . . .
	 *     . . # . . . . . . .
	 *     # # # . . . . . . .
	 *     . . . . . . . . . .
	 *     . . . . . . . . . .
	 *     . . . . . . . . . .
	 *     . . . . . . . . . .
	 *     . . . . . . . . . .
	 *     . . . . . . . . . .
	 *     . . . . . . . . . .
	 */

    for (int i = 0; i < TAM_X; i++) {

        for (int j = 0; j < TAM_Y; j++) {

            printf(" %c", *(w + j + i*TAM_Y) ? '#' : '.');
        }

        printf("\n");
    }
}

void gol_step(bool w1[], bool w2[])
{
	/*
	 *
	 * - Recorrer el mundo célula por célula comprobando si nace, sobrevive
	 *   o muere.
	 *
	 * - No se puede cambiar el estado del mundo a la vez que se recorre:
	 *   Usar un mundo auxiliar para guardar el siguiente estado.
	 *
	 */

	 int alives = 0;

	 for (int i = 0; i < TAM_X; i++) {

        for (int j = 0; j < TAM_Y; j++) {

            alives = gol_count_neighbors(w1, i, j);

            if(gol_get_cell(w1, i, j))

                *(w2 + j + i*TAM_Y) = alives == 2 || alives == 3;

            else

                *(w2 + j + i*TAM_Y) = alives == 3;
        }

        alives = 0;
	 }
}

int gol_count_neighbors(bool w[], int i, int j)
{
	// Devuelve el número de vecinos
	int counter = 0;

	counter += gol_get_cell(w, i-1, j-1);
	counter += gol_get_cell(w, i-1, j);
	counter += gol_get_cell(w, i-1, j+1);
	counter += gol_get_cell(w, i, j-1);
	counter += gol_get_cell(w, i, j+1);
	counter += gol_get_cell(w, i+1, j-1);
	counter += gol_get_cell(w, i+1, j);
	counter += gol_get_cell(w, i+1, j+1);

	return counter;

}

bool gol_get_cell(bool w[], int i, int j)
{
	/*
	 * Devuelve el estado de la célula de posición indicada
	 * (¡cuidado con los límites del array!)
	 */

	 if ( i < 0 || i >= TAM_X || j < 0 || j >= TAM_Y)

        return 0;

     return *(w + j + i*TAM_Y);
}
