#include "CacheLayer.hpp"
#include <cassert>
#include <cstdio>
#include <optional>

CacheLayer::CacheLayer(MemoryManager *memory, Policy policy, bool writeBack,
                       bool writeAllocate) {
  this->referenceCounter = 0;
  this->policy = policy;
  this->memory = memory;
  if (!this->isPolicyValid()) {
    fprintf(stderr, "Policy invalid!\n");
    exit(-1);
  }
  this->initCache();
  this->statistics.numRead = 0;
  this->statistics.numWrite = 0;
  this->statistics.numHit = 0;
  this->statistics.numMiss = 0;
  this->statistics.totalCycles = 0;
  this->writeBack = writeBack;
  this->writeAllocate = writeAllocate;
}

bool CacheLayer::inCache(uint32_t addr) {
  return getBlockId(addr) != -1 ? true : false;
}

std::pair<uint8_t, bool> CacheLayer::getByte(uint32_t addr, uint32_t *cycles) {
  this->referenceCounter++;
  this->statistics.numRead++;

  // Hit
  int blockId;
  if ((blockId = this->getBlockId(addr)) != -1) {
    uint32_t offset = this->getOffset(addr);
    this->statistics.numHit++;
    this->statistics.totalCycles += this->policy.hitLatency;
    this->blocks[blockId].lastReference = this->referenceCounter;
    if (cycles)
      *cycles = this->policy.hitLatency;
    return std::make_pair(this->blocks[blockId].data[offset], true);
  }

  // Miss
  this->statistics.numMiss++;
  if (cycles)
    *cycles = policy.missLatency;
  return std::make_pair(0, false);
}

uint32_t CacheLayer::getBlockId(uint32_t addr) {
  uint32_t tag = this->getTag(addr);
  uint32_t id = this->getId(addr);
  // printf("0x%x 0x%x 0x%x\n", addr, tag, id);
  // iterate over the given set
  for (uint32_t i = id * policy.associativity;
       i < (id + 1) * policy.associativity; ++i) {
    if (this->blocks[i].id != id) {
      fprintf(stderr, "Inconsistent ID in block %d\n", i);
      exit(-1);
    }
    if (this->blocks[i].valid && this->blocks[i].tag == tag) {
      return i;
    }
  }
  return -1;
}

bool CacheLayer::setByte(uint32_t addr, uint8_t val, uint32_t *cycles) {
  this->referenceCounter++;
  this->statistics.numWrite++;

  // Hit
  int blockId;
  if ((blockId = this->getBlockId(addr)) != -1) {
    uint32_t offset = this->getOffset(addr);
    this->statistics.numHit++;
    this->statistics.totalCycles += this->policy.hitLatency;
    this->blocks[blockId].modified = true;
    this->blocks[blockId].lastReference = this->referenceCounter;
    this->blocks[blockId].data[offset] = val;
    if (cycles)
      *cycles = this->policy.hitLatency;
    return true;
  }

  // Miss
  this->statistics.numMiss++;
  return false;
}

Block CacheLayer::fetchBlock(uint32_t addr) {
  auto block_id = this->getBlockId(addr);
  assert(block_id != -1);
  return this->blocks[block_id];
}

Block CacheLayer::moveBlock(uint32_t addr) {
  auto block_id = this->getBlockId(addr);
  assert(block_id != -1);

  auto block = this->blocks[block_id];
  this->blocks[block_id].valid = false;
  return block;
}

std::optional<Block> CacheLayer::loadBlockFromMemory(uint32_t addr) {
  uint32_t blockSize = this->policy.blockSize;
  ++referenceCounter;
  // Initialize new block from memory
  Block b;
  b.valid = true;
  b.modified = false;
  b.tag = this->getTag(addr);
  b.id = this->getId(addr);
  b.size = blockSize;
  b.data = std::vector<uint8_t>(b.size);
  uint32_t bits = this->log2i(blockSize);
  uint32_t mask = ~((1 << bits) - 1);
  uint32_t blockAddrBegin = addr & mask;
  b.addr = blockAddrBegin;
  b.lastReference = referenceCounter;
  for (uint32_t i = blockAddrBegin; i < blockAddrBegin + blockSize; ++i) {
    b.data[i - blockAddrBegin] = this->memory->getByteNoCache(i);
  }
  // std::cerr << "Load block:\n";
  // std::cerr << b.addr << " " << b.id << " " << b.tag << "\n";
  return this->installBlock(b);
}

std::optional<Block> CacheLayer::installBlock(Block block) {
  // std::cerr << "Install block:\n";
  // std::cerr << block.addr << " " << block.id << " " << block.tag << "\n";
  ++referenceCounter;
  auto addr = block.addr;
  auto index = this->getId(addr);
  auto tag = this->getTag(addr);

  block.tag = tag;
  block.id = index;
  block.lastReference = referenceCounter;

  auto set_begin_id = index * this->policy.associativity;
  auto set_end_id = (index + 1) * this->policy.associativity;

  auto evict_id = getReplacementBlockId(set_begin_id, set_end_id);
  std::cerr << "evict:  " << evict_id << "\n";
  std::swap(block, this->blocks[evict_id]);
  std::cerr << "valid:  " << this->blocks[evict_id].valid << "\n";
  // auto ret_block = this->blocks[evict_id];
  // this->blocks[evict_id] = block;
  if (block.valid) {
    return block;
  } else {
    return std::nullopt;
  }
}

