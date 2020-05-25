#ifndef INCLUDE_HASH_MMH3_HASH_H_
#define INCLUDE_HASH_MMH3_HASH_H_
#include <string>

#include "deps/MurmurHash3.h"
#include "hash.h"

template <typename T, typename S = uint32_t>
class MMH3Hash : public Hash<T, S> {
   public:
    explicit MMH3Hash(S seed);
    ~MMH3Hash();
    S Value(const T& input) const override;
};

template <typename T, typename S>
MMH3Hash<T, S>::MMH3Hash(S seed) : Hash<T, S>(seed) {
}

template <typename T, typename S>
MMH3Hash<T, S>::~MMH3Hash() {
}

template <>
uint32_t MMH3Hash<std::string, uint32_t>::Value(const std::string& input) const {
    uint32_t output;
    MurmurHash3_x86_32(input.c_str(), input.size(), seed_, &output);
    return output;
}

#endif   // INCLUDE_HASH_MMH3_HASH_H_
