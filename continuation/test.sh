#!/bin/sh
# ライブラリの動作を確認

echo gcc
gcc cont.c continuation.c
./a.out
echo gcc -O3
gcc -O3 cont.c continuation.c
./a.out
echo


echo tcc
tcc cont.c continuation.c
./a.out
echo


echo clang
clang cont.c continuation.c
./a.out
echo clang-O3
clang -O3 cont.c continuation.c
./a.out
echo


