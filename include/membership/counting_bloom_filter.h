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
    // Type of counter (default: uint16_t)
    typename C = uint16_t,
    // Hash factory method class (default: MMH3HashFactory)
    template <typename...> class HF = MMH3HashFactory,
    // Element type which will be inserted into counting bloom filter (default: std::string)
    typename T = std::string,
    // Hash output size (default: uint32_t)
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

#define CLASS_METHOD_IMPL(method_name, ...)                \
    template <std::size_t HC, std::size_t MC,              \
              typename C, template <typename...> class HF, \
              typename T, typename S>                      \
    __VA_ARGS__ CountingBloomFilter<HC, MC, C, HF, T, S>::method_name

CLASS_METHOD_IMPL(CountingBloomFilter, )
() : BloomFilter<HC, MC, HF, T, S>(), counters_(MC, 0) {
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        this->counters_[bit] += 1;
        if (this->counters_[bit] == 1) {
            this->bitset_memory_.set(bit);
        }
    });
}

CLASS_METHOD_IMPL(erase, void)
(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        this->counters_[bit] -= 1;
        if (this->counters_[bit] == 0) {
            this->bitset_memory_.set(bit, false);
        }
    });
}

CLASS_METHOD_IMPL(clear, void)
() {
    BloomFilter<HC, MC, HF, T, S>::clear();
    counters_.clear();
    counters_.resize(MC, 0);
}

#endif   // INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
