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

void test_all() {

  for (int replace1 = 0; replace1 < 3; ++replace1) {
    for (int replace2 = 0; replace2 < 3; ++replace2) {
      for (int victim = 0; victim < 2; ++victim) {
        for (int inclusion = 0; inclusion < 3; ++inclusion) {
          //   int replace = 1, victim = 1, inclusion = 0;
          std::cerr << replace1 << "   " << replace2 << "   " << victim << "   "
                    << inclusion << "\n";
          // srand(time(0));
          std::vector<Operation> trace;
          std::map<uint32_t, uint32_t> next_ref_map;
          std::vector<uint32_t> next_ref;

          uint32_t cacheline_bits = 4;

          CacheLayer::Policy l1policy, l2policy, victim_policy;

          l1policy.blockNum = 4;
          l1policy.associativity = 1;
          l1policy.blockSize = 1 << cacheline_bits;
          l1policy.cacheSize = l1policy.blockNum * l1policy.blockSize;
          l1policy.hitLatency = 2;
          l1policy.missLatency = 8;
          l1policy.next_ref = &next_ref_map;
          l1policy.replacement_policy = (CacheLayer::ReplacementPolicy)replace1;

          l2policy.blockNum = 16;
          l2policy.associativity = 4;
          l2policy.blockSize = 1 << cacheline_bits;
          l2policy.cacheSize = l2policy.blockNum * l2policy.blockSize;
          l2policy.hitLatency = 8;
          l2policy.missLatency = 100;
          l2policy.next_ref = &next_ref_map;
          l2policy.replacement_policy = (CacheLayer::ReplacementPolicy)replace2;

          victim_policy.blockNum = 4;
          victim_policy.blockSize = 1 << cacheline_bits;
          victim_policy.cacheSize =
              victim_policy.blockNum * victim_policy.blockSize;
          victim_policy.associativity = victim_policy.blockNum;
          victim_policy.hitLatency = 2;
          victim_policy.hitLatency = 8;
          victim_policy.replacement_policy = CacheLayer::ReplacementPolicy::LRU;

          // Initialize memory and cache
          MemoryManager *memory = nullptr;
          memory = new MemoryManager();
          CacheLayer l1cache(memory, l1policy);
          CacheLayer l2cache(memory, l2policy);
          CacheLayer victim_cache(memory, victim_policy);

          L1L2Cache *l1l2cache;
          if (victim) {
            l1l2cache = new L1L2Cache(memory, l1cache, l2cache, &victim_cache);
          } else {
            l1l2cache = new L1L2Cache(memory, l1cache, l2cache);
          }

          l1l2cache->inclusion_policy = (L1L2Cache::InclusionPolicy)inclusion;

          memory->setCache(l1l2cache);

          int a[1000] = {};
          // srand(time(0));
          int T = 1000000;
          while (T--) {
            char type; //'r' for read, 'w' for write
            uint32_t addr;
            addr = rand() % 1000;
            if (rand() & 1) {
              type = 'r';
            } else {
              type = 'w';
            }
            trace.push_back(Operation{type, addr});
          }

          next_ref.clear();
          next_ref.resize(trace.size());
          int uniq = 0;
          for (int i = trace.size() - 1; i >= 0; --i) {
            int cacheline_addr = get_cacheline(trace[i].addr, cacheline_bits);
            if (!next_ref_map.count(cacheline_addr)) {
              next_ref[i] = trace.size();
              uniq++;
            } else {
              next_ref[i] = next_ref_map[cacheline_addr];
            }
            next_ref_map[cacheline_addr] = i;
          }

          for (int i = 0; i < trace.size(); ++i) {
            char type = trace[i].type;
            uint32_t addr = trace[i].addr;

            if (!memory->isPageExist(addr))
              memory->addPage(addr);
            int x;
            switch (type) {
            case 'r':
              // std::cerr << "read   " << addr << "\n";
              x = memory->getByte(addr);
              // std::cerr << ">>  " << x << "  " << a[addr] << "\n";
              assert(x == a[addr]);
              break;
            case 'w':
              x = rand() % 128;
              // std::cerr << "write   " << addr << "  " << x << "\n";
              memory->setByte(addr, x);
              a[addr] = x;
              break;
            default:
              dbgprintf("Illegal type %c\n", type);
              exit(-1);
            }
            next_ref_map[get_cacheline(addr, cacheline_bits)] = next_ref[i];
            // l1l2cache->print_info();
          }

          std::cerr << "L1  Reference: " << l1l2cache->stat.L1_Reference
                    << "\n";
          std::cerr << "L1  Miss:      " << l1l2cache->stat.L1_Miss << "\n";
          std::cerr << "L2  Reference: " << l1l2cache->stat.L2_Reference
                    << "\n";
          std::cerr << "L2  Miss:      " << l1l2cache->stat.L2_Miss << "\n";
          std::cerr << "Mem Reference: " << l1l2cache->stat.Memory_Reference
                    << "\n";

          delete memory;
        }
      }
    }
  }
}

