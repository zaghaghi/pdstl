#ifndef INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
#define INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
#include <include/exception/not_supported.h>
#include <include/hash/mmh3_hash_factory.h>

#include <bitset>
#include <iostream>
#include <memory>

#include "membership.h"

template <std::size_t HC, std::size_t MC, template <typename...> class HF = MMH3HashFactory, typename T = std::string, typename S = uint32_t>
class BloomFilter : public Membership<T> {
   private:
    std::unique_ptr<HF<T, S>> hash_factory_;
    std::size_t hash_count_;
    std::bitset<MC> bitset_memory_;
    std::vector<std::unique_ptr<Hash<T, S>>> hashes_;

   public:
    explicit BloomFilter();
    ~BloomFilter();
    void insert(const T& item) override;
    void erase(const T& item) override;
    void clear() override;
    bool contains(const T& item) const override;
};

template <std::size_t HC, std::size_t MC, template <typename...> class HF, typename T, typename S>
BloomFilter<HC, MC, HF, T, S>::BloomFilter() : hash_count_(HC) {
    hash_factory_ = std::make_unique<HF<T, S>>();
    bitset_memory_.reset();
    hashes_ = hash_factory_->createHashVector(HC);
}

template <std::size_t HC, std::size_t MC, template <typename...> class HF, typename T, typename S>
BloomFilter<HC, MC, HF, T, S>::~BloomFilter() {
}

template <std::size_t HC, std::size_t MC, template <typename...> class HF, typename T, typename S>
void BloomFilter<HC, MC, HF, T, S>::insert(const T& item) {
    std::cout << "insert(" << item << ")" << std::endl;
    std::for_each(hashes_.cbegin(), hashes_.cend(), [this, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        this->bitset_memory_.set(bit);
    });
    std::cout << bitset_memory_ << std::endl;
}

template <std::size_t HC, std::size_t MC, template <typename...> class HF, typename T, typename S>
void BloomFilter<HC, MC, HF, T, S>::erase(const T& item) {
    throw NotSupportedException();
}

template <std::size_t HC, std::size_t MC, template <typename...> class HF, typename T, typename S>
void BloomFilter<HC, MC, HF, T, S>::clear() {
    bitset_memory_.reset();
}

template <std::size_t HC, std::size_t MC, template <typename...> class HF, typename T, typename S>
bool BloomFilter<HC, MC, HF, T, S>::contains(const T& item) const {
    std::bitset<MC> item_bitset;
    std::for_each(hashes_.cbegin(), hashes_.cend(), [&item_bitset, &item](auto& hash) {
        auto bit = hash->Value(item) % MC;
        item_bitset.set(bit);
    });
    std::cout << bitset_memory_ << std::endl;
    return (bitset_memory_ & item_bitset) == item_bitset;
}
#endif   // INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
