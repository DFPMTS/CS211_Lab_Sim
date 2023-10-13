#include "lib.h"
#include "stdio.h"

int main() {
  register float fx asm("fs2");
  register float fy asm("fs3");
  register float fz asm("fs4");
  register float fw asm("fs5");

  register long xx asm("s2");
  register long xy asm("s3");
  register long xz asm("s4");
  register long xw asm("s5");
  asm("addi sp, sp, -32;"
      "li %[xx], 1;"
      "li %[xy], 2;"
      "li %[xz], 3;"
      "li %[xw], 4;"
      "fcvt.s.w %[fx], %[xx];"
      "fcvt.s.w %[fy], %[xy];"
      "fcvt.s.w %[fz], %[xz];"
      "fcvt.s.w %[fw], %[xw];"
      "fsw %[fx], 0(sp);"
      "fsw %[fy], 4(sp);"
      "fsw %[fz], 8(sp);"
      "fsw %[fw], 12(sp);"
      "sw %[xx], 16(sp);"
      "sw %[xy], 20(sp);"
      "sw %[xz], 24(sp);"
      "sw %[xw], 28(sp);"

       "addi sp, sp, 32"
      : [fx] "+f"(fx), [fy] "+f"(fy), [fz] "+f"(fz), [fw] "+f"(fw),
        [xx] "+r"(xx), [xy] "+r"(xy), [xz] "+r"(xz), [xw] "+r"(xw));
  print_f(fx);
  print_c('\n');
  print_f(fy);
  print_c('\n');
  print_f(fz);
  print_c('\n');
  print_f(fw);
  print_c('\n');
  print_d(xx);
  print_c('\n');
  print_d(xy);
  print_c('\n');
  print_d(xz);
  print_c('\n');
  print_d(xw);
  print_c('\n');

  exit_proc();
}