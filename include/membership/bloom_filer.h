#ifndef INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
#define INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
#include <memory>

#include "membership.h"

template <typename T, typename HF>
class BloomFilter : public Membership<T> {
   private:
    std::unique_ptr<HF> hash_factory_;
    uint16_t hash_count_;

   public:
    explicit BloomFilter(uint16_t hash_count);
    ~BloomFilter();
    void insert(const T& item) override;
    void erase(const T& item) override;
    void clear() override;
    bool contains(const T& item) const override;
};

template <typename T, typename HF>
BloomFilter<T, HF>::BloomFilter(uint16_t hash_count) : hash_count_(hash_count) {
    hash_factory_ = std::make_unique<HF>();
}

template <typename T, typename HF>
BloomFilter<T, HF>::~BloomFilter() {
}

template <typename T, typename HF>
void BloomFilter<T, HF>::insert(const T& item) {
}

template <typename T, typename HF>
void BloomFilter<T, HF>::erase(const T& item) {
}

template <typename T, typename HF>
void BloomFilter<T, HF>::clear() {
}

template <typename T, typename HF>
bool BloomFilter<T, HF>::contains(const T& item) const {
    return false;
}
#endif   // INCLUDE_MEMBERSHIP_BLOOM_FILTER_H_
