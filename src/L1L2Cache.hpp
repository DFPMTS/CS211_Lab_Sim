#pragma once

#include "CacheLayer.hpp"
#include "MemoryManager.h"
#include <cassert>
#include <map>
#include <optional>

class CacheLayer;
struct Block;
class MemoryManager;

class L1L2Cache;

inline uint32_t get_cacheline(uint32_t addr, uint32_t cacheline_bits) {
  return addr & (~((1 << cacheline_bits) - 1));
}

struct Operation {
  char type;
  uint32_t addr;
};

class CacheWrapper {
public:
  L1L2Cache *getCache(MemoryManager *memory, uint32_t cacheline_size,
                      uint32_t l1_blocks, uint32_t l1_assoc, uint32_t l2_blocks,
                      uint32_t l2_assoc, bool has_victim, uint32_t victim_assoc,
                      int l1_replace, int l2_replace, int l1l2_inclusion,
                      std::vector<Operation> trace = {});

  ~CacheWrapper();

private:
  L1L2Cache *L1L2 = NULL;
  CacheLayer *L1 = NULL;
  CacheLayer *L2 = NULL;
  CacheLayer *Victim = NULL;
};

// enforce same cache line size
class L1L2Cache {
public:
  L1L2Cache(MemoryManager *mem_manager, CacheLayer &l1, CacheLayer &l2,
            CacheLayer *victim = NULL, std::vector<Operation> trace = {});
  ~L1L2Cache();

  enum InclusionPolicy { Inclusive = 0, Exclusive = 1, NINE = 2 };

  uint8_t getByte(uint32_t addr, uint32_t *cycles);

  void setByte(uint32_t addr, uint8_t val, uint32_t *cycles);

  InclusionPolicy inclusion_policy = Exclusive;

  void print_info();

  struct L1L2Statistics {
    int L1_Reference = 0;
    int L1_Miss = 0;
    int Victim_Hit = 0;
    int L2_Reference = 0;
    int L2_Miss = 0;
    int Memory_Reference = 0;
  } stat;

  MemoryManager *memory;
  CacheLayer &L1_cache;
  CacheLayer &L2_cache;
  CacheLayer *victim_cache;

private:
  void L1_process_evicted(Block block);
  void L1_writeback(Block block);
  void L2_backwardInvalidation(std::optional<Block> &block);
  void L2_writeback(Block block);

  uint8_t operation_internal(uint32_t addr, uint32_t *cycles,
                             bool is_write = false, uint8_t val = 0);

  bool has_trace = false;
  std::vector<Operation> trace;
  std::map<uint32_t, uint32_t> next_ref_map;
  std::vector<uint32_t> next_ref;

  FILE *dump_trace_fd;
  uint32_t cacheline_bits;
  int trace_index;
};