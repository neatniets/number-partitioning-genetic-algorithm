#include "population.h"
#include <stdio.h>

#define NUM_RAND_POPS           10
#define NUM_CHROM               10
#define NUM_ITEMS               10

void print_population(const pop_t *pop);

int main(void) {
        for (size_t i=0; i<NUM_RAND_POPS; i++) {
                pop_t *tmp = pop_rand(NUM_CHROM, NUM_ITEMS);
                printf("random population #%zu:\n", i);
                print_population(tmp);
                printf("making copy of population #%zu\n", i);
                pop_t *copy = pop_malloc(NUM_CHROM);
                for (size_t j=0; j<NUM_CHROM; j++) {
                        copy->chroms[j] = tmp->chroms[j];
                }
                printf("copy of #%zu:\n", i);
                print_population(copy);
                printf("freeing (not purging) copy\n");
                pop_free(copy);
                printf("original population still exists:\n");
                print_population(tmp);
                printf("purging original population\n");
                pop_purge(tmp);
        }
        return 0;
}

void print_population(const pop_t *pop) {
        for (size_t i=0; i<pop->num_chroms; i++) {
                chrom_print(pop->chroms[i]);
        }
}
