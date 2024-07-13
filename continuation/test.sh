#!/bin/sh

echo gcc
gcc cont.c continuation.c
./a.out
echo


echo tcc
tcc cont.c continuation.c
./a.out
echo


echo clang
clang cont.c continuation.c
./a.out
echo


