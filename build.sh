#!/usr/bin/bash

if ! which nasm > /dev/null 2>&1; then
	echo "nasm not found"
	exit 1
fi

if ! ls phicalc.c utils.asm keybinds.h > /dev/null; then
	echo "not all source files are found"
	exit 2
fi

nasm utils.asm -f elf64 -o utils.o
gcc phicalc.c utils.o -o phicalc -lm -O2
rm utils.o
