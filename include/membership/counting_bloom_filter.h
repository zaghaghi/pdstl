#ifndef INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
#define INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_

#include <string>
#include <vector>

#include "bloom_filter.h"

/*
 * Counting Bloom Filter
 *
 * CountingBloomFilter class implements counting bloom filter algorithm for solving membership problem.
 */
template <
    // Number of hash functions
    std::size_t HC,
    // Number of memory bits
    std::size_t MC,
    // Type of counter
    typename C = uint16_t,
    // Hash factory method class
    template <typename...> class HF = MMH3HashFactory,
    // Element type which will be inserted into counting bloom filter
    typename T = std::string,
    // Hash output size
    typename S = uint32_t>
class CountingBloomFilter : public BloomFilter<HC, MC, HF, T, S> {
   protected:
    std::vector<C> counters_;
    using BloomFilter<HC, MC, HF, T, S>::hashes_;
    using BloomFilter<HC, MC, HF, T, S>::bitset_memory_;

   public:
    // Default constructor
    CountingBloomFilter();

    /* insert
     * @item the item to insert into the bloom filter.
     * 
     * Insert item into counting bloom filter.
     */
    void insert(const T& item) override;

    /* erase
     * @item the item to erase from filter.
     * 
     * Erase item from counting bloom filter.
     */
    void erase(const T& item) override;

    /* clear
     * 
     * Clear filter and resets its internal memory.
     */
    void clear() override;
};

template <std::size_t HC, std::size_t MC, typename C, template <typename...> class HF, typename T, typename S>
CountingBloomFilter<HC, MC, C, HF, T, S>::CountingBloomFilter() : BloomFilter<HC, MC, HF, T, S>(), counters_(MC, 0) {
}

template <std::size_t HC, std::size_t MC, typename C, template <typename...> class HF, typename T, typename S>
void CountingBloomFilter<HC, MC, C, HF, T, S>::insert(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        this->counters_[bit] += 1;
        if (this->counters_[bit] == 1) {
            this->bitset_memory_.set(bit);
        }
    });
}

template <std::size_t HC, std::size_t MC, typename C, template <typename...> class HF, typename T, typename S>
void CountingBloomFilter<HC, MC, C, HF, T, S>::erase(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        this->counters_[bit] -= 1;
        if (this->counters_[bit] == 0) {
            this->bitset_memory_.set(bit, false);
        }
    });
}

template <std::size_t HC, std::size_t MC, typename C, template <typename...> class HF, typename T, typename S>
void CountingBloomFilter<HC, MC, C, HF, T, S>::clear() {
    BloomFilter<HC, MC, HF, T, S>::clear();
    counters_.clear();
    counters_.resize(MC, 0);
}

#endif   // INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
