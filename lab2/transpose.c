
#include "lib.h"
#include "stdio.h"
#define M 280

int a[M][M];
int b[M][M];
int main() {
  int c[M];
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      b[j][i] = a[i][j];
      c[j] += a[i][j];
    }
  }

  exit_proc();
}
