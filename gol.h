#ifndef GOL_H_INCLUDED
#define GOL_H_INCLUDED

#include <stdbool.h>

#define NUM_WORLDS 2
#define ITERATIONS 500

struct gol; // Declaración adelantada (forward declaration)

struct gol * gol_alloc(int x, int y);	// Constructor. Tengo que pasar el tamaño de los mundos, para poder reservar memoria dinámicamente
void gol_init(struct gol *self);	// Inicializa el mundo.
void gol_print(struct gol *self);	// Imprime el mundo
void gol_step(struct gol *self);	// Itera el siguiente paso
void gol_free(struct gol *self);	// Destructor

#endif // GOL_H_INCLUDED
