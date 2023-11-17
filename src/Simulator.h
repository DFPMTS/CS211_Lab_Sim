/*
 * Main Body for the RISCV Simulator
 *
 * Created by He, Hao at 2019-3-11
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdarg>
#include <cstdint>
#include <string>
#include <vector>

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

  struct FUStatus {
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
      rs1_FU = unknown;

      rs2_type = RegType::INVALID;
      rs2 = -1;
      rs2_ready = false;
      rs2_FU = unknown;

      rs3_type = RegType::INVALID;
      rs3 = -1;
      rs3_ready = false;
      rs3_FU = unknown;
    }

    bool ready() { return rs1_ready && rs2_ready && rs3_ready; }

    bool check_WAR(RegType other_rd_type, RegId other_rd, executeComponent FU) {
      if (!busy || dispatched)
        return true;
      bool pass = true;
      if (same_reg(other_rd_type, other_rd, rs1_type, rs1) && rs1_ready) {
        pass = false;
      }
      if (same_reg(other_rd_type, other_rd, rs2_type, rs2) && rs2_ready) {
        pass = false;
      }
      if (same_reg(other_rd_type, other_rd, rs3_type, rs3) && rs3_ready) {
        pass = false;
      }
      return pass;
    }

    void update_ready(RegType other_rd_type, RegId other_rd) {
      if (!busy || dispatched)
        return;
      if (same_reg(other_rd_type, other_rd, rs1_type, rs1)) {
        rs1_ready = true;
      }
      if (same_reg(other_rd_type, other_rd, rs2_type, rs2)) {
        rs2_ready = true;
      }
      if (same_reg(other_rd_type, other_rd, rs3_type, rs3)) {
        rs3_ready = true;
      }
    }

    bool busy;
    bool dispatched;
    Inst inst;
    uint64_t pc;

    int offset;

    RegType rd_type;
    RegId rd;

    RegType rs1_type;
    RegId rs1;
    int64_t op1;
    float op1_f;
    bool rs1_ready;
    executeComponent rs1_FU;

    RegType rs2_type;
    RegId rs2;
    int64_t op2;
    float op2_f;
    bool rs2_ready;
    executeComponent rs2_FU;

    RegType rs3_type;
    RegId rs3;
    float op3_f;
    bool rs3_ready;
    executeComponent rs3_FU;

    int64_t out;
    float out_f;
    int timeout;
  };

  struct Scoreboard {
    Scoreboard() {
      memset(result_status_x, -1, sizeof(result_status_x));
      memset(result_status_f, -1, sizeof(result_status_f));
      for (int i = 0; i < number_of_component; ++i)
        fu_status[i].clear();
    }

    // * try to issue an instruction
    // return false: 1. FU needed busy 3. result register pending
    bool issue(DReg &d_reg, FReg &f_reg, Scoreboard &scoreboardNew) {
      // check FU status
      executeComponent fu = getComponentUsed(d_reg.inst);
      if (fu_status[fu].busy)
        return false;
      if (d_reg.inst == FMADD_S || d_reg.inst == FMSUB_S) {
        // also need to check fmaAdd
        if (fu_status[fmaAdd].busy)
          return false;
      }
      // check rd
      if (d_reg.rd_reg_type == RegType::INT) {
        if (result_status_x[d_reg.dest] != -1)
          return false;
      } else if (d_reg.rd_reg_type == RegType::FLOAT) {
        if (result_status_f[d_reg.dest] != -1)
          return false;
      }
      // clear to issue
      // ! dirty workaround
      std::swap(fu_status[fu], scoreboardNew.fu_status[fu]);

      fu_status[fu].inst = d_reg.inst;
      fu_status[fu].busy = true;
      fu_status[fu].dispatched = false;
      fu_status[fu].timeout = latency[fu];
      fu_status[fu].offset = d_reg.offset;
      fu_status[fu].pc = d_reg.pc;

      fu_status[fu].op1 = d_reg.op1;
      fu_status[fu].op2 = d_reg.op2;
      fu_status[fu].op1_f = d_reg.op1_f;
      fu_status[fu].op2_f = d_reg.op3_f;
      fu_status[fu].op3_f = d_reg.op3_f;

      fu_status[fu].rd = d_reg.dest;
      fu_status[fu].rd_type = d_reg.rd_reg_type;
      if (d_reg.rd_reg_type == RegType::INT) {
        scoreboardNew.result_status_x[d_reg.dest] = fu;
      } else if (d_reg.rd_reg_type == RegType::FLOAT) {
        scoreboardNew.result_status_f[d_reg.dest] = fu;
      }

      fu_status[fu].rs1 = d_reg.rs1;
      fu_status[fu].rs1_type = d_reg.rs1_reg_type;
      if (d_reg.rs1_reg_type == RegType::INT) {
        fu_status[fu].rs1_ready = (result_status_x[d_reg.rs1] == -1);
        fu_status[fu].rs1_FU = (executeComponent)result_status_x[d_reg.rs1];
      } else if (fu_status[fu].rs1_type == RegType::FLOAT) {
        fu_status[fu].rs1_ready = (result_status_f[d_reg.rs1] == -1);
        fu_status[fu].rs1_FU = (executeComponent)result_status_f[d_reg.rs1];
      } else {
        fu_status[fu].rs1_ready = true;
      }

      fu_status[fu].rs2 = d_reg.rs2;
      fu_status[fu].rs2_type = d_reg.rs2_reg_type;
      if (d_reg.rs2_reg_type == RegType::INT) {
        fu_status[fu].rs2_ready = (result_status_x[d_reg.rs2] == -1);
        fu_status[fu].rs2_FU = (executeComponent)result_status_x[d_reg.rs2];
      } else if (fu_status[fu].rs2_type == RegType::FLOAT) {
        fu_status[fu].rs2_ready = (result_status_f[d_reg.rs2] == -1);
        fu_status[fu].rs2_FU = (executeComponent)result_status_f[d_reg.rs2];
      } else {
        fu_status[fu].rs2_ready = true;
      }

      fu_status[fu].rs3 = d_reg.rs3;
      fu_status[fu].rs3_type = d_reg.rs3_reg_type;
      if (d_reg.rs3_reg_type == RegType::INT) {
        fu_status[fu].rs3_ready = (result_status_x[d_reg.rs3] == -1);
        fu_status[fu].rs3_FU = (executeComponent)result_status_x[d_reg.rs3];
      } else if (fu_status[fu].rs3_type == RegType::FLOAT) {
        fu_status[fu].rs3_ready = (result_status_f[d_reg.rs3] == -1);
        fu_status[fu].rs3_FU = (executeComponent)result_status_f[d_reg.rs3];
      } else {
        fu_status[fu].rs3_ready = true;
      }

      if (d_reg.inst == FMADD_S || d_reg.inst == FMSUB_S) {
        // also lock fmaAdd
        scoreboardNew.fu_status[fmaAdd].busy = true;
        scoreboardNew.fu_status[fmaAdd].timeout = -1;
      }

      /* "Branch instructions hold up the issuing of further instructions until
         they have completed."
         https://www.icsa.inf.ed.ac.uk/research/groups/hase/models/6600/index.html
      */
      // ! TODO stall fetch & decode for branch
      if (isBranch(d_reg.inst) || isJump(d_reg.inst)) {
        f_reg.stall = datamem_stall_lock;
        d_reg.stall = datamem_stall_lock;
      }

      // ! dirty workaround
      std::swap(fu_status[fu], scoreboardNew.fu_status[fu]);

      return true;
    }

    // 1. execute instructions when source operands are ready
    // 2. retire instructions when there's no WAR hazard
    void tick(Simulator *simulator, Scoreboard &scoreboardNew) {
      // printf("memCalc FU %d   %d   %d   %d   %d\n", fu_status[0].inst,
      //        fu_status[0].busy, fu_status[0].ready(), fu_status[0].rs1_ready,
      //        fu_status[0].rs2_ready);
      for (int i = 0; i < number_of_component; ++i) {
        if (fu_status[i].busy && fu_status[i].ready()) {
          if (!fu_status[i].dispatched) {
            // dispatch to FU
            simulator->execute(fu_status[i], scoreboardNew.fu_status[i]);
            scoreboardNew.fu_status[i].dispatched = true;
          } else {
            // printf("FU:  %d dispatched, timeout: %d\n", i,
            //        fu_status[i].timeout);
            // check if complete
            // ! note that for fma the fmaAdd timeout was set to -1 and need to
            // ! be unlocked by fmaMul
            if (fu_status[i].timeout == 0) {
              // ready to retire, check WAR
              bool WAR_pass = true;
              for (int j = 0; j < number_of_component; ++j) {
                if (j != i) {
                  if (!fu_status[j].check_WAR(fu_status[i].rd_type,
                                              fu_status[i].rd,
                                              (executeComponent)i)) {
                    WAR_pass = false;
                    break;
                  }
                }
              }
              // printf("FU:  %d completed, WAR_pass: %d\n", i, WAR_pass);
              if (WAR_pass) {
                // writeback
                simulator->writeBack(fu_status[i]);
                scoreboardNew.fu_status[i].busy = false;
                if (i == fmaMul && fu_status[fmaAdd].timeout < 0) {
                  scoreboardNew.fu_status[fmaAdd].busy = false;
                }
                if (fu_status[i].rd_type == RegType::INT) {
                  scoreboardNew.result_status_x[fu_status[i].rd] = -1;
                  // printf("cleared INT  %d\n", fu_status[i].rd);
                } else if (fu_status[i].rd_type == RegType::FLOAT) {
                  scoreboardNew.result_status_f[fu_status[i].rd] = -1;
                  // printf("cleared FP  %d\n", fu_status[i].rd);
                }
                for (int j = 0; j < number_of_component; ++j) {
                  if (j != i) {
                    scoreboardNew.fu_status[j].update_ready(
                        fu_status[i].rd_type, fu_status[i].rd);
                  }
                }
              }
            } else {
              // printf("Timeout:    %d\n", fu_status[i].timeout);
              scoreboardNew.fu_status[i].timeout = fu_status[i].timeout - 1;
            }
          }
        }
      }
    }

    FUStatus fu_status[number_of_component];
    int result_status_x[32];
    int result_status_f[32];
  } scoreboard, scoreboardNew;

  void fetch();
  void decode();
  void execute(FUStatus &fu_status, FUStatus &new_fu_status);
  void writeBack(FUStatus &fu_status);

  int64_t handleSystemCall(int64_t op1, int64_t op2);

  std::string getRegInfoStr();
  void panic(const char *format, ...);

  int decode_hold = 0;
};

} // namespace RISCV

#endif
