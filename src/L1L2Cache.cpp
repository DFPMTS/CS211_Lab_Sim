#include "L1L2Cache.hpp"

L1L2Cache *CacheWrapper::getCache(MemoryManager *memory,
                                  uint32_t cacheline_size, uint32_t l1_blocks,
                                  uint32_t l1_assoc, uint32_t l2_blocks,
                                  uint32_t l2_assoc, bool has_victim,
                                  uint32_t victim_assoc, int l1_replace,
                                  int l2_replace, int l1l2_inclusion,
                                  std::vector<Operation> trace) {

  if (l1_replace == CacheLayer::ReplacementPolicy::Optimal ||
      l2_replace == CacheLayer::ReplacementPolicy::Optimal) {

  } else {
    trace = {};
  }

  CacheLayer::Policy l1policy, l2policy, victim_policy;

  l1policy.blockNum = l1_blocks;
  l1policy.associativity = l1_assoc;
  l1policy.blockSize = cacheline_size;
  l1policy.cacheSize = l1policy.blockNum * l1policy.blockSize;
  l1policy.hitLatency = 2;
  l1policy.missLatency = 8;
  // l1policy.next_ref = &next_ref_map;
  l1policy.replacement_policy = (CacheLayer::ReplacementPolicy)l1_replace;

  l2policy.blockNum = l2_blocks;
  l2policy.associativity = l2_assoc;
  l2policy.blockSize = cacheline_size;
  l2policy.cacheSize = l2policy.blockNum * l2policy.blockSize;
  l2policy.hitLatency = 8;
  l2policy.missLatency = 100;
  // l2policy.next_ref = &next_ref_map;
  l2policy.replacement_policy = (CacheLayer::ReplacementPolicy)l2_replace;

  L1 = new CacheLayer(memory, l1policy);
  L2 = new CacheLayer(memory, l2policy);

  if (has_victim) {
    victim_policy.blockNum = victim_assoc;
    victim_policy.blockSize = cacheline_size;
    victim_policy.cacheSize = victim_policy.blockNum * victim_policy.blockSize;
    victim_policy.associativity = victim_policy.blockNum;
    victim_policy.hitLatency = 1;
    victim_policy.missLatency = 8;
    victim_policy.replacement_policy = CacheLayer::ReplacementPolicy::LRU;

    Victim = new CacheLayer(memory, victim_policy);
  }

  if (has_victim) {
    L1L2 = new L1L2Cache(memory, *L1, *L2, Victim, trace);
  } else {
    L1L2 = new L1L2Cache(memory, *L1, *L2, NULL, trace);
  }
  L1L2->inclusion_policy = (L1L2Cache::InclusionPolicy)l1l2_inclusion;

  return L1L2;
}

CacheWrapper::~CacheWrapper() {
  delete L1;
  delete L2;
  delete Victim;
  delete L1L2;
}

L1L2Cache::L1L2Cache(MemoryManager *mem_manager, CacheLayer &l1, CacheLayer &l2,
                     CacheLayer *victim, std::vector<Operation> provided_trace)
    : memory(mem_manager), L1_cache(l1), L2_cache(l2), victim_cache(victim),
      trace(provided_trace) {

  if (trace.size()) {
    // * check if replacement policy are Optimal
    assert(l1.policy.replacement_policy ==
               CacheLayer::ReplacementPolicy::Optimal ||
           l2.policy.replacement_policy ==
               CacheLayer::ReplacementPolicy::Optimal);

    has_trace = true;

    next_ref_map.clear();
    next_ref.clear();
    next_ref.resize(trace.size());

    trace_index = 0;
    cacheline_bits = 0;
    uint32_t cacheline_size = l1.policy.blockSize;
    assert(cacheline_size);
    while (cacheline_size > 1) {
      cacheline_size >>= 1;
      cacheline_bits++;
    }
    for (int i = trace.size() - 1; i >= 0; --i) {
      auto cacheline_addr = get_cacheline(trace[i].addr, cacheline_bits);
      if (!next_ref_map.count(cacheline_addr)) {
        next_ref[i] = trace.size();
      } else {
        next_ref[i] = next_ref_map[cacheline_addr];
      }
      next_ref_map[cacheline_addr] = i;
    }

    L1_cache.policy.next_ref = &this->next_ref_map;
    L2_cache.policy.next_ref = &this->next_ref_map;
  }
  dump_trace_fd = fopen("dump.trace", "w");
}

