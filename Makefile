genetic-algorithm: genetic-algorithm.c number-partition.c population.c chromosome.c
	gcc -Wall -o genetic-algorithm.out genetic-algorithm.c number-partition.c population.c chromosome.c

greedy: greedy.c
	gcc -Wall -o greedy.out greedy.c
