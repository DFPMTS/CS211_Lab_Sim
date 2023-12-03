#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "CacheLayer.hpp"
#include "Debug.h"
#include "L1L2Cache.hpp"
#include "MemoryManager.h"

void inclusive() {

  CacheWrapper wrapper;

  int cacheline_size = 1;
  int l1_blocks = 2;
  int l2_blocks = 2;
  int l1_assoc = 2;
  int l2_assoc = 2;

  auto l1_replacement_policy = CacheLayer::ReplacementPolicy::LRU;
  auto l2_replacement_policy = CacheLayer::ReplacementPolicy::LRU;
  auto inclusion_policy = L1L2Cache::InclusionPolicy::Inclusive;

  MemoryManager memory;

  auto cache = wrapper.getCache(
      &memory, cacheline_size, l1_blocks, l1_assoc, l2_blocks, l2_assoc, false,
      0, l1_replacement_policy, l2_replacement_policy, inclusion_policy);
  memory.setCache(cache);

  memory.addPage(0);

  std::cerr << "Initial State\n";
  cache->print_info();
  std::cerr << "Read  " << 0 << "\n";
  memory.getByte(0);
  cache->print_info();
  std::cerr << "Read  " << 1 << "\n";
  memory.getByte(1);
  cache->print_info();
  std::cerr << "Read  " << 0 << "\n";
  memory.getByte(0);
  cache->print_info();
  std::cerr << "Read  " << 2 << "\n";
  memory.getByte(2);
  cache->print_info();
}

void exclusive() {

  CacheWrapper wrapper;

  int cacheline_size = 1;
  int l1_blocks = 2;
  int l2_blocks = 4;
  int l1_assoc = 2;
  int l2_assoc = 4;

  auto l1_replacement_policy = CacheLayer::ReplacementPolicy::LRU;
  auto l2_replacement_policy = CacheLayer::ReplacementPolicy::LRU;
  auto inclusion_policy = L1L2Cache::InclusionPolicy::Exclusive;

  MemoryManager memory;

  auto cache = wrapper.getCache(
      &memory, cacheline_size, l1_blocks, l1_assoc, l2_blocks, l2_assoc, false,
      0, l1_replacement_policy, l2_replacement_policy, inclusion_policy);
  memory.setCache(cache);

  memory.addPage(0);
  std::cerr << "Initial State\n";
  cache->print_info();
  std::cerr << "Read  " << 0 << "\n";
  memory.getByte(0);
  cache->print_info();
  std::cerr << "Read  " << 1 << "\n";
  memory.getByte(1);
  cache->print_info();
  std::cerr << "Read  " << 2 << "\n";
  memory.getByte(2);
  cache->print_info();
  std::cerr << "Read  " << 3 << "\n";
  memory.getByte(3);
  cache->print_info();
  std::cerr << "Read  " << 4 << "\n";
  memory.getByte(4);
  cache->print_info();
  std::cerr << "Read  " << 5 << "\n";
  memory.getByte(5);
  cache->print_info();
}
int main() { inclusive(); }