L1L2Cache::~L1L2Cache() { fclose(dump_trace_fd); }

uint8_t L1L2Cache::operation_internal(uint32_t addr, uint32_t *cycles,
                                      bool write, uint8_t val) {
  // * L1
  stat.L1_Reference++;
  // std::cerr << "L1\n";

  auto [L1_val, L1_hit] = L1_cache.getByte(addr, cycles);
  if (L1_hit) {
    if (write) {
      L1_cache.setByte(addr, val, cycles);
    }
    return L1_val;
  }

  // * victim
  // std::cerr << "Victim\n";
  if (victim_cache) {
    auto [victim_val, victim_hit] = victim_cache->getByte(addr, cycles);
    if (victim_hit) {
      stat.Victim_Hit++;
      auto victim_moved_block = victim_cache->moveBlock(addr);
      auto L1_evicted_block = L1_cache.installBlock(victim_moved_block, true);
      if (write) {
        L1_cache.setByte(addr, val, cycles);
      }
      if (L1_evicted_block.has_value()) {
        L1_process_evicted(*L1_evicted_block);
      }

      if (inclusion_policy == InclusionPolicy::Inclusive) {
        // * ensure cache line present in L2
        if (!L2_cache.inCache(addr)) {
          auto L2_evicted_block = L2_cache.installBlock(victim_moved_block);
          if (L2_evicted_block.has_value()) {
            if (inclusion_policy == Inclusive) {
              L2_backwardInvalidation(L2_evicted_block);
            }
            L2_writeback(*L2_evicted_block);
          }
        }
      }

      return victim_val;
    }
  }

  // * L2
  stat.L1_Miss++;
  stat.L2_Reference++;
  // std::cerr << "L2\n";

  auto [L2_val, L2_hit] = L2_cache.getByte(addr, cycles);
  if (L2_hit) {
    if (inclusion_policy == NINE || inclusion_policy == Inclusive) {
      // * install cacheline fetched from L2 to L1
      auto L2_fetched_block = L2_cache.fetchBlock(addr);
      auto L1_evicted_block = L1_cache.installBlock(L2_fetched_block);
      if (write) {
        L1_cache.setByte(addr, val, cycles);
      }
      if (L1_evicted_block.has_value()) {
        L1_process_evicted(*L1_evicted_block);
      }

      return L2_val;
    } else if (inclusion_policy == Exclusive) {
      // * move the cacheline from L2 to L1
      auto L2_moved_block = L2_cache.moveBlock(addr);
      auto L1_evicted_block = L1_cache.installBlock(L2_moved_block);
      if (write) {
        L1_cache.setByte(addr, val, cycles);
      }
      if (L1_evicted_block.has_value()) {
        L1_process_evicted(*L1_evicted_block);
      }
      return L2_val;
    }
  }

  // * Memory
  stat.L2_Miss++;
  stat.Memory_Reference++;

  if (inclusion_policy == NINE || inclusion_policy == Inclusive) {
    // * NINE / Inclusive
    // * Memory -> L2
    // std::cerr << "Load From Memory\n";
    auto L2_evicted_block = L2_cache.loadBlockFromMemory(addr);
    // * write back to memory
    if (L2_evicted_block.has_value()) {
      if (inclusion_policy == Inclusive) {
        // * Backward invalidation
        if (L2_evicted_block.has_value()) {
          L2_backwardInvalidation(L2_evicted_block);
        }
      }
      L2_writeback(*L2_evicted_block);
    }

    // * L2 -> L1
    // std::cerr << "L2 to L1\n";
    auto L2_fetched_block = L2_cache.fetchBlock(addr);
    auto L1_evicted_block = L1_cache.installBlock(L2_fetched_block);
    if (write) {
      L1_cache.setByte(addr, val, cycles);
    }
    if (L1_evicted_block.has_value()) {
      L1_process_evicted(*L1_evicted_block);
    }
  } else if (inclusion_policy == Exclusive) {
    // * Exclusive
    // * To L1 only
    // std::cerr << "Mem to L1";
    auto L1_evicted_block = L1_cache.loadBlockFromMemory(addr);
    if (write) {
      L1_cache.setByte(addr, val, cycles);
    }
    if (L1_evicted_block.has_value()) {
      L1_process_evicted(*L1_evicted_block);
    }
  }

  // std::cerr << "Memory\n";
  return memory->getByteNoCache(addr);
}

