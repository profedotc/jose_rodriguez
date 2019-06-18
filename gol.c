#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

enum world_type {CURRENT_WORLD, NEXT_WORLD};

static int  count_neighbors(struct gol *self, int i, int j);  // Cuenta el numero de vecinas vivas de una celda
static bool get_cell(const struct gol *self, enum world_type wtype, int i, int j);
static void set_cell(struct gol *self, enum world_type wtype, int i, int j, bool value);
static void fix_coords(const struct gol *self, int *i, int *j);

bool gol_alloc(struct gol *self, int size_x, int size_y) {

    for (int world = CURRENT_WORLD; world <= NEXT_WORLD; world++) {

        self->worlds[world] = (bool *)malloc(size_x * size_y * sizeof(bool)); // Reserva dinámica de vector de punteros a fila (bool *)

        if (!self->worlds[world]) {
            return false;
        }
    }

    self->size_x = size_x;
    self->size_y = size_y;

    return true;
}

void gol_free(struct gol *self) {

    for (int world = CURRENT_WORLD; world <= NEXT_WORLD; world++)
        free(self->worlds[world]);
}

void gol_init(struct gol *self)
{

    for (int i = 0; i < self->size_x; i++)
        for (int j = 0; j < self->size_y; j++)
            set_cell(self, CURRENT_WORLD, i, j, false);

    /* Inicializar con el patrón del glider:
    *           . # .
    *           . . #
    *           # # #
    */

    set_cell(self, CURRENT_WORLD, 0, 1, true);
    set_cell(self, CURRENT_WORLD, 1, 2, true);
    set_cell(self, CURRENT_WORLD, 2, 0, true);
    set_cell(self, CURRENT_WORLD, 2, 1, true);
    set_cell(self, CURRENT_WORLD, 2, 2, true);
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

            printf(" %c", get_cell(self, CURRENT_WORLD, i, j) ? '#' : '.');
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

            if (get_cell(self, CURRENT_WORLD, i, j)) {

                set_cell(self, NEXT_WORLD, i, j, alives == 2 || alives == 3);

	    } else {

                set_cell(self, NEXT_WORLD, i, j, alives == 3);
	    }
        }

        alives = 0;
    }

    // Intercambiamos mundos
    bool *swap = self->worlds[CURRENT_WORLD];
    self->worlds[CURRENT_WORLD] = self->worlds[NEXT_WORLD];
    self->worlds[NEXT_WORLD] = swap;
}

int count_neighbors(struct gol *self, int i, int j)
{
    // Devuelve el número de vecinos
    int counter = 0;

    counter += get_cell(self, CURRENT_WORLD, i-1, j-1);
    counter += get_cell(self, CURRENT_WORLD, i-1, j);
    counter += get_cell(self, CURRENT_WORLD, i-1, j+1);
    counter += get_cell(self, CURRENT_WORLD, i, j-1);
    counter += get_cell(self, CURRENT_WORLD, i, j+1);
    counter += get_cell(self, CURRENT_WORLD, i+1, j-1);
    counter += get_cell(self, CURRENT_WORLD, i+1, j);
    counter += get_cell(self, CURRENT_WORLD, i+1, j+1);

    return counter;
}

static void fix_coords(const struct gol *self, int *i, int *j) {

    if (*i >= self->size_x)
	*i = 0;
    else if (*i < 0)
	*i = self->size_x - 1;

    if (*j >= self->size_y)
	*j = 0;
    else if (*j < 0)
	*j = self->size_y - 1;
}

static bool get_cell(const struct gol *self, enum world_type wtype, int i, int j)
{
    /*
    * Devuelve el estado de la célula de posición indicada
    * (¡cuidado con los límites del array!)
    */

    fix_coords(self, &i, &j);
    //return *(self->worlds[wtype] + j + i * self->size_y);
    return self->worlds[wtype][j + i * self->size_y];
}

static void set_cell(struct gol *self, enum world_type wtype, int i, int j, bool value)
{

    fix_coords(self, &i, &j);
    //*(self->worlds[wtype] + j + i * self->size_y) = value;
    self->worlds[wtype][j + i * self->size_y] = value;
}
