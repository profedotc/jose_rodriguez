#include <stdio.h>
#include "gol.h"

void gol_init(struct gol *self)
{
    // Poner el mundo a false

    for (bool *ini = self->world_a[0]; ini < self->world_a[0] + TAM_X*TAM_Y; ini++)

        *ini = false;

    /* Inicializar con el patrón del glider:
    *           . # .
    *           . . #
    *           # # #
    */

    *(self->world_a[0] + 1 + 0*TAM_Y) = true;
    *(self->world_a[0] + 2 + 1*TAM_Y) = true;
    *(self->world_a[0] + 0 + 2*TAM_Y) = true;
    *(self->world_a[0] + 1 + 2*TAM_Y) = true;
    *(self->world_a[0] + 2 + 2*TAM_Y) = true;

    self->current = self->world_a[0];
    self->next = self->world_b[0];
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

            printf(" %c", *(self->current + j + i*TAM_Y) ? '#' : '.');
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

    for (int i = 0; i < TAM_X; i++)
    {

        for (int j = 0; j < TAM_Y; j++)
        {

            alives = gol_count_neighbors(self, i, j);

            if(gol_get_cell(self, i, j))

                *(self->next + j + i*TAM_Y) = alives == 2 || alives == 3;

            else

                *(self->next + j + i*TAM_Y) = alives == 3;
        }

        alives = 0;
    }

    // Intercambiamos mundos
    bool *tmp_world_p = self->current;
    self->current = self->next;
    self->next = tmp_world_p;
}

int gol_count_neighbors(struct gol *self, int i, int j)
{
    // Devuelve el número de vecinos
    int counter = 0;

    counter += gol_get_cell(self, i-1, j-1);
    counter += gol_get_cell(self, i-1, j);
    counter += gol_get_cell(self, i-1, j+1);
    counter += gol_get_cell(self, i, j-1);
    counter += gol_get_cell(self, i, j+1);
    counter += gol_get_cell(self, i+1, j-1);
    counter += gol_get_cell(self, i+1, j);
    counter += gol_get_cell(self, i+1, j+1);

    return counter;

}

bool gol_get_cell(struct gol *self, int i, int j)
{
    /*
    * Devuelve el estado de la célula de posición indicada
    * (¡cuidado con los límites del array!)
    */

    if ( i < 0 || i >= TAM_X || j < 0 || j >= TAM_Y)

        return 0;

    return *(self->current + j + i*TAM_Y);
}
