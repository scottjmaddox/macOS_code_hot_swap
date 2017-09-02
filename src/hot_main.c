#include <stdlib.h> /* calloc */
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep */

#include "hot_main.h"

typedef struct
{
	int x;
} State;

void init(void **state_handle)
{
	// Allocate and zero the state
	State *state = calloc(1, sizeof(*state));

	*state_handle = state;
}

/**
 * Returns 1 if the application should keep running, or 0 if done.
 */
int main_loop(void *state, int *hot_swap)
{
	State *s = state;
	static int y = 0;
	s->x += 1;
	y += 1;
	printf("x: %d, y: %d\n", s->x, y);
	*hot_swap = (y % 2) == 0;

	sleep(1);

	return 1; // run forever
}
