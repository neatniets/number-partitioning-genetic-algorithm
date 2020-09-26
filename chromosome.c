/** Implements function prototypes in chromosome.h
 * @file chromosome.c */
#include "chromosome.h"
#include <stdlib.h>
#include <stdio.h>

static void invert_if_needed(chrom_t *chrom) {
        if (chrom->bytes[0] & INITIAL_BIT) {
                chrom_invert(chrom);
        }
}

chrom_t *chrom_malloc(size_t num_bits) {
        chrom_t *tmp = malloc(CHROM_SIZE(num_bits));
        tmp->num_bits = num_bits;
        return tmp;
}
static void rand_byte(uint8_t *byte, void *no_context) {
        *byte = rand();
}
chrom_t *chrom_rand(size_t num_bits) {
        chrom_t *tmp = chrom_malloc(num_bits);
        chrom_bytewise(tmp, NULL, rand_byte);
        invert_if_needed(tmp);
        return tmp;
}
void chrom_free(chrom_t *chrom) {
        free(chrom);
}

struct chrom_bitwise_context {
        size_t num_bits;
        void *context;
        void *func_ptr;
};
static void read_byte(uint8_t *byte, void *chrom_bitwise_context) {
        struct chrom_bitwise_context *context
                = (struct chrom_bitwise_context *)chrom_bitwise_context;
        uint8_t bits2read = (context->num_bits >= BITS)
                            ? BITS
                            : context->num_bits;
        byte_bitwise_read(byte, bits2read, context->context,
                          (void (*)(bool, void *))context->func_ptr);
        context->num_bits -= bits2read;
}
void chrom_bitwise_read(const chrom_t *chrom, void *context,
                        void (*func)(bool bit_val, void *context)) {
        struct chrom_bitwise_context tmp = {chrom->num_bits, context, func};
        chrom_bytewise((chrom_t *)chrom, &tmp, read_byte);
}
static void write_byte(uint8_t *byte, void *chrom_bitwise_context) {
        struct chrom_bitwise_context *context
                = (struct chrom_bitwise_context *)chrom_bitwise_context;
        uint8_t bits2read = (context->num_bits >= BITS)
                            ? BITS
                            : context->num_bits;
        byte_bitwise_write(byte, bits2read, context->context,
                           (bool (*)(void *))context->func_ptr);
        context->num_bits -= bits2read;
}
void chrom_bitwise_write(chrom_t *chrom, void *context,
                         bool (*func)(void *context)) {
        struct chrom_bitwise_context tmp = {chrom->num_bits, context, func};
        chrom_bytewise((chrom_t *)chrom, &tmp, write_byte);
}
static void rnw_byte(uint8_t *byte, void *chrom_bitwise_context) {
        struct chrom_bitwise_context *context
                = (struct chrom_bitwise_context *)chrom_bitwise_context;
        uint8_t bits2read = (context->num_bits >= BITS)
                            ? BITS
                            : context->num_bits;
        byte_bitwise_rnw(byte, bits2read, context->context,
                           (bool (*)(bool, void *))context->func_ptr);
        context->num_bits -= bits2read;
}
void chrom_bitwise_rnw(chrom_t *chrom, void *context,
                       bool (*func)(bool bit_val, void *context)) {
        struct chrom_bitwise_context tmp = {chrom->num_bits, context, func};
        chrom_bytewise((chrom_t *)chrom, &tmp, rnw_byte);
}

static void invert_byte(uint8_t *byte, void *no_context) {
        *byte = ~(*byte);
}
void chrom_invert(chrom_t *chrom) {
        chrom_bytewise(chrom, NULL, invert_byte);
}
struct ucx_context {
        const chrom_t *parent1;
        const chrom_t *parent2;
        size_t byte_index;
        uint8_t byte_bit_index;;
};
static bool ucx_bit(void *ucx_context) {
        struct ucx_context *context = (struct ucx_context *)ucx_context;
        if (context->byte_bit_index == BITS) {
                context->byte_bit_index = 0;
                context->byte_index++;
        }
        bool is_parent1_chosen = rand() % 2;
        bool bit_val = (is_parent1_chosen
                        * (context->parent1->bytes[context->byte_index]
                           & (INITIAL_BIT << context->byte_bit_index))
                       | (!is_parent1_chosen
                          * (context->parent2->bytes[context->byte_index]
                             & (INITIAL_BIT << context->byte_bit_index))));
        context->byte_bit_index++;
        return bit_val;
}
/* Uniform crossover */
chrom_t *chrom_ucx(const chrom_t *parent1, const chrom_t *parent2) {
        chrom_t *tmp = chrom_malloc(parent1->num_bits);
        struct ucx_context context = {parent1, parent2, 0, 0};
        chrom_bitwise_write(tmp, &context, ucx_bit);
        invert_if_needed(tmp);
        return tmp;
}
static bool mutate_bit(bool bit_val, void *prob_div) {
        bool is_mutated = (rand() % *(size_t *)prob_div) == 0;
        return (!is_mutated && bit_val) || (is_mutated && !bit_val);
}
/* Uniform probabilistic mutation */
void chrom_mutate(chrom_t *chrom) {
        size_t probability_divisor = chrom->num_bits;
        chrom_bitwise_rnw(chrom, &probability_divisor, mutate_bit);
        invert_if_needed(chrom);
}

static void print_bit(bool bit_val, void *no_context) {
        printf("%d", bit_val);
}
void chrom_print(const chrom_t *chrom) {
        chrom_bitwise_read(chrom, NULL, print_bit);
        putchar('\n');
}

/* LOW-LEVEL API */
void byte_bitwise_read(const uint8_t *byte, uint8_t num_bits, void *context,
                       void (*func)(bool bit_val, void *context)) {
        uint8_t bit_manip = INITIAL_BIT;
        for (uint8_t i=0; i<num_bits; i++, bit_manip <<= 1) {
                func(*byte & bit_manip, context);
        }
}
void byte_bitwise_write(uint8_t *byte, uint8_t num_bits, void *context,
                        bool (*func)(void *context)) {
        uint8_t bit_manip = INITIAL_BIT;
        *byte = 0;
        for (uint8_t i=0; i<num_bits; i++, bit_manip <<= 1) {
                *byte |= (bit_manip * func(context));
        }
}
void byte_bitwise_rnw(uint8_t *byte, uint8_t num_bits, void *context,
                      bool (*func)(bool bit_val, void *context)) {
        uint8_t bit_manip = INITIAL_BIT;
        for (uint8_t i=0; i<num_bits; i++, bit_manip <<= 1) {
                *byte = (*byte & ~bit_manip)
                        | (bit_manip * func(*byte & bit_manip, context));
        }
}
/* Will not read byte; can be uninitialized */
void chrom_bytewise(chrom_t *chrom, void *context,
                    void (*func)(uint8_t *byte, void *context)) {
        size_t bytes = BITS2BYTES(chrom->num_bits);
        for (size_t i=0; i<bytes; i++) {
                func(chrom->bytes+i, context);
        }
}
/* LOW-LEVEL API */

