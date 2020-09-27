#include "number-partition.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define NUM_PASSES              100

long long sum_set(const long long *set_vals, size_t set_count);
void print_result(const result_t *res);
void print_set(const long long *set_vals, size_t set_count);

/* Input format:
 * N
 * x_1, x_2, x_3, . . . x_N-1
 * N = number of items/values to partition
 * x_i = i-th element's value */
int main(void) {
        srand(time(NULL));
        size_t num_items;
        scanf(" %zu", &num_items);
        prob_set_t *ps = prob_set_malloc(num_items);
        for (size_t i=0; i<num_items; i++) {
                scanf(" %lld", ps->item_vals+i);
        }
        long double avg_gen = 0;
        long long best_gen = LLONG_MAX;
        long double avg_sum_diff = 0;
        long long best_sum_diff = LLONG_MAX;
        for (size_t i=0; i<NUM_PASSES; i++) {
                printf("pass #%zu:\n", i);
                result_t *res = num_part_2way(ps);
                print_result(res);
                avg_gen = avg_gen / (i+1) * i
                          + (long double)res->num_gens_passed / (i+1);
                if (res->num_gens_passed < best_gen) {
                        best_gen = (long long)res->num_gens_passed;
                }
                long long set0_sum = sum_set(res->set0_vals, res->set0_count);
                long long set1_sum = sum_set(res->set1_vals, res->set1_count);
                long long sum_diff;
                if (set1_sum < set0_sum) {
                        sum_diff = set0_sum - set1_sum;
                } else {
                        sum_diff = set1_sum - set0_sum;
                }
                if (sum_diff < best_sum_diff) {
                        best_sum_diff = sum_diff;
                }
                avg_sum_diff = avg_sum_diff / (i+1) * i
                               + (long double)sum_diff / (i+1);
                result_free(res);
                putchar('\n');
        }
        printf("average number of generations passed = %Lf\n"
               "lowest number of generations passed = %lld\n"
               "average sum difference = %Lf\n"
               "best sum difference = %lld\n",
               avg_gen, best_gen, avg_sum_diff, best_sum_diff);
        prob_set_free(ps);
        return 0;
}

long long sum_set(const long long *set_vals, size_t set_count) {
        long long sum = 0;
        for (size_t i=0; i<set_count; i++) {
                sum += set_vals[i];
        }
        return sum;
}
void print_result(const result_t *res) {
        printf("# generations passed: %zu\n"
               "# items in set0: %zu\n"
               "set0 item values:\n",
               res->num_gens_passed, res->set0_count);
        print_set(res->set0_vals, res->set0_count);
        long long set0_sum = sum_set(res->set0_vals, res->set0_count);
        printf("set0 sum: %lld\n", set0_sum);
        printf("# items in set1: %zu\n"
               "set1 item values:\n",
               res->set1_count);
        print_set(res->set1_vals, res->set1_count);
        long long set1_sum = sum_set(res->set1_vals, res->set1_count);
        printf("set1 sum: %lld\n", set1_sum);
        unsigned long long sum_diff;
        if (set1_sum > set0_sum) {
                sum_diff = set1_sum - set0_sum;
        } else {
                sum_diff = set0_sum - set1_sum;
        }
        printf("difference of sums: %llu\n", sum_diff);
}
void print_set(const long long *set_vals, size_t set_count) {
        for (size_t i=0; i<set_count; i++) {
                printf("%lld ", set_vals[i]);
        }
        putchar('\n');
}
