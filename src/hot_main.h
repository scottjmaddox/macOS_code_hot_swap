#ifndef HOT_MAIN_H
#define HOT_MAIN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void init_t(void **state_handle);
init_t init;

typedef int main_loop_t(void *state, int *hot_swap);
main_loop_t main_loop;

#ifdef __cplusplus
}
#endif

#endif /* HOT_MAIN_H */
