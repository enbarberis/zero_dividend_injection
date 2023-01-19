#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "flush_reload.h"

extern void zdi(uint64_t upper_dividend, uint64_t lower_dividend,
                uint64_t divisor, void *fr_buf);

int main(int argc, char **argv) {

    size_t results[2];
    uint8_t *fr_buf = mmap(NULL, 2*STRIDE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE, -1, 0);
    memset(fr_buf, 0x41, 2*STRIDE);

    printf("Test ZDI on 2^64 / 2^52\n");
    printf("2^64 / 2^52 == 4096 (hit second fr_buf entry architecturally)\n");
    printf("0    / 2^52 == 0    (hit first  fr_buf entry speculatively)\n"); 
    memset(results, 0, sizeof(results));
    for(int i=0; i<10000; i++) {
        flush(fr_buf);
        zdi(1, 0, 1ULL<<52, fr_buf);
        reload(fr_buf, results);
    }
    printf("results[0] = %ld\n", results[0]);
    printf("results[1] = %ld\n", results[1]);
    
    return 0;
}
