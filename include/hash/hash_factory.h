#ifndef INCLUDE_HASH_HASH_FACTORY_H_
#define INCLUDE_HASH_HASH_FACTORY_H_

#include <memory>
#include <vector>

#include "hash.h"

template <typename T, typename S>
class HashFactory {
   public:
    virtual std::unique_ptr<Hash<T, S>> createHash(S seed) = 0;
    virtual std::vector<std::unique_ptr<Hash<T, S>>> createHashVector(uint32_t num) = 0;
    virtual ~HashFactory() {}
};

#endif   //INCLUDE_HASH_HASH_FACTORY_H_
