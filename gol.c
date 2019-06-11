#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

enum world_type {CURRENT_WORLD, NEXT_WORLD};

static int  count_neighbors(struct gol *self, int i, int j);  // Cuenta el numero de vecinas vivas de una celda
static bool get_cell(struct gol *self, int i, int j); // Obtiene el estado de la celda

bool gol_alloc(struct gol *self, int size_x, int size_y) {

    for(int world = CURRENT_WORLD; world <= NEXT_WORLD; world++) {

        self->worlds[world] = malloc(size_x * sizeof(bool *)); // Reserva dinámica de vector de punteros a fila (bool *)

        if (!self->worlds[world]) {
            return false;
        }

        for (int row = 0; row < size_x; row++) {
            self->worlds[world][row] = malloc(size_y * sizeof(bool)); // Reserva dinámica de cada fila

        if (!self->worlds[world][row])
                return false;
        }
    }

    self->size_x = size_x;
    self->size_y = size_y;

    return true;
}

void gol_free(struct gol *self) {

    for (int world = CURRENT_WORLD; world <= NEXT_WORLD; world++) {
        for (int row=0; row < self->size_x; row++)
            free(self->worlds[world][row]);
        free(self->worlds[world]);
    }
}

void gol_init(struct gol *self)
{

    for (int i = 0; i < self->size_x; i++)
        for (int j = 0; j < self->size_y; j++)
            self->worlds[CURRENT_WORLD][i][j] = false;

    /* Inicializar con el patrón del glider:
    *           . # .
    *           . . #
    *           # # #
    */

    self->worlds[CURRENT_WORLD][0][1] = true;
    self->worlds[CURRENT_WORLD][1][2] = true;
    self->worlds[CURRENT_WORLD][2][0] = true;
    self->worlds[CURRENT_WORLD][2][1] = true;
    self->worlds[CURRENT_WORLD][2][2] = true;
}

void gol_print(struct gol *self)
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

    for (int i = 0; i < TAM_X; i++)
    {

        for (int j = 0; j < TAM_Y; j++)
        {

            printf(" %c", self->worlds[CURRENT_WORLD][i][j] ? '#' : '.');
        }

        printf("\n");
    }
}

void gol_step(struct gol *self)
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

    for (int i = 0; i < self->size_x; i++)
    {

        for (int j = 0; j < self->size_y; j++)
        {

            alives = count_neighbors(self, i, j);

            if(get_cell(self, i, j))

                self->worlds[NEXT_WORLD][i][j] = alives == 2 || alives == 3;

            else

                self->worlds[NEXT_WORLD][i][j] = alives == 3;
        }

        alives = 0;
    }

    // Intercambiamos mundos
    bool **swap = self->worlds[CURRENT_WORLD];
    self->worlds[CURRENT_WORLD] = self->worlds[NEXT_WORLD];
    self->worlds[NEXT_WORLD] = swap;
}

int count_neighbors(struct gol *self, int i, int j)
{
    // Devuelve el número de vecinos
    int counter = 0;

    counter += get_cell(self, i-1, j-1);
    counter += get_cell(self, i-1, j);
    counter += get_cell(self, i-1, j+1);
    counter += get_cell(self, i, j-1);
    counter += get_cell(self, i, j+1);
    counter += get_cell(self, i+1, j-1);
    counter += get_cell(self, i+1, j);
    counter += get_cell(self, i+1, j+1);

    return counter;

}

bool get_cell(struct gol *self, int i, int j)
{
    /*
    * Devuelve el estado de la célula de posición indicada
    * (¡cuidado con los límites del array!)
    */


    if ( i < 0 || i >= TAM_X || j < 0 || j >= TAM_Y)
        return 0;

    return self->worlds[CURRENT_WORLD][i][j];
}

