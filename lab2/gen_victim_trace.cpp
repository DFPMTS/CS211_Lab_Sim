#include <cstdio>
#include <cstdlib>
#include <iostream>

int main() {
  freopen("victim.trace", "w", stdout);
  int T = 100;
  srand(time(0));

  while (T--) {
    for (int i = 0; i < 100; ++i) {
      printf("r   %x\n", rand() % 1000);
      if (rand() % 10 == 0) {
        printf("r   %x\n", 8192 + (rand() % 4000));
      }
    }
  }
}