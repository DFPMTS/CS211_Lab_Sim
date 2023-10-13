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

      "add %[xz], %[xx], %[xx];"
      "fsub.s %[fw], %[fz], %[fy];"
      "fmadd.s %[fy], %[fw], %[fz], %[fx];"
      "mul %[xy], %[xw], %[xy];"
      "lw %[xw],20(sp);"
      "fsw %[fw],8(sp);"
      "sub %[xy], %[xx], %[xx];"
      "fsw %[fx],4(sp);"
      "fsw %[fw],0(sp);"
      "fmadd.s %[fy], %[fz], %[fz], %[fz];"
      "flw %[fy],8(sp);"
      "fsub.s %[fw], %[fw], %[fz];"
      "fsw %[fy],0(sp);"
      "fsw %[fy],0(sp);"
      "fmsub.s %[fx], %[fw], %[fz], %[fx];"
      "fmsub.s %[fz], %[fw], %[fw], %[fw];"
      "add %[xw], %[xx], %[xx];"
      "fsw %[fz],4(sp);"
      "sub %[xz], %[xy], %[xy];"
      "flw %[fx],4(sp);"
      "fdiv.s %[fx], %[fx], %[fy];"
      "fsub.s %[fz], %[fx], %[fz];"
      "fsub.s %[fy], %[fz], %[fw];"
      "fadd.s %[fx], %[fw], %[fw];"
      "fmadd.s %[fw], %[fy], %[fx], %[fy];"
      "mul %[xx], %[xx], %[xz];"
      "fmadd.s %[fz], %[fy], %[fx], %[fw];"
      "fsub.s %[fw], %[fx], %[fw];"
      "fadd.s %[fy], %[fw], %[fz];"
      "fmadd.s %[fx], %[fx], %[fx], %[fy];"
      "flw %[fy],12(sp);"
      "fadd.s %[fw], %[fy], %[fw];"
      "fsub.s %[fw], %[fw], %[fz];"
      "fsub.s %[fw], %[fw], %[fw];"
      "add %[xx], %[xw], %[xx];"
      "fmadd.s %[fy], %[fx], %[fz], %[fw];"
      "fmadd.s %[fy], %[fy], %[fz], %[fz];"
      "sw %[xx],20(sp);"
      "fsub.s %[fz], %[fz], %[fx];"
      "fsw %[fz],8(sp);"
      "fsub.s %[fy], %[fw], %[fy];"
      "sw %[xy],16(sp);"
      "fsw %[fy],4(sp);"
      "sub %[xw], %[xz], %[xw];"
      "lw %[xz],16(sp);"
      "sw %[xx],24(sp);"
      "fmadd.s %[fx], %[fw], %[fw], %[fy];"
      "fsw %[fx],8(sp);"
      "fmadd.s %[fw], %[fz], %[fy], %[fw];"
      "fadd.s %[fw], %[fw], %[fw];"

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