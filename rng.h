/* Copyright © 2019 wp.zhou <e615832610@qq.com>
 *    All rights reserved.
 *
 * Under certain requirements, the standard random function rand of C language can 
 * no longer meet our production needs. More different types of probability and 
 * statistics types may be used in the project.
 *
 * Here, I refer to http://xoshiro.di.unimi.it/ algorithm lists commonly used probability 
 * distribution functions such as uniform distribution, Gaussian distribution, Pareto 
 * distribution, etc.
 */

#include <stdint.h>

typedef struct {
    /* Pre-computed FP constants 
     * to avoid unnecessary conversions 
     * and divisions at runtime. */
    double          rng_iter_mult;
    double          rng_res_mult;
    double          rng_pct_mult;
    double          rng_pct_2_mult;

    /* number of iterations 
     * used for numbers generation */
    unsigned int    rng_iter;
    /* percentage of 'special' 
     * values to use (for special 
     * distribution) */
    unsigned int    rng_res;
    /* percentage of values to 
     * be treated as 'special' 
     * (for special distribution) */
    unsigned int    rng_pct;

    /* parameters for Pareto distribution */
    double          rng_pareto_h;
    double          rng_pareto_power;
} rng_rand_t;

typedef uint64_t rng_state_t[2];

rng_state_t rng_state ;


/* uniform distribution */
uint32_t rng_rand_uniform(uint32_t , uint32_t );
/* gaussian distribution */
uint32_t rng_rand_gaussian(uint32_t , uint32_t );
/* special distribution */
uint32_t rng_rand_special(uint32_t , uint32_t );
/* Pareto distribution */
uint32_t rng_rand_pareto(uint32_t , uint32_t );
/* Initialize thread-local RNG state */
void rng_random_seed_init(void);
