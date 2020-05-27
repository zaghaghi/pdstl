#ifndef INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
#define INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
#include <exception/not_supported.h>
#include <hash/mmh3_hash_factory.h>

#include <bitset>
#include <memory>
#include <string>
#include <vector>

#include "membership.h"

/*
 * Standard Bloom Filter
 *
 * BloomFilter class implements bloom filter algorithm for solving membership problem.
 */
template <
    // Number of hash functions
    std::size_t HC,
    // Number of memory bits
    std::size_t MC,
    // Hash factory method class (default: MMH3HashFactory)
    template <typename...> class HF = MMH3HashFactory,
    // Element type which will be inserted into bloom filter (default: std::string)
    typename T = std::string,
    // Hash output size (default: uint32_t)
    typename S = uint32_t>
class BloomFilter : public Membership<T> {
   protected:
    std::unique_ptr<HF<T, S>> hash_factory_;
    std::size_t hash_count_;
    std::bitset<MC> bitset_memory_;
    std::vector<std::unique_ptr<Hash<T, S>>> hashes_;

   public:
    // Default constructor
    BloomFilter();

    /* insert
     * @item the item to insert into the bloom filter.
     * 
     * Insert item into bloom filter.
     */
    void insert(const T& item) override;

    /* erase
     * @item the item to erase from filter.
     * 
     * Erase is not supported in standard bloom filter. will throw an exception
     */
    void erase(const T& item) override;

    /* clear
     * 
     * Clear filter and resets its internal memory.
     */
    void clear() override;

    /* contains
     * @item the item to check for existence.
     * 
     * Check the item and report that it's in the filter or not.
     * @return false if the item is not in the filter, true if item may be in the filter.
     */
    bool contains(const T& item) const override;
};

#define CLASS_METHOD_IMPL(method_name, ...)    \
    template <std::size_t HC, std::size_t MC,  \
              template <typename...> class HF, \
              typename T, typename S>          \
    __VA_ARGS__ BloomFilter<HC, MC, HF, T, S>::method_name

CLASS_METHOD_IMPL(BloomFilter, )
() : hash_count_(HC) {
    hash_factory_ = std::make_unique<HF<T, S>>();
    bitset_memory_.reset();
    hashes_ = hash_factory_->createHashVector(HC);
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        this->bitset_memory_.set(bit);
    });
}

CLASS_METHOD_IMPL(erase, void)
(const T& item) {
    throw NotSupportedException();
}

CLASS_METHOD_IMPL(clear, void)
() {
    bitset_memory_.reset();
}

CLASS_METHOD_IMPL(contains, bool)
(const T& item) const {
    std::bitset<MC> item_bitset;
    std::for_each(hashes_.cbegin(), hashes_.cend(),
                  [&item_bitset, &item](auto& hash) {
                      auto bit = hash->Value(item) % MC;
                      item_bitset.set(bit);
                  });
    return (bitset_memory_ & item_bitset) == item_bitset;
}

#endif   // INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