void test_all_v2() {

  for (int replace1 = 0; replace1 < 3; ++replace1) {
    for (int replace2 = 0; replace2 < 3; ++replace2) {
      for (int victim = 0; victim < 2; ++victim) {
        for (int inclusion = 0; inclusion < 3; ++inclusion) {
          //   int replace = 1, victim = 1, inclusion = 0;
          std::cerr << replace1 << "   " << replace2 << "   " << victim << "   "
                    << inclusion << "\n";

          std::vector<Operation> trace;

          // Initialize memory and cache
          MemoryManager *memory = nullptr;
          memory = new MemoryManager();

          CacheWrapper wrapper;

          uint32_t cacheline_size = 16;
          int l1_blocks = 4;
          int l2_blocks = 16;
          int l1_assoc = 1;
          int l2_assoc = 4;
          int victim_assoc = 4;

          int a[1000] = {};
          // srand(time(0));
          int T = 1000000;
          while (T--) {
            char type; //'r' for read, 'w' for write
            uint32_t addr;
            addr = rand() % 1000;
            if (rand() & 1) {
              type = 'r';
            } else {
              type = 'w';
            }
            trace.push_back(Operation{type, addr});
          }

          auto l1l2cache = wrapper.getCache(
              memory, cacheline_size, l1_blocks, l1_assoc, l2_blocks, l2_assoc,
              victim, victim_assoc, replace1, replace2, inclusion, trace);
          memory->setCache(l1l2cache);

          for (int i = 0; i < trace.size(); ++i) {
            char type = trace[i].type;
            uint32_t addr = trace[i].addr;

            if (!memory->isPageExist(addr))
              memory->addPage(addr);
            int x;
            switch (type) {
            case 'r':
              // std::cerr << "read   " << addr << "\n";
              x = memory->getByte(addr);
              // std::cerr << ">>  " << x << "  " << a[addr] << "\n";
              assert(x == a[addr]);
              break;
            case 'w':
              x = rand() % 128;
              // std::cerr << "write   " << addr << "  " << x << "\n";
              memory->setByte(addr, x);
              a[addr] = x;
              break;
            default:
              dbgprintf("Illegal type %c\n", type);
              exit(-1);
            }

            // l1l2cache->print_info();
          }

          std::cerr << "L1  Reference: " << l1l2cache->stat.L1_Reference
                    << "\n";
          std::cerr << "L1  Miss:      " << l1l2cache->stat.L1_Miss << "\n";
          std::cerr << "L2  Reference: " << l1l2cache->stat.L2_Reference
                    << "\n";
          std::cerr << "L2  Miss:      " << l1l2cache->stat.L2_Miss << "\n";
          std::cerr << "Mem Reference: " << l1l2cache->stat.Memory_Reference
                    << "\n";

          delete memory;
        }
      }
    }
  }
}

int main(int argc, char **argv) {

  test_all_v2();

  return 0;
}