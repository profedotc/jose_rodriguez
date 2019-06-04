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

#endif