bool CacheLayer::updateBlock(Block block) {
  int id = this->getBlockId(block.addr);
  if (id == -1) {
    return false;
  }

  for (int i = 0; i < block.size; ++i) {
    this->blocks[id].data[i] = block.data[i];
  }
  this->blocks[id].modified = true;

  return true;
}

std::optional<Block> CacheLayer::invalidateBlock(uint32_t addr) {
  std::cerr << "Invalidate!\n";
  auto id = this->getBlockId(addr);
  if (id == -1) {
    return std::nullopt;
  }
  auto block = this->blocks[id];
  this->blocks[id].valid = false;
  return block;
}

uint32_t CacheLayer::getReplacementBlockId(uint32_t begin, uint32_t end) {
  // Find invalid block first
  for (uint32_t i = begin; i < end; ++i) {
    if (!this->blocks[i].valid)
      return i;
  }

  // Otherwise use LRU
  uint32_t resultId = begin;
  uint32_t min = this->blocks[begin].lastReference;
  for (uint32_t i = begin; i < end; ++i) {
    if (this->blocks[i].lastReference < min) {
      resultId = i;
      min = this->blocks[i].lastReference;
    }
  }
  return resultId;
}

bool CacheLayer::isPolicyValid() {
  if (!this->isPowerOfTwo(policy.cacheSize)) {
    fprintf(stderr, "Invalid Cache Size %d\n", policy.cacheSize);
    return false;
  }
  if (!this->isPowerOfTwo(policy.blockSize)) {
    fprintf(stderr, "Invalid Block Size %d\n", policy.blockSize);
    return false;
  }
  if (policy.cacheSize % policy.blockSize != 0) {
    fprintf(stderr, "cacheSize %% blockSize != 0\n");
    return false;
  }
  if (policy.blockNum * policy.blockSize != policy.cacheSize) {
    fprintf(stderr, "blockNum * blockSize != cacheSize\n");
    return false;
  }
  if (policy.blockNum % policy.associativity != 0) {
    fprintf(stderr, "blockNum %% associativity != 0\n");
    return false;
  }
  return true;
}

void CacheLayer::printInfo(bool verbose) {
  // printf("---------- Cache Info -----------\n");
  // printf("Cache Size: %d bytes\n", this->policy.cacheSize);
  // printf("Block Size: %d bytes\n", this->policy.blockSize);
  // printf("Block Num: %d\n", this->policy.blockNum);
  // printf("Associativiy: %d\n", this->policy.associativity);
  // printf("Hit Latency: %d\n", this->policy.hitLatency);
  // printf("Miss Latency: %d\n", this->policy.missLatency);

  if (verbose) {
    for (int j = 0; j < this->blocks.size(); ++j) {
      const Block &b = this->blocks[j];
      printf("Block %d: tag 0x%x id %d %s %s (last ref %d)\n", j, b.tag, b.id,
             b.valid ? "valid  " : "invalid",
             b.modified ? "modified  " : "unmodified", b.lastReference);
      // printf("Data: ");
      // for (uint8_t d : b.data)
      // printf("%d ", d);
      // printf("\n");
    }
  }
}

void CacheLayer::printStatistics() {
  printf("-------- STATISTICS ----------\n");
  printf("Num Read: %d\n", this->statistics.numRead);
  printf("Num Write: %d\n", this->statistics.numWrite);
  printf("Num Hit: %d\n", this->statistics.numHit);
  printf("Num Miss: %d\n", this->statistics.numMiss);
  printf("Total Cycles: %llu\n", this->statistics.totalCycles);
  // if (this->lower_ache != nullptr) {
  //   printf("---------- LOWER CACHE ----------\n");
  //   this->lowerCache->printStatistics();
  // }
}

void CacheLayer::initCache() {
  this->blocks = std::vector<Block>(policy.blockNum);
  for (uint32_t i = 0; i < this->blocks.size(); ++i) {
    Block &b = this->blocks[i];
    b.valid = false;
    b.modified = false;
    b.size = policy.blockSize;
    b.tag = 0;
    b.id = i / policy.associativity;
    b.lastReference = 0;
    b.data = std::vector<uint8_t>(b.size);
  }
}

bool CacheLayer::isPowerOfTwo(uint32_t n) {
  return n > 0 && (n & (n - 1)) == 0;
}

uint32_t CacheLayer::log2i(uint32_t val) {
  if (val == 0)
    return uint32_t(-1);
  if (val == 1)
    return 0;
  uint32_t ret = 0;
  while (val > 1) {
    val >>= 1;
    ret++;
  }
  return ret;
}

uint32_t CacheLayer::getTag(uint32_t addr) {
  uint32_t offsetBits = log2i(policy.blockSize);
  uint32_t idBits = log2i(policy.blockNum / policy.associativity);
  uint32_t mask = (1ULL << (32 - offsetBits - idBits)) - 1;
  return (addr >> (offsetBits + idBits)) & mask;
}

uint32_t CacheLayer::getId(uint32_t addr) {
  uint32_t offsetBits = log2i(policy.blockSize);
  uint32_t idBits = log2i(policy.blockNum / policy.associativity);
  uint32_t mask = (1 << idBits) - 1;
  return (addr >> offsetBits) & mask;
}

uint32_t CacheLayer::getOffset(uint32_t addr) {
  uint32_t bits = log2i(policy.blockSize);
  uint32_t mask = (1 << bits) - 1;
  return addr & mask;
}

uint32_t CacheLayer::getAddr(Block &b) {
  uint32_t offsetBits = log2i(policy.blockSize);
  uint32_t idBits = log2i(policy.blockNum / policy.associativity);
  return (b.tag << (offsetBits + idBits)) | (b.id << offsetBits);
}