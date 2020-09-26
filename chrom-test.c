#include "chromosome.h"
#include <stdio.h>

#define NUM_RAND_CHROM          20
#define CHROM_NUM_ITEMS         20
#define NUM_CROSSOVERS          20

int main(void) {
        for (size_t i=0; i<NUM_RAND_CHROM; i++) {
                chrom_t *chrom = chrom_rand(CHROM_NUM_ITEMS);
                printf("rand chromosome #%zu:\n", i);
                chrom_print(chrom);
                printf("inverted:\n");
                chrom_invert(chrom);
                chrom_print(chrom);
                chrom_free(chrom);
        }
        putchar('\n');
        for (size_t i=0; i<NUM_CROSSOVERS; i++) {
                chrom_t *parent1 = chrom_rand(CHROM_NUM_ITEMS);
                printf("parent1:\n");
                chrom_print(parent1);
                chrom_t *parent2 = chrom_rand(CHROM_NUM_ITEMS);
                printf("parent2:\n");
                chrom_print(parent2);
                chrom_t *child = chrom_ucx(parent1, parent2);
                printf("child from crossover:\n");
                chrom_print(child);
                printf("mutating child...\n");
                chrom_mutate(child);
                printf("mutated:\n");
                chrom_print(child);
                chrom_free(parent1);
                chrom_free(parent2);
                chrom_free(child);
        }
        return 0;
}
