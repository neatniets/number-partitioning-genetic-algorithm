#ifndef NUMBER_PARTITION_H
#define NUMBER_PARTITION_H
/** Header file to solve a 2-way number partitioning problem using a
 * genetic algorithm
 * @file number-partition.h */

#include <stddef.h>

/** Helper macro to know the size of a problem set structure */
#define PROB_SET_SIZE(NUM_ITEMS) \
        (offsetof(prob_set_t, item_vals) + sizeof(long long) * NUM_ITEMS)
typedef struct problem_set prob_set_t;
/** Structure to pass to num_part_2way */
struct problem_set {
        size_t num_items;
        long long item_vals[];
};
typedef struct result result_t;
/** Return structure of num_part_2way */
struct result {
        size_t num_gens_passed;
        size_t set0_count;
        long long *set0_vals;
        size_t set1_count;
        long long *set1_vals;
};

/** Returns a problem set structure to pass to num_part_2way */
prob_set_t *prob_set_malloc(size_t num_items);
/** Frees a problem set structure from the heap */
void prob_set_free(prob_set_t *ps);
/** Frees a result structure returned by num_part_2way */
void result_free(result_t *res);

result_t *num_part_2way(const prob_set_t *ps);

#endif /* !NUMBER_PARTITION_H */
