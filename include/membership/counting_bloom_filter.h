#ifndef INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
#define INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_

#include <algorithm>
#include <string>
#include <vector>

#include "bloom_filter.h"

namespace pdstl {

/*! \brief Counting Bloom Filter
 *
 * counting_bloom_filter class implements counting filter algorithm for solving membership problem.
 * 
 * \tparam HC - Number of hash functions
 * \tparam MC - Number of memory bits
 * \tparam C - Type of counter (default: uint16_t)
 * \tparam HF - Hash factory method class (default: pdstl::mmh3_hash_factory)
 * \tparam T - Element type which will be inserted into counting bloom filter (default: std::string)
 * \tparam S - Hash output size (default: uint32_t)
 */
template <
    std::size_t HC,
    std::size_t MC,
    typename C = uint16_t,
    template <typename...> class HF = mmh3_hash_factory,
    typename T = std::string,
    typename S = uint32_t>
class counting_bloom_filter : public bloom_filter<HC, MC, HF, T, S> {
   protected:
    std::vector<C> counters_;
    using bloom_filter<HC, MC, HF, T, S>::hashes_;
    using bloom_filter<HC, MC, HF, T, S>::bitset_memory_;

   public:
    //! Default constructor
    counting_bloom_filter();

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
    __VA_ARGS__ counting_bloom_filter<HC, MC, C, HF, T, S>::method_name

CLASS_METHOD_IMPL(counting_bloom_filter, )
() : bloom_filter<HC, MC, HF, T, S>(), counters_(MC, 0) {
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->value(item) % MC;
        this->counters_[bit] += 1;
        if (this->counters_[bit] == 1) {
            this->bitset_memory_.set(bit);
        }
    });
}

CLASS_METHOD_IMPL(erase, void)
(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->value(item) % MC;
        this->counters_[bit] -= 1;
        if (this->counters_[bit] == 0) {
            this->bitset_memory_.set(bit, false);
        }
    });
}

CLASS_METHOD_IMPL(clear, void)
() {
    bloom_filter<HC, MC, HF, T, S>::clear();
    counters_.clear();
    counters_.resize(MC, 0);
}

#undef CLASS_METHOD_IMPL

}   // namespace pdstl

#endif   // INCLUDE_MEMBERSHIP_COUNTING_BLOOM_FILTER_H_
