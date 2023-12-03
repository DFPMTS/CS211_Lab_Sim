#include "lib.h"

const int M = 40;

void matmulti(float a[M][M], float b[M][M], float c[M][M], int M) {

  // for (int i = 0; i < M; ++i) {
  //   for (int j = 0; j < M; ++j) {
  //     c[i][j] = 0;
  //   }
  // }
  // for (int k = 0; k < M; ++k) {
  //   for (int j = 0; j < M; ++j) {
  //     for (int i = 0; i < M; ++i) {
  //       c[i][j] += a[i][k] * b[k][j];
  //     }
  //   }
  // }
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      c[i][j] = 0;
      for (int k = 0; k < M; ++k) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

int main() {
  float A[M][M], B[M][M], C[M][M];

  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      A[i][j] = i;
      B[i][j] = j;
      C[i][j] = 0;
    }
  }

  print_s("The content of A is: \n");
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      print_f(A[i][j]);
      print_s(" ");
    }
    print_s("\n");
  }

  print_s("The content of B is: \n");
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      print_f(B[i][j]);
      print_s(" ");
    }
    print_s("\n");
  }

  matmulti(A, B, C, M);

  print_s("The content of C=A*B is: \n");
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      print_f(C[i][j]);
      print_s(" ");
    }
    print_s("\n");
  }

  exit_proc();
}