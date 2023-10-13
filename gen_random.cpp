#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

std::vector<std::string> fvars = {"%[fw]"s, "%[fx]"s, "%[fy]"s, "%[fz]"s};
std::vector<std::string> xvars = {"%[xw]"s, "%[xx]"s, "%[xy]"s, "%[xz]"s};

std::string fadd_s = "fadd.s ";
std::string fsub_s = "fsub.s ";
std::string fmul_s = "fadd.s ";
std::string fdiv_s = "fdiv.s ";
std::string fsqrt_s = "fsqrt.s ";

std::string get_fvar() { return fvars[rand() % fvars.size()]; }

std::string gen_fadd() {
  return fadd_s + get_fvar() + ", " + get_fvar() + ", " + get_fvar() + ";";
}
std::string gen_fsub() {
  return fsub_s + get_fvar() + ", " + get_fvar() + ", " + get_fvar() + ";";
}
std::string gen_fmul() {
  return fmul_s + get_fvar() + ", " + get_fvar() + ", " + get_fvar() + ";";
}
std::string gen_fdiv() {
  return fdiv_s + get_fvar() + ", " + get_fvar() + ", " + get_fvar() + ";";
}

std::string gen_fsqrt() {
  return fsqrt_s + get_fvar() + ", " + get_fvar() + ";";
}

std::string gen_fmadd() {
  return "fmadd.s " + get_fvar() + ", " + get_fvar() + ", " + get_fvar() +
         ", " + get_fvar() + ";";
}

std::string gen_fmsub() {
  return "fmsub.s " + get_fvar() + ", " + get_fvar() + ", " + get_fvar() +
         ", " + get_fvar() + ";";
}

std::string get_xvar() { return xvars[rand() % xvars.size()]; }

std::string gen_xadd() {
  return "add " + get_xvar() + ", " + get_xvar() + ", " + get_xvar() + ";";
}
std::string gen_xsub() {
  return "sub " + get_xvar() + ", " + get_xvar() + ", " + get_xvar() + ";";
}
std::string gen_xmul() {
  return "mul " + get_xvar() + ", " + get_xvar() + ", " + get_xvar() + ";";
}
std::string gen_xdiv() {
  return "div " + get_xvar() + ", " + get_xvar() + ", " + get_xvar() + ";";
}

std::string fload_x() { return "flw " + get_fvar() + ",0(sp);"; }
std::string fload_y() { return "flw " + get_fvar() + ",4(sp);"; }
std::string fload_z() { return "flw " + get_fvar() + ",8(sp);"; }
std::string fload_w() { return "flw " + get_fvar() + ",12(sp);"; }

std::string fsave_x() { return "fsw " + get_fvar() + ",0(sp);"; }
std::string fsave_y() { return "fsw " + get_fvar() + ",4(sp);"; }
std::string fsave_z() { return "fsw " + get_fvar() + ",8(sp);"; }
std::string fsave_w() { return "fsw " + get_fvar() + ",12(sp);"; }

std::string xload_x() { return "lw " + get_xvar() + ",16(sp);"; }
std::string xload_y() { return "lw " + get_xvar() + ",20(sp);"; }
std::string xload_z() { return "lw " + get_xvar() + ",24(sp);"; }
std::string xload_w() { return "lw " + get_xvar() + ",28(sp);"; }

std::string xsave_x() { return "sw " + get_xvar() + ",16(sp);"; }
std::string xsave_y() { return "sw " + get_xvar() + ",20(sp);"; }
std::string xsave_z() { return "sw " + get_xvar() + ",24(sp);"; }
std::string xsave_w() { return "sw " + get_xvar() + ",28(sp);"; }

std::string get_inst() {
  int id, lw;
  id = rand() % 20;
  switch (id) {
  case 0:
    return gen_fadd();
    break;
  case 1:
    return gen_fsub();
    break;
  case 2:
    return gen_fmul();
    break;
  case 3:
    return gen_fdiv();
    break;
  case 4:
  case 17:
    lw = rand() % 4;
    switch (lw) {
    case 0:
      return fload_x();
      break;
    case 1:
      return fload_y();
      break;
    case 2:
      return fload_w();
      break;
    case 3:
      return fload_z();
      break;
    }
    break;
  case 5:
    return gen_fmadd();
    break;
  case 6:
    return gen_fmsub();
    break;
  case 7:
    return gen_fsub();
    break;
  case 8:
    return gen_fdiv();
    break;
  case 9:
    return gen_fsub();
    break;
  case 10:
    return fsave_w();
    break;
  case 11:
    return fsave_x();
    break;
  case 12:
    return fsave_y();
    break;
  case 13:
    return fsave_z();
    break;
  case 14:
    return gen_xadd();
    break;
  case 15:
    return gen_xsub();
    break;
  case 16:
    return gen_xmul();
    break;
  case 18:
    lw = rand() % 4;
    switch (lw) {
    case 0:
      return xload_x();
      break;
    case 1:
      return xload_y();
      break;
    case 2:
      return xload_w();
      break;
    case 3:
      return xload_z();
      break;
    }
    break;
  case 19:
    lw = rand() % 4;
    switch (lw) {
    case 0:
      return xsave_x();
      break;
    case 1:
      return xsave_y();
      break;
    case 2:
      return xsave_z();
      break;
    case 3:
      return xsave_w();
      break;
    }
    break;
  default:
    return "";
  }
}

int main() {
  srand(time(0));
  for (int i = 0; i < 200; ++i) {
    std::cout << "\"" << get_inst() << "\"\n";
  }
}