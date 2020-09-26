#ifndef NUMBER_PARTITION_H
#define NUMBER_PARTITION_H

#include <stddef.h>

#define PROB_SET_SIZE(NUM_ITEMS) \
        (offsetof(prob_set_t, item_vals) + sizeof(long long) * NUM_ITEMS)
typedef struct problem_set prob_set_t;
struct problem_set {
        size_t num_items;
        long long item_vals[];
};
typedef struct result result_t;
struct result {
        size_t num_gens_passed;
        size_t set0_count;
        long long *set0_vals;
        size_t set1_count;
        long long *set1_vals;
};

prob_set_t *prob_set_malloc(size_t num_items);
void prob_set_free(prob_set_t *ps);
void result_free(result_t *res);

result_t *num_part_2way(const prob_set_t *ps);

#endif /* !NUMBER_PARTITION_H */
