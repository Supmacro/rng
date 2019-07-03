#include <stdio.h>
#include "rng_xoroshiro.h"


/* This is a public function that can call different probability 
 * random functions depending on the parameters.*/
int test_rng(char *rng, int cnt, uint32_t(*rand)(uint32_t, uint32_t))
{
    int             i;

    if(rng == NULL || rand == NULL){
        return 1;
    }

    printf("%s: ", rng);
    for(i = 0; i != cnt; i++){
        if(i != cnt-1){
            printf("%u, ", rand(1, 100));
            continue;
        }
        printf("%u \n", rand(1, 100));
    }

    return 0;
}

/* Probability distributions: uniform, gaussian, special, pareto */
int main(int argc, char *argv[])
{
    char  rng_uniform[] ="[rng  uniform]";
    char  rng_gaussian[]="[rng gaussian]";
    char  rng_special[] ="[rng  special]";
    char  rng_pareto[]  ="[rng   pareto]";

    st_rand_thread_init();
   
    if(test_rng(rng_uniform, 10, &st_rand_uniform)){
        printf("error: rng uniform. \n");
        return 1;
    }

    if(test_rng(rng_gaussian, 10, &st_rand_gaussian)){
        printf("error: rng gaussian. \n");
        return 1;
    }

    if(test_rng(rng_special, 10, &st_rand_special)){
        printf("error: rng special. \n");
        return 1;
    }

    if(test_rng(rng_pareto, 10, &st_rand_pareto)){
        printf("error: rng pareto. \n");
        return 1;
    }

    return 0;
}


