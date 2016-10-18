#!/bin/bash

clang++ -S -emit-llvm $1 -I/usr/include/mpi -std=c++11
