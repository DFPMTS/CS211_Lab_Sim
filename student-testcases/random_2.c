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

      "fsub.s %[fw], %[fx], %[fx];"
      "fmsub.s %[fz], %[fx], %[fz], %[fx];"
      "fsw %[fx],8(sp);"
      "fmsub.s %[fx], %[fy], %[fz], %[fx];"
      "mul %[xx], %[xx], %[xx];"
      "fsub.s %[fz], %[fz], %[fy];"
      "fsub.s %[fx], %[fx], %[fw];"
      "fmsub.s %[fz], %[fy], %[fx], %[fz];"
      "fsub.s %[fy], %[fx], %[fz];"
      "fsw %[fw],0(sp);"
      "mul %[xw], %[xx], %[xy];"
      "flw %[fx],0(sp);"
      "add %[xw], %[xy], %[xz];"
      "fsub.s %[fw], %[fz], %[fx];"
      "fmsub.s %[fy], %[fy], %[fw], %[fy];"
      "fsub.s %[fz], %[fw], %[fy];"
      "fadd.s %[fw], %[fx], %[fz];"
      "fsub.s %[fw], %[fz], %[fz];"
      "fsw %[fx],8(sp);"
      "fmsub.s %[fy], %[fx], %[fz], %[fx];"
      "fsw %[fy],8(sp);"
      "flw %[fy],12(sp);"
      "sub %[xz], %[xw], %[xx];"
      "fsw %[fw],12(sp);"
      "sub %[xz], %[xz], %[xw];"
      "fsw %[fx],8(sp);"
      "fsw %[fy],4(sp);"
      "fadd.s %[fz], %[fz], %[fw];"
      "add %[xw], %[xx], %[xw];"
      "fadd.s %[fy], %[fw], %[fz];"
      "sw %[xw],24(sp);"
      "flw %[fz],0(sp);"
      "fmadd.s %[fw], %[fx], %[fz], %[fx];"
      "mul %[xy], %[xw], %[xw];"
      "sw %[xw],24(sp);"
      "lw %[xz],28(sp);"
      "fsw %[fy],8(sp);"
      "fadd.s %[fx], %[fz], %[fy];"
      "fsub.s %[fx], %[fy], %[fz];"
      "fdiv.s %[fw], %[fz], %[fy];"
      "flw %[fz],12(sp);"
      "fmadd.s %[fz], %[fy], %[fy], %[fw];"
      "mul %[xy], %[xw], %[xz];"
      "flw %[fx],4(sp);"
      "sub %[xw], %[xy], %[xx];"
      "add %[xz], %[xx], %[xx];"
      "sub %[xy], %[xz], %[xy];"
      "sw %[xx],16(sp);"
      "flw %[fz],0(sp);"
      "fadd.s %[fx], %[fz], %[fy];"

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