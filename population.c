#include "population.h"
#include <stdlib.h>

pop_t *pop_malloc(size_t num_chroms) {
        pop_t *tmp = malloc(POP_SIZE + sizeof(chrom_t *) * num_chroms);
        tmp->num_chroms = num_chroms;
        return tmp;
}
pop_t *pop_rand(size_t num_chroms, size_t num_bits) {
        pop_t *tmp = pop_malloc(num_chroms);
        for (size_t i=0; i<num_chroms; i++) {
                tmp->chroms[i] = chrom_rand(num_bits);
        }
        return tmp;
}

void pop_free(pop_t *pop) {
        free(pop);
}
void pop_purge(pop_t *pop) {
        for (size_t i=0; i<pop->num_chroms; i++) {
                chrom_free(pop->chroms[i]);
        }
        pop_free(pop);
}
