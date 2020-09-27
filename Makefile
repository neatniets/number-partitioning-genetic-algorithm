genetic-algorithm: genetic-algorithm.c number-partition.c population.c chromosome.c
	gcc -Wall -o genetic-algorithm.out genetic-algorithm.c number-partition.c population.c chromosome.c

greedy: greedy.c
	gcc -Wall -o greedy.out greedy.c

chrom-test: chrom-test.c chromosome.c
	gcc -Wall -o chrom-test.out chrom-test.c chromosome.c

pop-test: pop-test.c population.c chromosome.c
	gcc -Wall -o pop-test.out pop-test.c population.c chromosome.c
