#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "CacheLayer.hpp"
#include "Debug.h"
#include "L1L2Cache.hpp"
#include "MemoryManager.h"

bool parseParameters(int argc, char **argv);
void printUsage();

const char *traceFilePath = "../cache-trace/01-mcf-gem5-xcg.trace";

int main(int argc, char **argv) {
  // if (!parseParameters(argc, argv)) {
  //   return -1;
  // }

  CacheLayer::Policy l1policy, l2policy;
  l1policy.cacheSize = 32 * 1024;
  l1policy.blockSize = 64;
  l1policy.blockNum = 32 * 1024 / 64;
  l1policy.associativity = 8;
  l1policy.hitLatency = 2;
  l1policy.missLatency = 8;
  l2policy.cacheSize = 256 * 1024;
  l2policy.blockSize = 64;
  l2policy.blockNum = 256 * 1024 / 64;
  l2policy.associativity = 8;
  l2policy.hitLatency = 8;
  l2policy.missLatency = 100;

  // l1policy.cacheSize = 128;
  // l1policy.blockSize = 16;
  // l1policy.blockNum = 8;
  // l1policy.associativity = 1;
  // l1policy.hitLatency = 2;
  // l1policy.missLatency = 8;

  // l2policy.cacheSize = 512;
  // l2policy.blockSize = 16;
  // l2policy.blockNum = 32;
  // l2policy.associativity = 4;
  // l2policy.hitLatency = 8;
  // l2policy.missLatency = 100;

  // Initialize memory and cache
  MemoryManager *memory = nullptr;
  memory = new MemoryManager();
  CacheLayer l1cache(memory, l1policy);
  CacheLayer l2cache(memory, l2policy);
  L1L2Cache l1l2cache(memory, l1cache, l2cache);
  memory->setCache(&l1l2cache);

  // Read and execute trace in cache-trace/ folder
  std::ifstream trace(traceFilePath);
  if (!trace.is_open()) {
    printf("Unable to open file %s\n", traceFilePath);
    exit(-1);
  }

  char type; //'r' for read, 'w' for write
  uint32_t addr;
  int a[1000] = {};
  srand(time(0));
  // int T = 1000000;
  // while (T--) {
  // addr = rand() % 1000;
  // if (rand() & 1) {
  //   type = 'r';
  // } else {
  //   type = 'w';
  // }
  while (trace >> type >> std::hex >> addr) {
    if (!memory->isPageExist(addr))
      memory->addPage(addr);
    std::cerr << "\n";
    int x;
    x = rand() % 100;
    switch (type) {
    case 'r':
      std::cerr << "read   " << addr << "\n";
      x = memory->getByte(addr);
      // assert(x == a[addr]);
      break;
    case 'w':
      std::cerr << "write   " << addr << "\n";
      memory->setByte(addr, x);
      // a[addr] = x;
      break;
    default:
      dbgprintf("Illegal type %c\n", type);
      exit(-1);
    }
    // l1l2cache.print_info();
  }

  std::cerr << "L1  Reference: " << l1l2cache.stat.L1_Reference << "\n";
  std::cerr << "L1  Miss:      " << l1l2cache.stat.L1_Miss << "\n";
  std::cerr << "L2  Reference: " << l1l2cache.stat.L2_Reference << "\n";
  std::cerr << "L2  Miss:      " << l1l2cache.stat.L2_Miss << "\n";
  std::cerr << "Mem Reference: " << l1l2cache.stat.Memory_Reference << "\n";

  // Output Simulation Results
  // printf("L1 Cache:\n");
  // l1cache->printStatistics();

  // delete l1cache;
  // delete l2cache;
  std::cerr << "END\n";
  delete memory;
  return 0;
}

bool parseParameters(int argc, char **argv) {
  // Read Parameters
  if (argc > 1) {
    traceFilePath = argv[1];
    return true;
  } else {
    return false;
  }
}

void printUsage() { printf("Usage: CacheSim trace-file\n"); }