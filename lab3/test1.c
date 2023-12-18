#include "lib.h"

int a[20];

int main() {
  int len = 20;
  for (int i = 0; i < len; ++i) {
    a[i] = i;
  }
  for (int i = 0; i < len; ++i) {
    print_d(a[i]);
    print_c(' ');
  }
  print_c('\n');

  asm("add a0, zero,%[a];"
      "add a1, zero,%[len];"
      "li a7, 7;"
      "ecall;"
      "li a7, 2;"
      "ecall;" ::[a] "r"(a),
      [len] "r"(len));

  print_c('\n');
  for (int i = 0; i < len; ++i) {
    print_d(a[i]);
    print_c(' ');
  }
  print_c('\n');

  exit_proc();
}
