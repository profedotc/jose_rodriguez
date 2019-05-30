#ifndef _GOL_H_
#define _GOL_H_

#include <stdbool.h>

#define TAM_X 8
#define TAM_Y 8

struct gol
{
    bool world_a[TAM_X][TAM_Y];
    bool world_b[TAM_X][TAM_Y];
    bool *current;
    bool *next;
};

void gol_init(struct gol *self);    // Inicializa el mundo
void gol_print(struct gol *self);   // Imprime el mundo
void gol_step(struct gol *self);    // Itera el siguiente paso
int  gol_count_neighbors(struct gol *self, int i, int j);  // Cuenta el numero de vecinas vivas de una celda
bool gol_get_cell(struct gol *self, int i, int j); // Obtiene el estado de la celda

#endif
