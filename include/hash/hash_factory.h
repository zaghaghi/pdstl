#ifndef INCLUDE_HASH_HASH_FACTORY_H_
#define INCLUDE_HASH_HASH_FACTORY_H_

#include <memory>
#include <vector>

#include "include/hash/hash.h"

template <typename T, typename S>
class HashFactory {
   public:
    virtual std::unique_ptr<Hash<T, S>> createHash(int seed) = 0;
    virtual std::vector<std::unique_ptr<Hash<T, S>>> createHashVector(int num) = 0;
    virtual ~HashFactory() {}
};

#endif   //INCLUDE_HASH_HASH_FACTORY_H_
