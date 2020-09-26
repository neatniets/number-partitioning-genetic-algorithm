#ifndef POPULATION_H
#define POPULATION_H

#include "chromosome.h"

/** True size of population struct */
#define POP_SIZE \
        (offsetof(pop_t, chroms))
typedef struct population pop_t;
/** Structure containing a number of chromosomes */
struct population {
        /** Number of chromosomes in population */
        size_t num_chroms;
        /** Array of chromosomes */
        chrom_t *chroms[];
};

/** Returns a population that can contain num_chromosomes number of
 * chromosomes
 * @post                no chromosomes are created nor initialized
 * @post                population must be freed (via pop_free) */
pop_t *pop_malloc(size_t num_chroms);
/** Returns a population containing num_chromosomes number of randomized
 * chromosomes where each chromosome holds num_bits random bits
 * @post                each chromosome will have be freed (via pop_purge)
 * @post                population will have to be freed (via pop_purge) */
pop_t *pop_rand(size_t num_chroms, size_t num_bits);

/** Frees memory allocated for population; does not free chromosomes
 * @post                any space allocated for chromosomes remains allocated
 * */
void pop_free(pop_t *pop);
/** Frees memory allocated for population and all of its chromosomes
 * @post                chromosomes will be deallocated and should not be
 *                      dereferenced */
void pop_purge(pop_t *pop);

#endif /* !POPULATION_H */
