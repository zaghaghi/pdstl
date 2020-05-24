#ifndef INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#define INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#include <memory>

#include "hash_factory.h"
#include "mmh3_hash.h"

typedef uint32_t S;

template <typename T>
class MMH3HashFactory : public HashFactory<T, S> {
   public:
    std::unique_ptr<Hash<T, S>> createHash(int seed) override;
    std::vector<std::unique_ptr<Hash<T, S>>> createHashVector(int num) override;
    virtual ~MMH3HashFactory() {}
};

template <typename T>
std::unique_ptr<Hash<T, S>> MMH3HashFactory<T>::createHash(int seed) {
    return std::make_unique<MMH3Hash<T>>(42);
}

template <typename T>
std::vector<std::unique_ptr<Hash<T, S>>> MMH3HashFactory<T>::createHashVector(int num) {
    std::vector<std::unique_ptr<Hash<T, S>>> result;
    for (auto i = 0; i < num; i++) {
        result.emplace_back(std::make_unique<MMH3Hash<T>>(42));
    }

    return result;
}

#endif   //INCLUDE_HASH_MMH3_HASH_FACTORY_H_
