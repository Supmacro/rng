#include <stdint.h>

/* uniform distribution */
uint32_t st_rand_uniform(uint32_t , uint32_t );
/* gaussian distribution */
uint32_t st_rand_gaussian(uint32_t , uint32_t );
/* special distribution */
uint32_t st_rand_special(uint32_t , uint32_t );
/* Pareto distribution */
uint32_t st_rand_pareto(uint32_t , uint32_t );
/* Initialize thread-local RNG state */
void st_rand_thread_init(void);
