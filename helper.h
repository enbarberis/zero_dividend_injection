/*
 * Thursday, September 9th 2021
 *
 * Enrico Barberis - e.barberis@vu.nl
 * Pietro Frigo - p.frigo@vu.nl
 * Marius Muench - m.muench@vu.nl
 * Herbert Bos - herbertb@cs.vu.nl
 * Cristiano Giuffrida - giuffrida@cs.vu.nl
 *
 * Vrije Universiteit Amsterdam - Amsterdam, The Netherlands
 *
 */

#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

static inline __attribute__((always_inline)) void fence(void) {
    asm volatile("mfence");
}   

static inline __attribute__((always_inline)) void maccess(void *p) {
    *(volatile unsigned char *)p;
}

static inline __attribute__((always_inline)) void clflush(void* p) {
    asm volatile("clflush (%0)\n"::"r"(p));
}   

static inline __attribute__((always_inline)) uint64_t rdtscp(void) {
    uint64_t lo, hi;
    asm volatile("rdtscp\n" : "=a" (lo), "=d" (hi) :: "rcx");
    return (hi << 32) | lo;
}

static inline __attribute__((always_inline)) uint64_t load_time(void *p)
{
    uint64_t t0 = rdtscp();
    maccess(p);
    return rdtscp() - t0;
}

static void cpuid ( uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx, 
        uint32_t index, uint32_t ecx_in )
{
    uint32_t a,b,c,d;
    asm volatile ("cpuid"
            : "=a" (a), "=b" (b), "=c" (c), "=d" (d) \
            : "0" (index), "2"(ecx_in) );
    *eax = a; *ebx = b; *ecx = c; *edx = d;
}

/* Read the MSR "reg" on cpu "cpu" */
uint64_t rdmsr(uint32_t reg, int cpu)
{
	int fd;
	char msr_file_name[128];
	uint64_t data;

	sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);

	fd = open(msr_file_name, O_RDONLY);
	if (fd < 0) {
		printf( "rdmsr: can't open %s\n", msr_file_name);
		exit(1);
	}

	if ( pread(fd, &data, sizeof(data), reg) != sizeof(data) ) {
		printf( "rdmsr: cannot read %s/0x%08x\n", msr_file_name, reg);
		exit(2);
	}

	close(fd);

	return data;
}

#endif //_HELPER_H_
