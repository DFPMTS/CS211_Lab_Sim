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

      "fsw %[fx],0(sp);"
      "fdiv.s %[fx], %[fx], %[fx];"
      "fsub.s %[fw], %[fw], %[fx];"
      "fsw %[fz],4(sp);"
      "fdiv.s %[fz], %[fz], %[fw];"
      "fsw %[fy],4(sp);"
      "fdiv.s %[fw], %[fy], %[fx];"
      "fsw %[fz],0(sp);"
      "fadd.s %[fw], %[fw], %[fx];"
      "fdiv.s %[fx], %[fz], %[fw];"
      "fsub.s %[fy], %[fx], %[fw];"
      "mul %[xy], %[xx], %[xw];"
      "sw %[xz],20(sp);"
      "fdiv.s %[fx], %[fz], %[fw];"
      "fsub.s %[fx], %[fx], %[fx];"
      "fsub.s %[fy], %[fx], %[fz];"
      "fmadd.s %[fx], %[fz], %[fy], %[fy];"
      "fdiv.s %[fx], %[fy], %[fz];"
      "fadd.s %[fz], %[fw], %[fw];"
      "fsub.s %[fx], %[fw], %[fx];"
      "add %[xw], %[xx], %[xz];"
      "sub %[xx], %[xw], %[xw];"
      "fsw %[fy],12(sp);"
      "mul %[xx], %[xz], %[xz];"
      "fsw %[fw],12(sp);"
      "fmsub.s %[fw], %[fy], %[fy], %[fy];"
      "fsw %[fy],4(sp);"
      "fadd.s %[fz], %[fy], %[fy];"
      "fmadd.s %[fz], %[fy], %[fy], %[fx];"
      "add %[xz], %[xx], %[xx];"
      "fadd.s %[fx], %[fy], %[fz];"
      "sub %[xy], %[xw], %[xy];"
      "fmadd.s %[fy], %[fz], %[fz], %[fy];"
      "flw %[fw],4(sp);"
      "fadd.s %[fz], %[fy], %[fx];"
      "fsub.s %[fw], %[fy], %[fw];"
      "fsw %[fw],0(sp);"
      "add %[xx], %[xz], %[xz];"
      "fsub.s %[fw], %[fw], %[fx];"
      "fsw %[fw],0(sp);"
      "fsw %[fz],0(sp);"
      "lw %[xx],16(sp);"
      "flw %[fz],8(sp);"
      "fsub.s %[fw], %[fw], %[fx];"
      "mul %[xy], %[xy], %[xx];"
      "fsw %[fx],0(sp);"
      "fmsub.s %[fy], %[fx], %[fw], %[fz];"
      "fsw %[fz],8(sp);"
      "fmsub.s %[fx], %[fz], %[fz], %[fw];"
      "fsub.s %[fy], %[fz], %[fy];"
      "fadd.s %[fw], %[fz], %[fx];"
      "sub %[xy], %[xy], %[xz];"
      "sub %[xy], %[xy], %[xy];"
      "fsw %[fx],0(sp);"
      "fdiv.s %[fx], %[fz], %[fw];"
      "fsub.s %[fy], %[fx], %[fw];"
      "fsub.s %[fy], %[fw], %[fw];"
      "fmsub.s %[fz], %[fz], %[fx], %[fx];"
      "fsw %[fx],8(sp);"
      "fsw %[fz],8(sp);"
      "flw %[fx],8(sp);"
      "flw %[fz],8(sp);"
      "fsw %[fx],12(sp);"
      "fmsub.s %[fz], %[fw], %[fw], %[fy];"
      "flw %[fx],4(sp);"
      "fadd.s %[fy], %[fy], %[fw];"
      "fmsub.s %[fy], %[fz], %[fx], %[fx];"
      "mul %[xz], %[xz], %[xy];"
      "flw %[fz],12(sp);"
      "fdiv.s %[fz], %[fz], %[fy];"
      "fadd.s %[fy], %[fz], %[fw];"
      "fsw %[fz],8(sp);"
      "mul %[xw], %[xw], %[xy];"
      "fsub.s %[fy], %[fw], %[fz];"
      "add %[xy], %[xx], %[xx];"
      "fsub.s %[fz], %[fy], %[fy];"
      "fsw %[fw],12(sp);"
      "add %[xw], %[xz], %[xy];"
      "sw %[xw],24(sp);"
      "fsw %[fw],8(sp);"
      "flw %[fy],4(sp);"
      "fsw %[fy],0(sp);"
      "fadd.s %[fz], %[fw], %[fz];"
      "fsub.s %[fw], %[fy], %[fw];"
      "fmsub.s %[fz], %[fx], %[fx], %[fy];"
      "fdiv.s %[fw], %[fw], %[fz];"
      "fadd.s %[fw], %[fy], %[fw];"
      "sub %[xx], %[xx], %[xw];"
      "fsw %[fx],8(sp);"
      "fadd.s %[fz], %[fw], %[fw];"
      "fsub.s %[fw], %[fw], %[fw];"
      "fsub.s %[fw], %[fy], %[fx];"
      "add %[xy], %[xz], %[xz];"
      "fadd.s %[fy], %[fz], %[fx];"
      "fadd.s %[fy], %[fx], %[fw];"
      "fmsub.s %[fx], %[fx], %[fz], %[fx];"
      "sub %[xw], %[xz], %[xy];"
      "fsub.s %[fz], %[fw], %[fx];"
      "fsub.s %[fw], %[fy], %[fz];"
      "fsub.s %[fw], %[fx], %[fx];"

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