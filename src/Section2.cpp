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

void evaluate_trace(std::vector<Operation> trace,
                    CacheLayer::ReplacementPolicy l1_replacement_policy,
                    CacheLayer::ReplacementPolicy l2_replacement_policy,
                    L1L2Cache::InclusionPolicy inclusion_policy) {
  CacheWrapper wrapper;

  int cacheline_size = 32;
  int l1_blocks = 16;
  int l2_blocks = 64;
  int l1_assoc = 4;
  int l2_assoc = 8;

  MemoryManager memory;

  auto cache = wrapper.getCache(
      &memory, cacheline_size, l1_blocks, l1_assoc, l2_blocks, l2_assoc, false,
      0, l1_replacement_policy, l2_replacement_policy, inclusion_policy, trace);
  memory.setCache(cache);

  for (auto op : trace) {
    if (!memory.isPageExist(op.addr)) {
      memory.addPage(op.addr);
    }
    switch (op.type) {
    case 'r':
      memory.getByte(op.addr);
      break;
    case 'w':
      memory.setByte(op.addr, 0);
      break;

    default:
      assert(0);
      break;
    }
  }

  auto AMAT = (double)(cache->stat.L1_Reference * 2 + cache->stat.L1_Miss * 8 +
                       cache->stat.L2_Miss * 100) /
              cache->stat.L1_Reference;

  std::cerr << (int)l1_replacement_policy << ", " << (int)inclusion_policy
            << ", " << AMAT << ", " << cache->L1_cache.statistics.totalCycles
            << ", " << cache->L2_cache.statistics.totalCycles << "\n";

  // std::cerr << "L1  Reference:  " << cache->stat.L1_Reference << "\n";
  // std::cerr << "L1  Miss:       " << cache->stat.L1_Miss << "\n";
  // std::cerr << "L2  Reference:  " << cache->stat.L2_Reference << "\n";
  // std::cerr << "L2  Miss:       " << cache->stat.L2_Miss << "\n";
  // std::cerr << "Mem Reference:  " << cache->stat.Memory_Reference << "\n";
  // std::cerr << "L1 total cycles:" << cache->L1_cache.statistics.totalCycles
  //           << "\n";
  // std::cerr << "L2 total cycles:" << cache->L2_cache.statistics.totalCycles
  //           << "\n";
  // std::cerr << "\n";
}

int main() {
  std::vector<Operation> trace;
  {
    std::string trace_path = "../sanity/transpose.trace";
    std::ifstream fin = std::ifstream(trace_path);
    char type;
    uint32_t addr;
    while (fin >> type >> std::hex >> addr) {
      assert(type == 'r' || type == 'w');
      trace.push_back(Operation{type, addr});
    }
  }

  std::cerr << "replacement, inclusion, AMAT, L1cycles, L2cycles\n";

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::SRRIP,
                 CacheLayer::ReplacementPolicy::SRRIP,
                 L1L2Cache::InclusionPolicy::Inclusive);

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::LRU,
                 CacheLayer::ReplacementPolicy::LRU,
                 L1L2Cache::InclusionPolicy::Inclusive);

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::Optimal,
                 CacheLayer::ReplacementPolicy::Optimal,
                 L1L2Cache::InclusionPolicy::Inclusive);
  puts("");

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::SRRIP,
                 CacheLayer::ReplacementPolicy::SRRIP,
                 L1L2Cache::InclusionPolicy::Exclusive);

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::LRU,
                 CacheLayer::ReplacementPolicy::LRU,
                 L1L2Cache::InclusionPolicy::Exclusive);

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::Optimal,
                 CacheLayer::ReplacementPolicy::Optimal,
                 L1L2Cache::InclusionPolicy::Exclusive);
  puts("");
  evaluate_trace(trace, CacheLayer::ReplacementPolicy::SRRIP,
                 CacheLayer::ReplacementPolicy::SRRIP,
                 L1L2Cache::InclusionPolicy::NINE);

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::LRU,
                 CacheLayer::ReplacementPolicy::LRU,
                 L1L2Cache::InclusionPolicy::NINE);

  evaluate_trace(trace, CacheLayer::ReplacementPolicy::Optimal,
                 CacheLayer::ReplacementPolicy::Optimal,
                 L1L2Cache::InclusionPolicy::NINE);
  puts("");
}