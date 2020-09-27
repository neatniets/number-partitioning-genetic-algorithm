/** Implements a genetic algorithm for use with number-partition.h
 * functions
 * @file number-partition.c */
#include "number-partition.h"
#include "population.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/** Max number of generations the algorithm will go through before
 * terminating */
#define MAX_GENS                        100
/** Scaling factor for population size based on problem instance input size */
#define POP_SIZE_FACTOR                 1

/** Return a result structure based on the decidedly-best chromosome */
static result_t *result_malloc(chrom_t *best_chrom,
                               const long long *item_vals,
                               size_t num_gens_passed);
/** Finds the fittest chromosome in the population and returns its index */
static size_t find_fittest(const pop_t *pop);
/** Generates a random initial population of chromosomes and returns it */
static pop_t *initial_pop(size_t num_chroms, size_t num_bits,
                          const long long *item_vals);
/** Compares the fitness of two chromosomes; returns 1 if c2 is more fit,
 * -1 if c1 is more fit, and 0 if both are equally fit */
static int chrom_fit_cmp(const chrom_t *c1, const chrom_t *c2);
/** Calculates the fitness of an entire population and modifies the
 * chromosomes' unfitness values accordingly */
static void pop_calc_fitness(pop_t *pop, const long long *item_vals);
/** Performs tournament selection and returns a mating pool which _points_ to
 * chromosomes in the initial population
 * @post make sure to pop_free(mating_pool) and to pop_purge(pop) */
static pop_t *tourn_select(const pop_t *pop);
/** Generate a new generation to replace the old one using the mating pool for
 * crossover and mutating the offspring */
static pop_t *new_gen(const pop_t *tourn, const long long *item_vals);

prob_set_t *prob_set_malloc(size_t num_items) {
        prob_set_t *tmp = malloc(PROB_SET_SIZE(num_items));
        tmp->num_items = num_items;
        return tmp;
}
void prob_set_free(prob_set_t *ps) {
        free(ps);
}
void result_free(result_t *res) {
        free(res->set0_vals);
        free(res->set1_vals);
        free(res);
}

result_t *num_part_2way(const prob_set_t *ps) {
        chrom_t *best_chrom = chrom_malloc(ps->num_items);
        best_chrom->fitness = LLONG_MAX;
        const size_t pop_size = ps->num_items * POP_SIZE_FACTOR;
        pop_t *pop = initial_pop(pop_size, ps->num_items, ps->item_vals);
        size_t num_gen_passed = 1;
        while ((num_gen_passed < MAX_GENS)
               && (best_chrom->fitness != 0)) {
                pop_t *tourn = tourn_select(pop);
                pop_t *next_gen = new_gen(tourn, ps->item_vals);
                pop_free(tourn);
                pop_purge(pop);
                pop = next_gen;
                size_t fittest_i = find_fittest(pop);
                if (chrom_fit_cmp(best_chrom, pop->chroms[fittest_i]) == 1) {
                        memcpy(best_chrom, pop->chroms[fittest_i],
                               CHROM_SIZE(best_chrom->num_bits));
                }
                num_gen_passed++;
        }
        pop_purge(pop);
        result_t *res = result_malloc(best_chrom,
                                      ps->item_vals, num_gen_passed);
        chrom_free(best_chrom);
        return res;
}

struct result_sets_context {
        const long long *item_vals;
        long long *set0_vals;
        long long *set1_vals;
};
static void split_result_sets(bool bit_val, void *result_sets_context) {
        struct result_sets_context *context
                = (struct result_sets_context *)result_sets_context;
        if (bit_val == 0) {
                *context->set0_vals = *context->item_vals;
                context->set0_vals++;
        } else {
                *context->set1_vals = *context->item_vals;
                context->set1_vals++;
        }
        context->item_vals++;
}
static void count1s_bits(bool bit_val, void *count) {
        *(size_t *)count += bit_val;
}
static result_t *result_malloc(chrom_t *best_chrom,
                               const long long *item_vals,
                               size_t num_gens_passed) {
        result_t *res = malloc(sizeof(*res));
        res->num_gens_passed = num_gens_passed;
        res->set1_count = 0;
        chrom_bitwise_read(best_chrom, &res->set1_count, count1s_bits);
        res->set1_vals = malloc(sizeof(*res->set1_vals)*res->set1_count);
        res->set0_count = best_chrom->num_bits - res->set1_count;
        res->set0_vals = malloc(sizeof(*res->set0_vals)*res->set0_count);
        struct result_sets_context context = {item_vals, res->set0_vals,
                                              res->set1_vals};
        chrom_bitwise_read(best_chrom, &context, split_result_sets);
        return res;
}
static size_t find_fittest(const pop_t *pop) {
        size_t fittest_i = 0;
        for (size_t i=1; i<pop->num_chroms; i++) {
                if (chrom_fit_cmp(pop->chroms[fittest_i],
                                  pop->chroms[i]) == 1) {
                        fittest_i = i;
                }
        }
        return fittest_i;
}
static pop_t *initial_pop(size_t num_chroms, size_t num_bits,
                          const long long *item_vals) {
        pop_t *pop = pop_rand(num_chroms, num_bits);
        pop_calc_fitness(pop, item_vals);
        return pop;
}
static int chrom_fit_cmp(const chrom_t *c1, const chrom_t *c2) {
        if (c1->fitness < c2->fitness) {
                return -1;
        } else if (c1->fitness > c2->fitness) {
                return 1;
        } else {
                return 0;
        }
}
struct fitness_context {
        const long long *item_vals;
        long long fitness;
};
static void bit_calc_fitness(bool bit_val, void *fitness_context) {
        struct fitness_context *context
                = (struct fitness_context *)fitness_context;
        if (bit_val) {
                context->fitness -= *context->item_vals;
        } else {
                context->fitness += *context->item_vals;
        }
        context->item_vals++;
}
static void chrom_calc_fitness(chrom_t *chrom, const long long *item_vals) {
        struct fitness_context context = {item_vals, 0};
        chrom_bitwise_read(chrom, &context, bit_calc_fitness);
        if (context.fitness < 0) {
                chrom->fitness = -context.fitness;
        } else {
                chrom->fitness = context.fitness;
        }
}
static void pop_calc_fitness(pop_t *pop, const long long *item_vals) {
        for (size_t i=0; i<pop->num_chroms; i++) {
                chrom_calc_fitness(pop->chroms[i], item_vals);
        }
}
static pop_t *tourn_select(const pop_t *pop) {
        pop_t *tourn = pop_malloc(pop->num_chroms);
        for (size_t i=0; i<tourn->num_chroms; i++) {
                size_t i1 = rand() % pop->num_chroms;
                size_t i2 = rand() % pop->num_chroms;
                if (chrom_fit_cmp(pop->chroms[i1], pop->chroms[i2]) == 1) {
                        tourn->chroms[i] = pop->chroms[i2];
                } else {
                        tourn->chroms[i] = pop->chroms[i1];
                }
        }
        return tourn;
}
static pop_t *new_gen(const pop_t *tourn, const long long *item_vals) {
        pop_t *new_gen = pop_malloc(tourn->num_chroms);
        for (size_t i=0; i<new_gen->num_chroms; i++) {
                size_t p1i = rand() % tourn->num_chroms;
                size_t p2i = rand() % tourn->num_chroms;
                new_gen->chroms[i] = chrom_ucx(tourn->chroms[p1i],
                                               tourn->chroms[p2i]);
                chrom_mutate(new_gen->chroms[i]);
        }
        pop_calc_fitness(new_gen, item_vals);
        return new_gen;
}
