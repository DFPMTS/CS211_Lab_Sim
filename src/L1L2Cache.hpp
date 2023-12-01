#pragma once

#include "CacheLayer.hpp"
#include "MemoryManager.h"
#include <cassert>
#include <optional>

class CacheLayer;
struct Block;
class MemoryManager;

// enforce same cache line size
class L1L2Cache {
public:
  L1L2Cache(MemoryManager *mem_manager, CacheLayer &l1, CacheLayer &l2,
            CacheLayer *victim = NULL)
      : memory(mem_manager), L1_cache(l1), L2_cache(l2), victim_cache(victim) {}

  enum InclusionPolicy { Inclusive = 0, Exclusive = 1, NINE = 2 };

  uint8_t getByte(uint32_t addr, uint32_t *cycles);

  void setByte(uint32_t addr, uint8_t val, uint32_t *cycles);

  InclusionPolicy inclusion_policy = NINE;

  void print_info();

  struct L1L2Statistics {
    int L1_Reference = 0;
    int L1_Miss = 0;
    int L2_Reference = 0;
    int L2_Miss = 0;
    int Memory_Reference = 0;
  } stat;

private:
  void L1_process_evicted(Block block);
  void L1_writeback(Block block);
  void L2_backwardInvalidation(std::optional<Block> &block);
  void L2_writeback(Block block);

  MemoryManager *memory;
  CacheLayer &L1_cache;
  CacheLayer &L2_cache;
  CacheLayer *victim_cache;
};