uint8_t L1L2Cache::getByte(uint32_t addr, uint32_t *cycles) {
  // std::cerr << trace.size() << "\n";
  if (has_trace) {
    assert(trace[trace_index].type == 'r' && trace[trace_index].addr == addr);
  }

  fprintf(dump_trace_fd, "r   %x\n", addr);
  auto val = this->operation_internal(addr, cycles);

  if (has_trace) {
    next_ref_map[get_cacheline(addr, cacheline_bits)] = next_ref[trace_index];
    trace_index++;
  }
  return val;
}

void L1L2Cache::setByte(uint32_t addr, uint8_t val, uint32_t *cycles) {
  // std::cerr << trace.size() << "\n";
  if (has_trace) {
    assert(trace[trace_index].type == 'w' && trace[trace_index].addr == addr);
  }

  fprintf(dump_trace_fd, "w   %x\n", addr);
  this->operation_internal(addr, cycles, true, val);

  if (has_trace) {
    next_ref_map[get_cacheline(addr, cacheline_bits)] = next_ref[trace_index];
    trace_index++;
  }
}

void L1L2Cache::L1_process_evicted(Block block) {
  if (victim_cache) {
    if (inclusion_policy == Exclusive) {
      // * write-back victim cache, see report 3.2.3
      auto victim_evicted_block = victim_cache->installBlock(block);
      if (victim_evicted_block.has_value()) {
        L1_writeback(*victim_evicted_block);
      }
    } else {
      // * write-through victim cache
      auto victim_evicted_block = victim_cache->installBlock(block);
      // * just discard victim_evicted_block
      L1_writeback(block);
    }
  } else {
    L1_writeback(block);
  }
}

void L1L2Cache::L1_writeback(Block block) {
  assert(block.valid);

  if (inclusion_policy == NINE || inclusion_policy == Inclusive) {
    if (!block.modified)
      return;
    // * Inclusive / NINE
    bool in_L2 = L2_cache.updateBlock(block);
    if (!in_L2) {
      auto L2_evicted_block = L2_cache.installBlock(block);
      if (L2_evicted_block.has_value()) {
        if (inclusion_policy == Inclusive) {
          L2_backwardInvalidation(L2_evicted_block);
        }
        L2_writeback(*L2_evicted_block);
      }
    }
  } else if (inclusion_policy == Exclusive) {
    // * Exclusive
    auto L2_evicted_block = L2_cache.installBlock(block);
    if (L2_evicted_block.has_value()) {
      L2_writeback(*L2_evicted_block);
    }
  }
}

void L1L2Cache::L2_backwardInvalidation(
    std::optional<Block> &L2_evicted_block) {
  auto L1_evicted_block = L1_cache.invalidateBlock(L2_evicted_block->addr);
  if (L1_evicted_block.has_value()) {
    L2_evicted_block = L1_evicted_block;
  }
}

void L1L2Cache::L2_writeback(Block block) {
  assert(block.valid);
  if (!block.modified)
    return;

  for (int i = 0; i < block.size; ++i) {
    memory->setByteNoCache(block.addr + i, block.data[i]);
  }
}

void L1L2Cache::print_info() {
  printf("--------------------------L1--------------------------\n");
  L1_cache.printInfo(true);
  if (victim_cache) {
    printf("------------------------Victim------------------------\n");
    victim_cache->printInfo(true);
  }
  printf("--------------------------L2--------------------------\n");
  L2_cache.printInfo(true);
  printf("------------------------------------------------------\n");
  puts("");
}