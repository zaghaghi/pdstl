#ifndef INCLUDE_HASH_MMH3_HASH_H_
#define INCLUDE_HASH_MMH3_HASH_H_
#include "deps/MurmurHash3.h"
#include "hash.h"

typedef uint32_t S;

template <typename T>
class MMH3Hash : public Hash<T, S> {
   public:
    explicit MMH3Hash(int seed);
    ~MMH3Hash();
    S Value() override;
};

template <typename T>
MMH3Hash<T>::MMH3Hash(int seed) : Hash<T, S>(seed) {
}

template <typename T>
MMH3Hash<T>::~MMH3Hash() {
}

template <typename T>
S MMH3Hash<T>::Value() {
}

#endif   // INCLUDE_HASH_MMH3_HASH_H_
