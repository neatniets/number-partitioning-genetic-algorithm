# numpart-genalgo
A simple genetic algorithm to solve 2-way number partitioning.

This repository includes some test programs to test the chromosome and population structures & functions, a genetic algorithm to solve 2-way number partitioning, and a greedy algorithm to do the same.

Running "make genetic-algorithm" generates the output for the main part of the program.
The required input format is specified in the source code of genetic-algorithm.c

Some datasets are provided in the datasets folder and can be passed to genetic-algorithm.out via redirection.
The "uniform" datasets are uniform distributions across (0,N] where N is the number of items.
The "randbound" datasets are randomly chosen numbers bounded by N.
The "rand" datasets are randomly chosen numbers bounded by (LLONG_MAX / N) - 1 such that, if all items were in a single set, the *long long* datatype would not overflow.

# GENETIC ALGORITHM DETAILS

The chromosomes are directly encoded as binary strings with each bit representing which subset the respective item belongs to.

Crossover is uniform crossover, randomly choosing 1 parent to inherit from at each bit.

Mutation runs through each bit of the chromosome, flipping them probabilisticly at a rate of 1/N, where N is the length of the chromosome.
Therefore, the average rate of mutation is 1 bit per chromosome.

Population size is equal to the number of items passed to the function.

Mating pool is formed via tournament selection with a tournament size of k=2 and with a probability of p=1 that the more fit be chosen.
2 chromosomes are chosen at random from the population and their *un*fitnesses are compared. The more fit chromosome is added to the mating pool.
The chromosomes added to the mating pool are not removed from the population; duplicates are allowed in the mating pool.

Parents are randomly chosen from the mating pool for crossover.
Every chromosome then undergoes previously described mutation.

The genetic algorithm only runs for 100 generations or until a perfect subset split is found and then returns the best chromosome.
