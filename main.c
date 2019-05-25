/* CABECERAS */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* MACROS */
#define TAM_X 8
#define TAM_Y 8


// Esta expresión se repite mucho a lo largo del código:
// pointer + i + j
// #define POINTER(p, i, j) p + i + j ??
// No se puede porque en el preprocesado, antes del tiempo de compilación,
// desconocemos el valor de las variables.

/* PROTOTIPOS DE FUNCIONES */

// NOTA: se usa bool w[] y no bool *w porque se quiere hacer notar que la función trabaja con array,
//       pero ambas son igualmente sintaxis validas. Legibilidad del código
void gol_init(bool w[]);
void gol_print(bool w[]);
void gol_step(bool w1[], bool w2[]);
int gol_count_neighbors(bool w1[], int i, int j);
bool gol_get_cell(bool w1[], int i, int j);
void gol_copy(bool w1[], bool w2[]);

/* MAIN */
int main()
{
	int i = 0;
	// Declara dos mundos
	bool world_a[TAM_X][TAM_Y];
	bool world_b[TAM_X][TAM_Y];

	// inicializa el mundo
    // gol_init(world_a);
    // gol_init(&world_a);
    // gol_init(&world_a[0]);
    // El compilador se queja:  "warning: passing argument 1 of 'gol_print' from incompatible pointer type [-Wincompatible-pointer-types]
    //                          "note: expected '_Bool *' but argument is of type '_Bool (*)[8]'
    // La función espera una dirección de memoria y le estamos pasando una array. ¿Pero el nombre de un array no es una dirección?
	gol_init(&world_a[0][0]);

	do {
		printf("\033cIteration %d\n", i++);
		// Imprime el mundo
		gol_print(&world_a[0][0]);

		// Itera
		gol_step(&world_a[0][0], &world_b[0][0]);
	} while (getchar() != 'q');

	return EXIT_SUCCESS;
}

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
	 * - Copiar el mundo auxiliar sobre el mundo principal
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

	 gol_copy(w1, w2);
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

void gol_copy(bool orig[], bool aux[])
{
	// copia el mundo segundo mundo sobre el primero

	for (int i =0; i < TAM_X; i++)  {

        for(int j=0; j < TAM_Y; j++) {

            *(orig + j + i*TAM_Y) = *(aux + j + i*TAM_Y);
        }
	}
}
