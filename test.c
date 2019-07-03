#include <stdio.h>
#include "rng_xoroshiro.h"

int main()
{
    int             i;

    st_rand_thread_init();

    printf("uniform  rng:");
    for(i = 0; i != 10; i++){
        if(i != 9){
            printf("%u, ", st_rand_uniform(1, 100));
            continue;
        }
        printf("%u \n", st_rand_uniform(1, 100));
    }

    printf("gaussian rng:");
    for(i = 0; i != 10; i++){
        if(i != 9){
            printf("%u, ", st_rand_gaussian(1, 100));
            continue;
        }
        printf("%u \n", st_rand_gaussian(1, 100));
    }

    printf("special  rng:");
    for(i = 0; i != 10; i++){
        if(i != 9){
            printf("%u, ", st_rand_special(1, 100));
            continue;
        }
        printf("%u \n", st_rand_special(1, 100));
    }

    printf("pareto   rng:");
    for(i = 0; i != 10; i++){
        if(i != 9){
            printf("%u, ", st_rand_pareto(1, 100));
            continue;
        }
        printf("%u \n", st_rand_pareto(1, 100));
    }
    
    return 0;
}


