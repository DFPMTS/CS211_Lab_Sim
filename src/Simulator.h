/*
 * Main Body for the RISCV Simulator
 *
 * Created by He, Hao at 2019-3-11
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <bit>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <string>
#include <vector>

inline int VERBOSE = 0;

#include "BranchPredictor.h"
#include "MemoryManager.h"

namespace RISCV {

const int REGNUM = 32;
extern const char *REGNAME[32];
extern const char *REGNAME_F[32];
typedef uint32_t RegId;
enum Reg {
  REG_ZERO = 0,
  REG_RA = 1,
  REG_SP = 2,
  REG_GP = 3,
  REG_TP = 4,
  REG_T0 = 5,
  REG_T1 = 6,
  REG_T2 = 7,
  REG_S0 = 8,
  REG_S1 = 9,
  REG_A0 = 10,
  REG_A1 = 11,
  REG_A2 = 12,
  REG_A3 = 13,
  REG_A4 = 14,
  REG_A5 = 15,
  REG_A6 = 16,
  REG_A7 = 17,
  REG_S2 = 18,
  REG_S3 = 19,
  REG_S4 = 20,
  REG_S5 = 21,
  REG_S6 = 22,
  REG_S7 = 23,
  REG_S8 = 24,
  REG_S9 = 25,
  REG_S10 = 26,
  REG_S11 = 27,
  REG_T3 = 28,
  REG_T4 = 29,
  REG_T5 = 30,
  REG_T6 = 31,
};

enum InstType {
  R_TYPE,
  I_TYPE,
  S_TYPE,
  SB_TYPE,
  U_TYPE,
  UJ_TYPE,
};
enum Inst {
  LUI = 0,
  AUIPC = 1,
  JAL = 2,
  JALR = 3,
  BEQ = 4,
  BNE = 5,
  BLT = 6,
  BGE = 7,
  BLTU = 8,
  BGEU = 9,
  LB = 10,
  LH = 11,
  LW = 12,
  LD = 13,
  LBU = 14,
  LHU = 15,
  SB = 16,
  SH = 17,
  SW = 18,
  SD = 19,
  ADDI = 20,
  SLTI = 21,
  SLTIU = 22,
  XORI = 23,
  ORI = 24,
  ANDI = 25,
  SLLI = 26,
  SRLI = 27,
  SRAI = 28,
  ADD = 29,
  SUB = 30,
  SLL = 31,
  SLT = 32,
  SLTU = 33,
  XOR = 34,
  SRL = 35,
  SRA = 36,
  OR = 37,
  AND = 38,
  ECALL = 39,
  ADDIW = 40,
  MUL = 41,
  MULH = 42,
  DIV = 43,
  REM = 44,
  LWU = 45,
  SLLIW = 46,
  SRLIW = 47,
  SRAIW = 48,
  ADDW = 49,
  SUBW = 50,
  SLLW = 51,
  SRLW = 52,
  SRAW = 53,

  // lab0: F extension
  FMV_W_X = 54,
  FMV_X_W = 55,

  FCVT_S_W = 56,
  FCVT_W_S = 57,

  FLW = 58,
  FSW = 59,

  FADD_S = 60,
  FSUB_S = 61,
  FMUL_S = 62,
  FDIV_S = 63,
  FSQRT_S = 64,

  FMADD_S = 65,
  FMSUB_S = 66,
  FSGNJ_S = 67,
  // lab0: end

  UNKNOWN = -1,
};
enum class RegType { INT, FLOAT, INVALID };
extern const char *INSTNAME[];

// Opcode field
const int OP_REG = 0x33;
const int OP_IMM = 0x13;
const int OP_LUI = 0x37;
const int OP_BRANCH = 0x63;
const int OP_STORE = 0x23;
const int OP_LOAD = 0x03;
const int OP_SYSTEM = 0x73;
const int OP_AUIPC = 0x17;
const int OP_JAL = 0x6F;
const int OP_JALR = 0x67;
const int OP_IMM32 = 0x1B;
const int OP_32 = 0x3B;

// lab0: F extension
const int OP_FP = 0x53;
const int OP_FMADD_S = 0x43;
const int OP_FMSUB_S = 0x47;
const int OP_LOAD_FP = 0x07;
const int OP_STORE_FP = 0x27;
// lab0: end

inline bool isBranch(Inst inst) {
  if (inst == BEQ || inst == BNE || inst == BLT || inst == BGE ||
      inst == BLTU || inst == BGEU) {
    return true;
  }
  return false;
}

inline bool isJump(Inst inst) {
  if (inst == JAL || inst == JALR) {
    return true;
  }
  return false;
}

inline bool isReadMem(Inst inst) {
  if (inst == LB || inst == LH || inst == LW || inst == LD || inst == LBU ||
      inst == LHU || inst == LWU || inst == FLW) {
    return true;
  }
  return false;
}

class Simulator {

public:
  bool isSingleStep;
  bool verbose;
  bool shouldDumpHistory;
  uint64_t pc;
  uint64_t
      predictedPC;    // for branch prediction module, predicted PC destination
  uint64_t anotherPC; // // another possible prediction destination
  uint64_t reg[RISCV::REGNUM];
  // lab0: F extension
  float reg_f[RISCV::REGNUM];
  // lab0: end
  uint32_t stackBase;
  uint32_t maximumStackSize;
  MemoryManager *memory;
  BranchPredictor *branchPredictor;

  Simulator(MemoryManager *memory, BranchPredictor *predictor);
  ~Simulator();

  void initStack(uint32_t baseaddr, uint32_t maxSize);

  void simulate();

  void dumpHistory();

  void printInfo();

  void printStatistics();

private:
  struct FReg {
    // Control Signals
    bool bubble;
    uint32_t stall;

    uint64_t pc;
    uint32_t inst;
    uint32_t len;
  } fReg, fRegNew;
  struct DReg {
    // Control Signals
    bool bubble;
    uint32_t stall;
    RISCV::RegId rs1, rs2, rs3;

    uint64_t pc;
    RISCV::Inst inst;
    int64_t op1;
    int64_t op2;
    float op1_f;
    float op2_f;
    float op3_f;
    RISCV::RegId dest;
    int64_t offset;
    bool predictedBranch;
    RISCV::RegType rs1_reg_type;
    RISCV::RegType rs2_reg_type;
    RISCV::RegType rs3_reg_type;
    RISCV::RegType rd_reg_type;
  } dReg, dRegNew;
  struct EReg {
    // Control Signals
    bool bubble;
    uint32_t stall;

    uint64_t pc;
    RISCV::Inst inst;
    int64_t op1;
    int64_t op2;
    float op2_f;
    bool writeReg;
    RISCV::RegId destReg;
    int64_t out;
    float out_f;
    bool writeMem;
    bool readMem;
    bool readSignExt;
    uint32_t memLen;
    bool branch;
    RISCV::RegType rs1_reg_type;
    RISCV::RegType rs2_reg_type;
    RISCV::RegType rs3_reg_type;
    RISCV::RegType rd_reg_type;
  };
  struct MReg {
    // Control Signals
    bool bubble;
    uint32_t stall;

    uint64_t pc;
    RISCV::Inst inst;
    int64_t op1;
    int64_t op2;
    int64_t out;
    float out_f;
    bool writeReg;
    RISCV::RegId destReg;
    RISCV::RegType rs1_reg_type;
    RISCV::RegType rs2_reg_type;
    RISCV::RegType rs3_reg_type;
    RISCV::RegType rd_reg_type;
  };

  // Pipeline Related Variables
  // To avoid older values(in MEM) overriding newer values(in EX)
  bool executeWriteBack;
  RISCV::RegId executeWBReg;
  RISCV::RegType executeWBRegType;
  bool memoryWriteBack;
  RISCV::RegId memoryWBReg;
  RISCV::RegType memoryWBRegType;

  // Components in the execute stage
  // Reference:
  // https://docs.boom-core.org/en/latest/sections/intro-overview/boom-pipeline.html
  enum executeComponent {
    ALU,
    // memCalc,
    dataMem,
    branchALU,
    iMul,
    iDiv,
    int2FP,
    fp2Int,
    fpDiv,
    fmaAdd, /* fused multiply-add unit for fp */
    fmaMul, /* fused multiply-add unit for fp */
    number_of_component,
    unknown = ALU,
  };

  // The lowest cycle of an datamem access
  const uint32_t datamem_lat_lower_bound = 1;
  // The lock cast on the stage where the next stage is busy across multiple
  // cycles
  static const uint32_t datamem_stall_lock = UINT32_MAX;

  static constexpr uint32_t latency[number_of_component] = {
      /* ALU */ 0,
      // /* memCalc */ 1,
      /* dataMem */ datamem_stall_lock,
      /* branchALU */ 0,
      /* iMul */ 2,
      /* iDiv */ 6,
      /* int2FP */ 0,
      /* FP2int */ 0,
      /* fpDiv */ 24,
      /* fmaAdd */ 3,
      /* fmaMul */ 6,
  };

  static inline bool isLoad(RISCV::Inst inst) {
    return (inst == LB || inst == LH || inst == LW || inst == FLW ||
            inst == LD || inst == LBU || inst == LHU || inst == LWU);
  }

  static inline bool isStore(RISCV::Inst inst) {
    return (inst == SB || inst == SH || inst == SW || inst == FSW ||
            inst == SD);
  }

  static inline executeComponent getComponentUsed(RISCV::Inst inst) {
    { // start of using namespace, to reduce code duplication
      using namespace RISCV;
      switch (inst) {
        // fp
      case FMV_W_X:
      case FMV_X_W:
        return ALU;
        break;

      case FCVT_S_W:
        return int2FP;
        break;

      case FCVT_W_S:
        return fp2Int;
        break;

      case FLW:
        return dataMem;
        break;

      case FSW:
        // return memCalc;
        return dataMem;
        break;

      case FADD_S:
      case FSUB_S:
        return fmaAdd;

      case FMUL_S:
        return fmaMul;

      case FDIV_S:
      case FSQRT_S:
        return fpDiv;

      case FMADD_S:
      case FMSUB_S:
        // ! attention
        return fmaMul;

        /* When using the instructions below,
       general ALU is used */
      case ADDI:
      case ADD:
      case ADDIW:
      case ADDW:
      case SUB:
      case SUBW:
      case SLTI:
      case SLT:
      case SLTIU:
      case SLTU:
      case XORI:
      case XOR:
      case ORI:
      case OR:
      case ANDI:
      case AND:
      case SLLI:
      case SLL:
      case SLLIW:
      case SLLW:
      case SRLI:
      case SRL:
      case SRLIW:
      case SRLW:
      case SRAI:
      case SRA:
      case SRAW:
      case SRAIW:
        return ALU;
        break;
      /* When using the instructions below,
         ALU for memory address calculation is used */
      case SB:
      case SH:
      case SW:
      case SD:
        // return memCalc;
        return dataMem;
        break;
      /* When using the instructions below,
         datamem is used */
      case LB:
      case LH:
      case LW:
      case LD:
      case LBU:
      case LHU:
      case LWU:
        return dataMem;
        break;
      /* When using the instructions below,
         ALU for branch offset calculation is used */
      case LUI:
      case AUIPC:
      case JAL:
      case JALR:
      case BEQ:
      case BNE:
      case BLT:
      case BGE:
      case BLTU:
      case BGEU:
        return branchALU;
        break;
      /* When using the instructions below,
         integer multiplier is used */
      case MUL:
        return iMul;
        break;
      /* When using the instructions below,
         integer divider is used */
      case DIV:
        return iDiv;
        break;
        // YOUR CODE HERE
        // case INST:
        // ...
        // return COMPONENT_TYPE; break;

      case ECALL:
        // ! assuming magical syscall
        return ALU;
        break;

      default:
        return unknown;
        break;
      }
    } // end of using namespace RISCV;

    return unknown;
  }

  struct History {
    uint32_t instCount;
    uint32_t cycleCount;
    uint32_t stalledCycleCount;

    uint32_t predictedBranch; // Number of branch that is predicted successfully
    uint32_t unpredictedBranch; // Number of branch that is not predicted
                                // successfully

    uint32_t dataHazardCount;
    uint32_t controlHazardCount;
    uint32_t memoryHazardCount;

    std::vector<std::string> instRecord;
    std::vector<std::string> regRecord;

    std::string memoryDump;
  } history;

  static bool same_reg(RISCV::RegType type1, RISCV::RegId id1,
                       RISCV::RegType type2, RISCV::RegId id2) {
    if (type1 == RegType::INVALID || type2 == RegType::INVALID)
      return false;
    if (id1 == -1 || id2 == -1)
      return false;
    return (type1 == type2) && (id1 == id2);
  }

  using ROBIndex = int;

  struct ROBEntry {
    ROBEntry()
        : ready(false), busy(false), inst(UNKNOWN), rd_type(RegType::INVALID) {}
    bool ready;
    bool busy;
    Inst inst;
    RegType rd_type;
    RegId rd;
    uint64_t x_rd;
    float f_rd;
    uint32_t A;
    std::string inst_info;
    bool branch_misprediction;
    uint32_t target_pc;
  };

  struct ROB {
    ROB(int rob_size)
        : front(0), end(0), size(0), capacity(rob_size), rob(rob_size) {}

    bool free() { return size < capacity; }

    void clear() {
      for (int i = 0; i < rob.size(); ++i) {
        rob[i].busy = false;
      }
      size = 0;
      front = 0;
      end = 0;
    }

    void commit(ROB &robNew, Simulator *simulator) {
      if (VERBOSE)
        std::cerr << "ROB size: " << size << "\n";
      if (!size)
        return;
      if (VERBOSE)
        std::cerr << "Commit: " << front << "\n";
      auto &entry = rob[front];
      auto &new_entry = robNew[front];
      if (this->size && entry.busy && (entry.ready || entry.inst == ECALL)) {
        if (isStore(entry.inst)) {
          static int store_timeout = -1;
          if (store_timeout == -1) {
            uint32_t cycles = 0;
            bool good = false;
            switch (entry.inst) {
            case SB:
              good = simulator->memory->setByte(entry.A, entry.x_rd, &cycles);
              break;
            case SH:
              good = simulator->memory->setShort(entry.A, entry.x_rd, &cycles);
              break;
            case SW:
              good = simulator->memory->setInt(entry.A, entry.x_rd, &cycles);
              break;
            case FSW:
              good = simulator->memory->setInt(
                  entry.A, std::bit_cast<int>(entry.f_rd), &cycles);
              break;
            case SD:
              good = simulator->memory->setLong(entry.A, entry.x_rd, &cycles);
              break;
            default:
              assert(0);
            }
            // assert(good);
            store_timeout = cycles;
          } else {
            // * will be available by next cycle
            if (store_timeout == 0) {
              new_entry.busy = false;
              robNew.increase(robNew.front);
              --robNew.size;
              store_timeout = -1;
            } else {
              --store_timeout;
            }
          }
        } else {
          if (entry.inst == ECALL) {
            entry.x_rd = simulator->handleSystemCall(simulator->reg[REG_A0],
                                                     simulator->reg[REG_A7]);
            for (int j = 0; j < simulator->tomasuloNew.rs.size(); ++j) {
              auto &rs = simulator->tomasuloNew.rs[j];
              rs.update_ready(entry.rd_type, entry.rd, entry.x_rd, .0f, front);
            }
            if (VERBOSE) {
              std::cerr << "INPUT:  " << entry.x_rd << "\n";
            }
          }
          if (entry.rd_type == RegType::INT && entry.rd != 0) {
            if (VERBOSE)
              std::cerr << " Write back integer: " << entry.rd << " "
                        << entry.x_rd << '\n';
            simulator->reg[entry.rd] = entry.x_rd;
            // * may overwrite
            // * consider a issue in the same cycle
            // * must check tomasuloNew
            if (simulator->tomasuloNew.result_status_x[entry.rd] == front)
              simulator->tomasuloNew.result_status_x[entry.rd] = -1;
          } else if (entry.rd_type == RegType::FLOAT) {
            if (VERBOSE)
              std::cerr << " Write back float: " << entry.rd << " "
                        << entry.f_rd << '\n';
            simulator->reg_f[entry.rd] = entry.f_rd;
            if (simulator->tomasuloNew.result_status_f[entry.rd] == front)
              simulator->tomasuloNew.result_status_f[entry.rd] = -1;
          }
          new_entry.busy = false;
          robNew.increase(robNew.front);
          --robNew.size;
          if ((isBranch(entry.inst) || isJump(entry.inst)) &&
              entry.branch_misprediction) {
            // * clear ROB
            robNew.clear();
            // * clear RS
            simulator->tomasuloNew.clearRS();
            // * clear RegStat
            memset(simulator->tomasuloNew.result_status_x, -1,
                   sizeof(simulator->tomasuloNew.result_status_x));
            memset(simulator->tomasuloNew.result_status_f, -1,
                   sizeof(simulator->tomasuloNew.result_status_f));
            // * redirect control flow
            simulator->pc = entry.target_pc;
            if (VERBOSE)
              std::cerr << "Redirect:  " << simulator->pc << "\n";
            simulator->fReg.stall = 0;
            simulator->dReg.stall = 0;
            simulator->decode_hold = 0;
            simulator->fRegNew.bubble = true;
            simulator->dRegNew.bubble = true;
          }
        }
      }
    }

    ROBEntry &operator[](int index) { return rob[index]; }

    int issue(DReg &d_reg, ROB &robNew, std::string inst_info) {
      auto &entry = robNew.rob[robNew.end];

      entry.ready = false;
      entry.busy = true;
      entry.inst = d_reg.inst;
      entry.rd_type = d_reg.rd_reg_type;
      entry.rd = d_reg.dest;
      entry.inst_info = inst_info;

      auto rob_index = robNew.end;

      increase(robNew.end);
      ++robNew.size;

      return rob_index;
    }

    void increase(int &x) {
      ++x;
      if (x >= capacity)
        x -= capacity;
    }

    bool checkLoad(int rob_index, uint32_t A) {
      int check_front = front;
      while (check_front != rob_index) {
        if (isStore(rob[check_front].inst)) {
          if (std::abs((long long)rob[check_front].A - (long long)A) < 8)
            return false;
        }
        increase(check_front);
      }
      return true;
    }

    void print() {
      int print_front = front;
      if (size) {
        std::cerr << "(" << print_front << ")" << rob[print_front].inst_info
                  << "\n";
        increase(print_front);
      }
      while (print_front != end) {
        std::cerr << "(" << print_front << ")" << rob[print_front].inst_info
                  << "\n";
        increase(print_front);
      }
    }

    int front;
    int end;
    int size;
    int capacity;
    std::vector<ROBEntry> rob;
  };

  struct ReservationStation {
    void clear() {
      busy = false;
      inst = UNKNOWN;
      dispatched = false;

      offset = 0;

      rd_type = RegType::INVALID;
      rd = -1;

      rs1_type = RegType::INVALID;
      rs1 = -1;
      rs1_ready = false;
      rs1_ROB = -1;

      rs2_type = RegType::INVALID;
      rs2 = -1;
      rs2_ready = false;
      rs2_ROB = -1;

      rs3_type = RegType::INVALID;
      rs3 = -1;
      rs3_ready = false;
      rs3_ROB = -1;
    }

    bool ready() { return rs1_ready && rs2_ready && rs3_ready; }

    void update_ready(RegType other_rd_type, RegId other_rd, uint64_t x_rd,
                      float f_rd, ROBIndex rob_index) {
      if (!busy || dispatched)
        return;
      if (rs1_ROB == rob_index &&
          same_reg(other_rd_type, other_rd, rs1_type, rs1)) {
        rs1_ready = true;
        op1 = x_rd;
        op1_f = f_rd;
        if (VERBOSE)
          std::cerr << "Updated rs1 of " << inst_info << "\n";
      }
      if (rs2_ROB == rob_index &&
          same_reg(other_rd_type, other_rd, rs2_type, rs2)) {
        rs2_ready = true;
        op2 = x_rd;
        op2_f = f_rd;
        if (VERBOSE)
          std::cerr << "Updated rs2 of " << inst_info << "\n";
      }
      if (rs3_ROB == rob_index &&
          same_reg(other_rd_type, other_rd, rs3_type, rs3)) {
        rs3_ready = true;
        op3_f = f_rd;
        if (VERBOSE)
          std::cerr << "Updated rs3 of " << inst_info << "\n";
      }
    }
    std::string inst_info;

    executeComponent fu;
    Inst inst;
    bool busy;
    bool dispatched;
    uint64_t pc;
    int offset;
    uint32_t A;
    int rob_index;
    bool branch_misprediction;
    int predicted_branch;
    uint32_t target_pc;

    RegType rd_type;
    RegId rd;

    RegType rs1_type;
    RegId rs1;
    int64_t op1;
    float op1_f;
    bool rs1_ready;
    ROBIndex rs1_ROB;

    RegType rs2_type;
    RegId rs2;
    int64_t op2;
    float op2_f;
    bool rs2_ready;
    ROBIndex rs2_ROB;

    RegType rs3_type;
    RegId rs3;
    float op3_f;
    bool rs3_ready;
    ROBIndex rs3_ROB;

    int64_t out;
    float out_f;
    int timeout;
  };

  struct Tomasulo {
    Tomasulo() : rob(number_of_component * 2), rs(number_of_component * 2) {
      std::fill(FU_busy, FU_busy + number_of_component, 0);
      for (int i = 0; i < number_of_component; ++i) {
        rs[2 * i].fu = (executeComponent)i;
        rs[2 * i + 1].fu = (executeComponent)i;
      }
      memset(result_status_x, -1, sizeof(result_status_x));
      memset(result_status_f, -1, sizeof(result_status_f));
    }

    int get_free_RS(executeComponent fu) {
      int free_index = -1;
      if (VERBOSE) {
        for (int i = 0; i < rs.size(); ++i) {
          std::cerr << rs[i].busy << " ";
        }
        std::cerr << "\n";
      }
      for (int i = 0; i < rs.size(); ++i) {
        if (rs[i].fu == fu && !rs[i].busy) {
          free_index = i;
          break;
        }
      }
      return free_index;
    }

    void clearRS() {
      for (int i = 0; i < rs.size(); ++i) {
        rs[i].busy = false;
      }
      memset(FU_busy, 0, sizeof(FU_busy));
    }

    // * try to issue an instruction
    // return false: 1. FU needed busy 3. result register pending
    bool issue(DReg &d_reg, FReg &f_reg, Tomasulo &tomasulo,
               Tomasulo &tomasuloNew, std::string inst_info) {
      // * ROB available
      if (!rob.free()) {
        if (VERBOSE)
          std::cerr << "No enough ROB!\n";
        return false;
      }
      // check FU status
      executeComponent fu = getComponentUsed(d_reg.inst);
      // * RS available
      auto rs_index = get_free_RS(fu);
      if (rs_index == -1) {
        if (VERBOSE)
          std::cerr << "No enough RS!\n";
        return false;
      }

      // * ROB issue
      auto rob_index = tomasulo.rob.issue(d_reg, tomasuloNew.rob, inst_info);

      if (VERBOSE)
        std::cerr << "ROB:  " << rob_index << "   "
                  << "RS:  " << rs_index << "\n";

      // * RS issue
      auto &entry = tomasuloNew.rs[rs_index];
      // if (d_reg.rd_reg_type == RegType::INT) {
      //   if (result_status_x[d_reg.dest] != -1)
      //     return false;
      // } else if (d_reg.rd_reg_type == RegType::FLOAT) {
      //   if (result_status_f[d_reg.dest] != -1)
      //     return false;
      // }
      // // clear to issue
      entry.A = -1;
      entry.inst_info = inst_info;

      entry.inst = d_reg.inst;
      entry.busy = true;
      entry.dispatched = false;
      entry.timeout = latency[fu];
      entry.offset = d_reg.offset;
      entry.pc = d_reg.pc;
      entry.branch_misprediction = false;
      entry.predicted_branch = d_reg.predictedBranch;

      entry.rob_index = rob_index;

      entry.op1 = d_reg.op1;
      entry.op2 = d_reg.op2;
      entry.op1_f = d_reg.op1_f;
      entry.op2_f = d_reg.op2_f;
      entry.op3_f = d_reg.op3_f;

      entry.rd = d_reg.dest;
      entry.rd_type = d_reg.rd_reg_type;
      if (entry.rd == 0 && entry.rd_type == RegType::INT) {
        entry.rd_type = RegType::INVALID;
      }
      if (entry.rd_type == RegType::INT) {
        tomasuloNew.result_status_x[entry.rd] = rob_index;
        if (VERBOSE)
          std::cerr << "RegStat: " << REGNAME[entry.rd] << " <- " << rob_index
                    << "\n";
      } else if (entry.rd_type == RegType::FLOAT) {
        tomasuloNew.result_status_f[entry.rd] = rob_index;
      }
      if (VERBOSE) {
        std::cerr << "Op status: "
                  << "\n";

        std::cerr << " Offset: " << entry.offset << "\n";
      }

      entry.rs1 = d_reg.rs1;
      entry.rs1_type = d_reg.rs1_reg_type;
      if (entry.rs1_type == RegType::INT) {
        if (result_status_x[entry.rs1] == -1) {
          entry.rs1_ready = true;
          entry.op1 = d_reg.op1;
          if (VERBOSE)
            std::cerr << " Op1 ready: " << entry.op1 << "\n";
        } else if (tomasulo.rob[result_status_x[entry.rs1]].ready) {
          entry.rs1_ready = true;
          entry.op1 = tomasulo.rob[result_status_x[entry.rs1]].x_rd;
          if (VERBOSE)
            std::cerr << " Op1 from ROB: " << result_status_x[entry.rs1]
                      << " : " << entry.op1 << "\n";
        } else {
          entry.rs1_ready = false;
          if (VERBOSE)
            std::cerr << "Pending on ROB:  " << result_status_x[entry.rs1]
                      << "\n";
        }
        entry.rs1_ROB = result_status_x[entry.rs1];
      } else if (entry.rs1_type == RegType::FLOAT) {
        if (result_status_f[entry.rs1] == -1) {
          entry.rs1_ready = true;
          entry.op1_f = d_reg.op1_f;
        } else if (tomasulo.rob[result_status_f[entry.rs1]].ready) {
          entry.rs1_ready = true;
          entry.op1_f = tomasulo.rob[result_status_f[entry.rs1]].f_rd;
        } else {
          entry.rs1_ready = false;
        }
        entry.rs1_ROB = result_status_f[entry.rs1];
      } else {
        entry.rs1_ready = true;
      }

      entry.rs2 = d_reg.rs2;
      entry.rs2_type = d_reg.rs2_reg_type;
      if (entry.rs2_type == RegType::INT) {
        if (result_status_x[entry.rs2] == -1) {
          entry.rs2_ready = true;
          entry.op2 = d_reg.op2;
          if (VERBOSE)
            std::cerr << " Op2 ready: " << entry.op2 << "\n";
        } else if (tomasulo.rob[result_status_x[entry.rs2]].ready) {
          entry.rs2_ready = true;
          entry.op2 = tomasulo.rob[result_status_x[entry.rs2]].x_rd;
          if (VERBOSE)
            std::cerr << " Op2 from ROB: " << result_status_x[entry.rs2]
                      << " : " << entry.op2 << "\n";
        } else {
          entry.rs2_ready = false;
          if (VERBOSE)
            std::cerr << "Pending on ROB:  " << result_status_x[entry.rs2]
                      << "\n";
        }
        entry.rs2_ROB = result_status_x[entry.rs2];
      } else if (entry.rs2_type == RegType::FLOAT) {
        if (result_status_f[entry.rs2] == -1) {
          entry.rs2_ready = true;
          entry.op2_f = d_reg.op2_f;
        } else if (tomasulo.rob[result_status_f[entry.rs2]].ready) {
          entry.rs2_ready = true;
          entry.op2_f = tomasulo.rob[result_status_f[entry.rs2]].f_rd;
        } else {
          entry.rs2_ready = false;
        }
        entry.rs2_ROB = result_status_f[entry.rs2];
      } else {
        entry.rs2_ready = true;
      }

      entry.rs3 = d_reg.rs3;
      entry.rs3_type = d_reg.rs3_reg_type;
      if (entry.rs3_type == RegType::FLOAT) {
        if (result_status_f[entry.rs3] == -1) {
          entry.rs3_ready = true;
          entry.op3_f = d_reg.op3_f;
        } else if (tomasulo.rob[result_status_f[entry.rs3]].ready) {
          entry.rs3_ready = true;
          entry.op3_f = tomasulo.rob[result_status_f[entry.rs3]].f_rd;
        } else {
          entry.rs3_ready = false;
        }
        entry.rs3_ROB = result_status_f[entry.rs3];
      } else {
        entry.rs3_ready = true;
      }

      // if (d_reg.inst == FMADD_S || d_reg.inst == FMSUB_S) {
      //   // also lock fmaAdd
      //   scoreboardNew.fu_status[fmaAdd].busy = true;
      //   scoreboardNew.fu_status[fmaAdd].timeout = -1;
      // }

      // // * stall fetch & decode for branch / jump
      // if (isBranch(d_reg.inst) || isJump(d_reg.inst)) {
      //   f_reg.stall = datamem_stall_lock;
      //   d_reg.stall = datamem_stall_lock;
      // }

      if (entry.inst == ECALL) {
        entry.busy = false;
      }

      return true;
    }

    // 1. execute instructions when source operands are ready
    // 2. retire instructions when there's no WAR hazard
    void tick(Simulator *simulator, Tomasulo &tomasuloNew) {
      // printf("memCalc FU %d   %d   %d   %d   %d\n", fu_status[0].inst,
      //        fu_status[0].busy, fu_status[0].ready(), fu_status[0].rs1_ready,
      //        fu_status[0].rs2_ready);
      for (int i = 0; i < rs.size(); ++i)
        if (rs[i].busy && rs[i].ready() && rs[i].dispatched) {
          if (VERBOSE)
            std::cerr
                << "--------------------------EXECUTED---------------------"
                   "---------\n";
          if (isLoad(rs[i].inst) && rs[i].A == -1) {
            // * Load step 1
            tomasuloNew.rs[i].A = rs[i].op1 + rs[i].offset;
            // * bypass
            FU_busy[rs[i].fu] = false;
            tomasuloNew.FU_busy[rs[i].fu] = false;
            tomasuloNew.rs[i].dispatched = false;
            tomasuloNew.rs[i].dispatched = false;
          } else if (rs[i].timeout == 0) {
            // * writeback
            if (VERBOSE)
              printf("FU:  %d completed\n", rs[i].fu);
            // writeback
            auto &rob_entry = tomasuloNew.rob[rs[i].rob_index];

            // * value write back
            if (rs[i].rd_type == RegType::INT && rs[i].rd != 0) {
              rob_entry.x_rd = rs[i].out;
            } else if (rs[i].rd_type == RegType::FLOAT) {
              rob_entry.f_rd = rs[i].out_f;
            }
            rob_entry.target_pc = rs[i].target_pc;
            rob_entry.branch_misprediction = rs[i].branch_misprediction;

            if (isStore(rs[i].inst)) {
              rob_entry.A = rs[i].out;
              rob_entry.x_rd = rs[i].op2;
              rob_entry.f_rd = rs[i].op2_f;
            }
            rob_entry.ready = true;
            if (VERBOSE) {
              std::cerr << "ROB ready: (" << rs[i].rob_index << ")"
                        << "\n";
              std::cerr << "Complete execution: " << rs[i].inst_info << "\n";
            }

            // * bypass
            FU_busy[rs[i].fu] = false;
            tomasuloNew.FU_busy[rs[i].fu] = false;
            for (int j = 0; j < rs.size(); ++j) {
              if (j != i) {
                tomasuloNew.rs[j].update_ready(rs[i].rd_type, rs[i].rd,
                                               rs[i].out, rs[i].out_f,
                                               rs[i].rob_index);
              }
              if (VERBOSE)
                if (tomasuloNew.rs[j].busy && !tomasuloNew.rs[j].ready()) {
                  std::cerr << tomasuloNew.rs[j].inst_info << " "
                            << (!tomasuloNew.rs[j].rs1_ready ? " (x)rs1" : "")
                            << (!tomasuloNew.rs[j].rs2_ready ? " (x)rs2" : "")
                            << (!tomasuloNew.rs[j].rs3_ready ? " (x)rs3" : "")
                            << "\n";
                }
            }
            if (VERBOSE)
              std::cerr << "----------<><><><>\n";

            if (rs[i].fu == dataMem) {
              if (VERBOSE)
                std::cerr << ">>>>LOAD/STORE finished!: " << rs[i].offset
                          << "\n";
              // * maintain order for load/store buffer
              bool moved = false;
              for (int j = i + 1; j < rs.size() && rs[j].fu == dataMem; ++j) {
                tomasuloNew.rs[j - 1] = tomasuloNew.rs[j];
                tomasuloNew.rs[j].busy = false;
                rs[j - 1] = rs[j];
                rs[j].busy = false;
                moved = true;
              }
              if (!moved) {
                tomasuloNew.rs[i].busy = false;
              }
            } else {
              // * just remove from RS
              tomasuloNew.rs[i].busy = false;
            }
          } else {
            if (VERBOSE)
              printf("Timeout:    %d\n", rs[i].timeout);
            tomasuloNew.rs[i].timeout = rs[i].timeout - 1;
          }
        }

      for (int i = 0; i < rs.size(); ++i) {
        if (VERBOSE)
          if (rs[i].busy && !rs[i].ready()) {
            std::cerr << rs[i].inst_info << " "
                      << (!rs[i].rs1_ready ? " (x)rs1" : "")
                      << (!rs[i].rs2_ready ? " (x)rs2" : "")
                      << (!rs[i].rs3_ready ? " (x)rs3" : "") << "\n";
          }
        if (rs[i].busy && rs[i].ready() && !rs[i].dispatched) {
          if (FU_busy[rs[i].fu])
            continue;
          if (isLoad(rs[i].inst)) {
            bool abort = false;
            if (rs[i].A == -1) {
              // * store before load
              for (int j = i - 1; j >= 0 && rs[j].fu == dataMem; --j) {
                if (rs[j].busy && isStore(rs[j].inst)) {
                  abort = true;
                  break;
                }
              }
            } else {
              // * no overlap
              abort = !rob.checkLoad(rs[i].rob_index, rs[i].A);
            }
            if (abort) {
              continue;
            }
          }
          if (VERBOSE)
            std::cerr << "Begin execution: " << rs[i].inst_info << "\n";
          // dispatch to FU
          // * Simulate selection
          FU_busy[rs[i].fu] = true;
          tomasuloNew.FU_busy[rs[i].fu] = true;
          if ((!isLoad(rs[i].inst) || rs[i].A != -1)) {
            simulator->execute(rs[i], tomasuloNew.rs[i]);
          }

          tomasuloNew.rs[i].dispatched = true;
        }
      }

      rob.commit(tomasuloNew.rob, simulator);
    }

    ROB rob;
    ROBIndex result_status_x[32];
    ROBIndex result_status_f[32];
    std::vector<ReservationStation> rs;
    bool FU_busy[number_of_component];
  } tomasulo, tomasuloNew;

  void fetch();
  void decode();
  void execute(ReservationStation &rs, ReservationStation &new_rs);
  // void writeBack(ReservationStation &rs);

  int64_t handleSystemCall(int64_t op1, int64_t op2);

  std::string getRegInfoStr();
  void panic(const char *format, ...);

  int decode_hold = 0;
};

} // namespace RISCV

#endif
