/*
*************************************************************

  Modified from test/quicksort.c in the startup codebase.

*************************************************************
*/
#include "lib.h"

void quicksort(int *a, int begin, int end) {
  int i, j, t, pivot;
  if (begin > end)
    return;

  pivot = a[begin];
  i = begin;
  j = end;
  while (i != j) {
    while (a[j] >= pivot && i < j)
      j--;
    while (a[i] <= pivot && i < j)
      i++;
    if (i < j) {
      t = a[i];
      a[i] = a[j];
      a[j] = t;
    }
  }
  a[begin] = a[i];
  a[i] = pivot;

  quicksort(a, begin, i - 1);
  quicksort(a, i + 1, end);
}

int main() {
  const int M = 1000;
  int b[M];
  for (int i = 0; i < M; ++i) {
    b[i] = M - i;
  }
  print_s("Prev B: ");
  for (int i = 0; i < M; ++i) {
    print_d(b[i]);
    print_s(" ");
  }
  print_s("\n");

  print_s("Sorted B: ");
  quicksort(b, 0, M - 1);
  for (int i = 0; i < M; ++i) {
    print_d(b[i]);
    print_s(" ");
  }
  print_s("\n");

  exit_proc();
}