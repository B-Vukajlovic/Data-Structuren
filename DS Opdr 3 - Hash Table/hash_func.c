#include "hash_func.h"

/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long) *str;
}

unsigned long hash_djb2(const unsigned char *str) {
    unsigned long hash = 5381;
    unsigned long int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

unsigned long hash_k_and_r_v2(const unsigned char *str) {
    unsigned long hashval;

    for (hashval = 0; *str != '\0'; str++) {
        hashval = *str + 31*hashval;
    }

    return hashval;
}
