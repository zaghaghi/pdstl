#ifndef INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
#define INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_

#include <string>
#include <vector>

#include "bloom_filter.h"

/*! \brief Counting Bloom Filter
 *
 * CountingBloomFilter class implements counting filter algorithm for solving membership problem.
 * 
 * \tparam HC - Number of hash functions
 * \tparam MC - Number of memory bits
 * \tparam C - Type of counter (default: uint16_t)
 * \tparam HF - Hash factory method class (default: MMH3HashFactory)
 * \tparam T - Element type which will be inserted into counting bloom filter (default: std::string)
 * \tparam S - Hash output size (default: uint32_t)
 */
template <
    std::size_t HC,
    std::size_t MC,
    typename C = uint16_t,
    template <typename...> class HF = MMH3HashFactory,
    typename T = std::string,
    typename S = uint32_t>
class CountingBloomFilter : public BloomFilter<HC, MC, HF, T, S> {
   protected:
    std::vector<C> counters_;
    using BloomFilter<HC, MC, HF, T, S>::hashes_;
    using BloomFilter<HC, MC, HF, T, S>::bitset_memory_;

   public:
    //! Default constructor
    CountingBloomFilter();

    /*! \brief Insert item into counting bloom filter.
     * \param item - the item to insert into the bloom filter.
     * 
     */
    void insert(const T& item) override;

    /*! \brief Erase item from counting bloom filter.
     * 
     * \param item - the item to erase from filter.
     * 
     * 
     */
    void erase(const T& item) override;

    /*! \brief Clear filter and resets its internal memory.
     * 
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

#undef CLASS_METHOD_IMPL

#endif   // INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
