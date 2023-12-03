#pragma once

#include "MemoryManager.h"
#include <cstdint>
#include <map>
#include <optional>
#include <vector>

class MemoryManager;

struct Block {
  bool valid;
  bool modified;
  uint32_t addr;
  uint32_t tag;
  uint32_t id;
  uint32_t size;
  uint32_t lastReference;
  int RRPV;
  std::vector<uint8_t> data;
};

class CacheLayer {
public:
  enum ReplacementPolicy { LRU = 0, SRRIP = 1, Optimal = 2 };

  struct Policy {
    // In bytes, must be power of 2
    uint32_t cacheSize;
    uint32_t blockSize;
    uint32_t blockNum;
    uint32_t associativity;
    uint32_t hitLatency;  // in cycles
    uint32_t missLatency; // in cycles
    ReplacementPolicy replacement_policy = LRU;
    uint32_t RRIP_M = 2;
    std::map<uint32_t, uint32_t> *next_ref = NULL;
  };

  struct Statistics {
    uint32_t numRead;
    uint32_t numWrite;
    uint32_t numHit;
    uint32_t numMiss;
    uint64_t totalCycles;
  };

  CacheLayer(MemoryManager *memory, Policy policy);

  bool inCache(uint32_t addr);
  uint32_t getBlockId(uint32_t addr);

  // * read byte, return false on miss
  std::pair<uint8_t, bool> getByte(uint32_t addr, uint32_t *cycles = nullptr);

  // * write byte, return false on miss
  bool setByte(uint32_t addr, uint8_t val, uint32_t *cycles = nullptr);

  // * load a block from memory, return the evicted cacheline if exists
  std::optional<Block> loadBlockFromMemory(uint32_t addr);

  // * fetch a block from this level of cache
  Block fetchBlock(uint32_t addr);

  // * move (fetch & invalidate) a block from this level of cache
  Block moveBlock(uint32_t addr);

  // * install a block to cache, return the evicted cacheline if exists
  std::optional<Block> installBlock(Block block, bool from_victim = false);

  bool updateBlock(Block block);

  // * invalidate and return the block if it exists
  std::optional<Block> invalidateBlock(uint32_t addr);

  void printInfo(bool verbose);
  void printStatistics();

  Statistics statistics;
  Policy policy;

private:
  uint32_t referenceCounter;
  MemoryManager *memory;

  std::vector<Block> blocks;

  void initCache();
  uint32_t getReplacementBlockId(uint32_t begin, uint32_t end);

  // Utility Functions
  bool isPolicyValid();
  bool isPowerOfTwo(uint32_t n);
  uint32_t log2i(uint32_t val);
  uint32_t getTag(uint32_t addr);
  uint32_t getId(uint32_t addr);
  uint32_t getOffset(uint32_t addr);
  uint32_t getAddr(Block &b);
};