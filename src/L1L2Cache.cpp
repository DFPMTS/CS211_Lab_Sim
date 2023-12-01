#include "L1L2Cache.hpp"

uint8_t L1L2Cache::getByte(uint32_t addr, uint32_t *cycles) {
  // * L1
  stat.L1_Reference++;
  std::cerr << "L1\n";

  auto [L1_val, L1_hit] = L1_cache.getByte(addr, cycles);
  if (L1_hit) {
    return L1_val;
  }

  // * victim
  if (victim_cache) {
    auto [victim_val, victim_hit] = victim_cache->getByte(addr, cycles);
    if (victim_hit) {
      auto victim_moved_block = victim_cache->moveBlock(addr);
      auto L1_evicted_block = L1_cache.installBlock(victim_moved_block);
      if (L1_evicted_block.has_value()) {
        L1_process_evicted(*L1_evicted_block);
      }

      return victim_val;
    }
  }

  // * L2
  stat.L1_Miss++;
  stat.L2_Reference++;
  std::cerr << "L2\n";

  auto [L2_val, L2_hit] = L2_cache.getByte(addr, cycles);
  if (L2_hit) {
    if (inclusion_policy == NINE || inclusion_policy == Inclusive) {
      // * install cacheline fetched from L2 to L1
      auto L2_fetched_block = L2_cache.fetchBlock(addr);
      auto L1_evicted_block = L1_cache.installBlock(L2_fetched_block);
      if (L1_evicted_block.has_value()) {
        L1_process_evicted(*L1_evicted_block);
      }

      return L2_val;
    } else if (inclusion_policy == Exclusive) {
      // * move the cacheline from L2 to L1
      auto L2_moved_block = L2_cache.moveBlock(addr);
      auto L1_evicted_block = L1_cache.installBlock(L2_moved_block);
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
    std::cerr << "Load From Memory\n";
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
    std::cerr << "L2 to L1\n";
    auto L2_fetched_block = L2_cache.fetchBlock(addr);
    auto L1_evicted_block = L1_cache.installBlock(L2_fetched_block);
    if (L1_evicted_block.has_value()) {
      L1_process_evicted(*L1_evicted_block);
    }
  } else if (inclusion_policy == Exclusive) {
    // * Exclusive
    // * To L1 only
    std::cerr << "L1-";
    auto L1_evicted_block = L1_cache.loadBlockFromMemory(addr);
    if (L1_evicted_block.has_value()) {
      L1_process_evicted(*L1_evicted_block);
    }
  }

  std::cerr << "Memory\n";
  return memory->getByteNoCache(addr);
}

void L1L2Cache::setByte(uint32_t addr, uint8_t val, uint32_t *cycles) {
  this->getByte(addr, cycles);
  assert(L1_cache.setByte(addr, val, NULL));
}

void L1L2Cache::L1_process_evicted(Block block) {
  if (victim_cache) {
    // * evict into victim cache
    auto victim_evicted_block = victim_cache->installBlock(block);
    if (victim_evicted_block.has_value()) {
      block = *victim_evicted_block;
    }
  }
  L1_writeback(block);
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
  printf("--------------------------L2--------------------------\n");
  L2_cache.printInfo(true);
}