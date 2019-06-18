#include <stdio.h>
#include <stdlib.h>
#include "gol.h"

enum world_type {CURRENT_WORLD, NEXT_WORLD};

struct gol {
	int size_x;
	int size_y;
	bool *mem;
	bool *worlds[NUM_WORLDS];
};

/*
 * Macro funcional para acceder a las celdas
 * Se define aquí porque no queremos que el usuario de mi librería disponga de ella
 * Sólo de uso en este fichero, gol.c
 * Los paréntesis es para evitar un funcionamiento erróneo de la macro. Supongamos 
 * el siguiente ejemplo:
 *
 * #define CELL(self, wtype, i, j)
 *
 * CELL(self, wtype, j, i + 2)
 *
 * Esa macro se sustituiría por este código:
 *
 * self->worlds[wtype][j + i + 2 * self->size_y]
 *
 * es decir (orden de precedencia de operadores),
 *
 * self->worlds[wtype][j + i + (2 * self->size_y)]
 *
 * lo que desemboca en un error porque no es la posición de memoria
 * a la que queremos acceder
 *
 */

#define CELL(self, wtype, i, j)	((self)->worlds[(wtype)][(j) + (i) * self->size_y])

static int  count_neighbors(struct gol *self, int i, int j);  // Cuenta el numero de vecinas vivas de una celda
static bool get_cell(const struct gol *self, enum world_type wtype, int i, int j);
static void set_cell(struct gol *self, enum world_type wtype, int i, int j, bool value);
static void fix_coords(const struct gol *self, int *i, int *j);

struct gol * gol_alloc(int size_x, int size_y) {

    struct gol *self = (struct gol *)malloc(sizeof(struct gol));
    if (!self)
	return NULL;

    self->mem = (bool *)malloc(NUM_WORLDS * size_x * size_y * sizeof(bool)); // Reserva dinámica de bloque de memoria por mundo

    if (!self->mem) {
        free(self);
        return NULL;
    }

    self->size_x = size_x;
    self->size_y = size_y;
    self->worlds[CURRENT_WORLD] = self->mem;
    self->worlds[NEXT_WORLD] = self->mem + size_x * size_y;

    return self;
}

void gol_free(struct gol *self) {

    free(self->mem);
    free(self);
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

    for (int i = 0; i < self->size_x; i++)
    {

        for (int j = 0; j < self->size_y; j++)
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

static int count_neighbors(struct gol *self, int i, int j)
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
    return CELL(self, wtype, i, j);
}

static void set_cell(struct gol *self, enum world_type wtype, int i, int j, bool value)
{
    /*
    * Setea la celda con el valor del parámetro value
    */

    fix_coords(self, &i, &j);
    CELL(self, wtype, i, j) = value;
}
