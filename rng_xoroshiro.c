#include <stdlib.h>
#include <math.h>

#include "xoroshiro128plus.h"


/* Pre-computed FP constants to avoid unnecessary 
 * conversions and divisions at runtime. */
static double st_rand_iter_mult;
static double st_rand_res_mult;
static double st_rand_pct_mult;
static double st_rand_pct_2_mult;

/* number of iterations used for numbers generation */
static unsigned int st_rand_iter ;

/* percentage of 'special' values to use (for special distribution) */
static unsigned int st_rand_res;

/* percentage of values to be treated as 'special' (for special distribution) */
static unsigned int st_rand_pct;

/* parameters for Pareto distribution */
static double st_pareto_h;     /* parameter h */
static double st_pareto_power; /* parameter pre-calculated by h */

typedef uint64_t st_rng_state_t[2];

/* TLS st_rng_state_t st_rng_state CK_CC_CACHELINE; */
st_rng_state_t st_rng_state ;

/* Return a uniformly distributed pseudo-random 64-bit unsigned integer */
inline uint64_t st_rand_uniform_uint64(void)
{
    return xoroshiro_next(st_rng_state);
}

/* Return a uniformly distributed pseudo-random double in the [0, 1) interval */
inline double st_rand_uniform_double(void)
{
    const uint64_t x = st_rand_uniform_uint64();
    const union { uint64_t i; double d; } u = { .i = UINT64_C(0x3FF) << 52 | x >> 12 }; 

    return u.d - 1.0;
}

void st_rand_init()
{
    st_rand_iter = 12;
    st_rand_iter_mult = 1.0 / st_rand_iter;

    st_rand_res = 75;
    st_rand_res_mult = 100.0 / (100.0 - st_rand_res); 

    st_rand_pct = 1;
    st_rand_pct_mult = st_rand_pct / 100.0;
    st_rand_pct_2_mult = st_rand_pct / 200.0;

    st_pareto_h = 0.2;
    st_pareto_power = log(st_pareto_h) / log(1.0 - st_pareto_h);
}

/* uniform distribution */
uint32_t st_rand_uniform(uint32_t a, uint32_t b)
{
    return a + st_rand_uniform_double() * (b - a + 1);
}

/* gaussian distribution */
uint32_t st_rand_gaussian(uint32_t a, uint32_t b)
{
    double       sum;
    double       t;
    unsigned int i;

    t = b - a + 1;
    for(i=0, sum=0; i < st_rand_iter; i++){
        sum += st_rand_uniform_double() * t;
    }

    return a + (uint32_t) (sum * st_rand_iter_mult) ;
}

/* 'special' distribution */
uint32_t st_rand_special(uint32_t a, uint32_t b)
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
    range_size = t * st_rand_res_mult;
    /* Generate uniformly distributed one at this stage  */
    rnd = st_rand_uniform_double(); /* Random double in the [0, 1) interval */
    /* Random integer in the [0, range_size) interval */
    res = rnd * range_size;
    /* Use gaussian distribution for (100 - st_rand_res) 
     * percent of all generated values.*/
    if (res < t)
    {
        sum = 0.0;

        for(i = 0; i < st_rand_iter; i++){
            sum += st_rand_uniform_double();
        }

        return a + sum * t * st_rand_iter_mult;
    }
  /* For the remaining st_rand_res percent of values use the uniform
     distribution. We map previously generated random double in the [0, 1)
     interval to the st_rand_pct percent part of the [a, b] interval. Then we move
     the resulting value in the [0, (b-a) * (st_rand_pct / 100)] interval to the
     center of the original interval [a, b]. */
    d = t * st_rand_pct_mult;
    res = rnd * (d + 1);
    res += t / 2 - t * st_rand_pct_2_mult;

    return a + (uint32_t) res;
}

/* Pareto distribution */
uint32_t st_rand_pareto(uint32_t a, uint32_t b)
{
    return a + (uint32_t) ((b - a + 1) *
                         pow(st_rand_uniform_double(), st_pareto_power));
}

/* Initialize thread-local RNG state */
void st_rand_thread_init(void)
{
    st_rand_init();

    /* We use libc PRNG to seed xoroshiro128+ */
    st_rng_state[0] = (((uint64_t) random()) << 32) |
        (((uint64_t) random()) & UINT32_MAX);
    st_rng_state[1] = (((uint64_t) random()) << 32) |
        (((uint64_t) random()) & UINT32_MAX);
}

