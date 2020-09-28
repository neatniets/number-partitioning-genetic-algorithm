#ifndef CHROMOSOME_H
#define CHROMOSOME_H
/** Defines object representing a binary string chromosome & defines functions
 * to act upon this chromosome
 * @file chromosome.h */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** Number of bits in a byte */
#define BITS            8
/** Leading bit to each byte in chromosome */
#define INITIAL_BIT     0x01

/** Macro to convert bits to how many bytes are required to hold them;
 * so excess bits will be rounded up to a full byte */
#define BITS2BYTES(NUM_BITS) \
        (((NUM_BITS) + BITS - 1) / BITS)

/** Macro to convert bits to the size of the chromosome required to hold that
 * many bits */
#define CHROM_SIZE(NUM_BITS) \
        (offsetof(chrom_t, bytes) + BITS2BYTES(NUM_BITS))
/** Typedef for struct chromosome */
typedef struct chromosome chrom_t;
/** Structure representing a binary chromosome; its unfitness is always
 * uninitialized when allocated */
struct chromosome {
        /** Unfitness variable to be determined externally */
        long long unfitness;
        /** Number of bits contained in bytes array */
        size_t num_bits;
        /** Byte array to hold bits */
        uint8_t bytes[];
};

/** Allocates a chromosome big enough to fit num_bits
 * @post                unfitness is uninitialized
 * @post                bytes are uninitialized; space for them is
 *                      allocated */
chrom_t *chrom_malloc(size_t num_bits);
/** Allocates a chromosome with num_bits random bits
 * @post                unfitness is uninitialized
 * @post                leading bit of entire chromosome is always 0 */
chrom_t *chrom_rand(size_t num_bits);
/** Frees space granted for chromosome */
void chrom_free(chrom_t *chrom);

/** Reads each bit of the chromosome sequentially and passes the value to
 * func */
void chrom_bitwise_read(const chrom_t *chrom, void *context,
                        void (*func)(bool bit_val, void *context));
/** Writes each bit of the chromosome to the return value of func sequentially
 * without reading the original bit */
void chrom_bitwise_write(chrom_t *chrom, void *context,
                         bool (*func)(void *context));
/** Reads each bit of the chromosome sequentially and passes it to func and
 * overwrites that bit with the return value of func */
void chrom_bitwise_rnw(chrom_t *chrom, void *context,
                       bool (*func)(bool bit_val, void *context));

/** Inverts each bit of the chromosome */
void chrom_invert(chrom_t *chrom);
/** Performs uniform crossover between parents to generate child chromosome
 * @post                child chromosome's unfitness will be uninitialized
 * @post                child chromosome's leading bit will always be 0;
 *                      this is guaranteed as the parents should follow this
 *                      pattern, but will also be enforced internally */
chrom_t *chrom_ucx(const chrom_t *parent1, const chrom_t *parent2);
/** Performs probabilistic mutation upon each bit of chromosome; average
 * mutation rate is 1 bit mutated across chromosome
 * @post                if the leading bit of chromosome is mutated to be 1,
 *                      the chromosome will be inverted so that it is 0 again;
 *                      this merely swaps between 2 groups and should not
 *                      impact unfitness from if it were left uninverted
 * @post                chromosome's unfitness will be invalidated but
 *                      unaltered */
void chrom_mutate(chrom_t *chrom);

/** Prints each bit of chromosome then prints a newline */
void chrom_print(const chrom_t *chrom);

/* LOW-LEVEL API */
/** Reads each bit in the byte sequentially and passes that bit value to func
 * @pre                 primarily used in conjunction with chrom_bytewise */
void byte_bitwise_read(const uint8_t *byte, uint8_t num_bits, void *context,
                       void (*func)(bool bit_val, void *context));
/** Does not read each bit of byte; merely assigns the return value of func to
 * each bit of byte sequentially
 * @pre                 primarily used in conjunction with chrom_bytewise */
void byte_bitwise_write(uint8_t *byte, uint8_t num_bits, void *context,
                        bool (*func)(void *context));
/** Reads each bit in byte sequentially, passing it into func, and reassigns
 * it to the return value of func
 * @pre                 primarily used in conjunction with chrom_bytewise */
void byte_bitwise_rnw(uint8_t *byte, uint8_t num_bits, void *context,
                      bool (*func)(bool bit_val, void *context));
/** Iterates over every byte in the chromosome and passes its address to func
 * @pre                 byte is never read by the function; safe to be
 *                      uninitialized
 * @post                the function itself will not alter any part of the
 *                      chromosome; func, however, is permitted to alter the
 *                      bytes */
void chrom_bytewise(chrom_t *chrom, void *context,
                    void (*func)(uint8_t *byte, void *context));
/* LOW-LEVEL API */

#endif /* !CHROMOSOME_H */
