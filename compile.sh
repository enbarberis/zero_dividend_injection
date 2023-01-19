#!/bin/sh
nasm -f elf64 zdi.s -o zdi.o
gcc fr_checker.c -o fr_checker
gcc zdi.c -o zdi zdi.o
