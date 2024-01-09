/* Example hash function with terrible performance */
unsigned long hash_too_simple(const unsigned char *str);

/* Hash function created by Daniel J.Bernstein.
Source: https://stackoverflow.com/questions/
        64699597/how-to-write-djb2-hashing-function-in-c*/
unsigned long hash_djb2(const unsigned char *str);

/* Hash function destributed in K&R version 2, on pg 144 of the book
Source: https://stackoverflow.com/questions/
        7666509/hash-function-for-string*/
unsigned long hash_k_and_r_v2(const unsigned char *str);
