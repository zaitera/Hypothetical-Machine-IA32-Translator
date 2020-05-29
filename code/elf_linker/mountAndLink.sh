#!/bin/bash

ASM_NAME='test'
nasm -f elf -o $ASM_NAME.o $ASM_NAME.s && \
ld -m elf_i386 -o $ASM_NAME.e $ASM_NAME.o && \
rm $ASM_NAME.o
