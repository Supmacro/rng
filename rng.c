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
#include <stdlib.h>
#include <math.h>

#include "xoroshiro128plus.h"
#include "rng.h"


static rng_rand_t  local_Rng;

/* TLS rng_state_t rng_state CK_CC_CACHELINE; */
rng_state_t rng_state ;


/* Return a uniformly distributed pseudo-random 
 * 64-bit unsigned integer */
inline uint64_t rng_rand_uniform_uint64(void)
{
    return xoroshiro_next(rng_state);
}


/* Return a uniformly distributed pseudo-random 
 * double in the [0, 1) interval */
inline double rng_rand_uniform_double(void)
{
    const uint64_t x = rng_rand_uniform_uint64();
    const union { uint64_t i; double d; } u = { .i = UINT64_C(0x3FF) << 52 | x >> 12 }; 

    return u.d - 1.0;
}


/*>> uniform distribution >>*/
uint32_t rng_rand_uniform(uint32_t a, uint32_t b)
{
    return a + rng_rand_uniform_double() * (b - a + 1);
}


/*>> gaussian distribution >>*/
uint32_t rng_rand_gaussian(uint32_t a, uint32_t b)
{
    double       sum;
    double       t;
    unsigned int i;

    t = b - a + 1;
    for(i=0, sum=0; i < local_Rng.rng_iter; i++){
        sum += rng_rand_uniform_double() * t;
    }

    return a + (uint32_t) (sum * local_Rng.rng_iter_mult) ;
}


/*>> Pareto distribution >>*/
uint32_t rng_rand_pareto(uint32_t a, uint32_t b)
{
    return a + (uint32_t) ((b - a + 1) *
                         pow(rng_rand_uniform_double(), local_Rng.rng_pareto_power));
}


/*>> 'special' distribution >>*/
uint32_t rng_rand_special(uint32_t a, uint32_t b)
{
    double       sum;
    double       t;
    double       range_size;
    double       res;
    double       d;
    double       rnd;
    unsigned int i;

    t = b - a;

    /* Increase range size for special values. */
    range_size = t * local_Rng.rng_res_mult;
    /* Generate uniformly distributed one at this stage  */
    rnd = rng_rand_uniform_double(); /* Random double in the [0, 1) interval */
    /* Random integer in the [0, range_size) interval */
    res = rnd * range_size;
    /* Use gaussian distribution for (100 - local_Rng.rng_res) 
     * percent of all generated values.*/
    if (res < t)
    {
        sum = 0.0;

        for(i = 0; i < local_Rng.rng_iter; i++){
            sum += rng_rand_uniform_double();
        }

        return a + sum * t * local_Rng.rng_iter_mult;
    }
    /* 
     *  For the remaining local_Rng.rng_res percent of values use the uniform
     *  distribution. We map previously generated random double in the [0, 1)
     *  interval to the local_Rng.rng_pct percent part of the [a, b] interval. Then we move
     *  the resulting value in the [0, (b-a) * (local_Rng.rng_pct / 100)] interval to the
     *  center of the original interval [a, b]. 
     */
    d = t * local_Rng.rng_pct_mult;
    res = rnd * (d + 1);
    res += t / 2 - t * local_Rng.rng_pct_2_mult;

    return a + (uint32_t) res;
}


void rng_random_seed_init(void)
{
    local_Rng.rng_iter = 12;
    local_Rng.rng_iter_mult = 1.0 / local_Rng.rng_iter;

    local_Rng.rng_res = 75;
    local_Rng.rng_res_mult = 100.0 / (100.0 - local_Rng.rng_res); 

    local_Rng.rng_pct = 1;
    local_Rng.rng_pct_mult = local_Rng.rng_pct / 100.0;
    local_Rng.rng_pct_2_mult = local_Rng.rng_pct / 200.0;

    local_Rng.rng_pareto_h = 0.2;
    local_Rng.rng_pareto_power = log(local_Rng.rng_pareto_h) / log(1.0 - local_Rng.rng_pareto_h);

    /* We use libc PRNG to seed xoroshiro128+ */
    rng_state[0] = (((uint64_t) random()) << 32) |
        (((uint64_t) random()) & UINT32_MAX);
    rng_state[1] = (((uint64_t) random()) << 32) |
        (((uint64_t) random()) & UINT32_MAX);
}


