# Lab1 Readme
## To Run

```
mkdir build
cd build
cmake ..
make
```

## Testcases

```
benchmark/quicksort.c 
benchmark/matrixmulti.c
benchmark/cholesky.c
```

## Experiment Results
in
```
benchmark/
```
for example, 
```
matmul_scoreboard_192.out
```
is the result of scoreboard running matrix multiplication on 192 $\times$ 192 matrices
and 
```
cholesky_five_stage_32.out
```
is the result of five-stage-fully-bypassed pipeline (lab0) running Cholesky factorization on a 32 $\times$ 32 matrix.