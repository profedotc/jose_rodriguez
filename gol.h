#ifndef _GOL_H_
#define _GOL_H_

#include <stdbool.h>

#define TAM_X 8
#define TAM_Y 8

void gol_init(bool w[]);    // Inicializa el mundo
void gol_print(bool w[]);   // Imprime el mundo
void gol_step(bool w1[], bool w2[]);    // Itera el siguiente paso
int  gol_count_neighbors(bool w1[], int i, int j);  // Cuenta el numero de vecinas vivas de una celda
bool gol_get_cell(bool w1[], int i, int j); // Obtiene el estado de la celda

#endif
