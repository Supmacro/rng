test : test.o rng_xoroshiro.o
	gcc -o $@ $^ -lm
test.o : test.c
	gcc -g -c $^
rng_xoroshiro.o : rng_xoroshiro.c
	gcc -g -c $^ 

.PHONEY:clean
clean:
	rm -rf ./*.o test
