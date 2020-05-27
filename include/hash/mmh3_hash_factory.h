#ifndef INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#define INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#include <memory>
#include <random>
#include <set>

#include "hash_factory.h"
#include "mmh3_hash.h"

template <
    // Input type of hash function
    typename T,
    // Output type of hash function (default: uint32_t)
    typename S = uint32_t>
class MMH3HashFactory : public HashFactory<T, S> {
    std::set<S> generateDistinctRandomSeeds(std::size_t num);

   public:
    typedef std::unique_ptr<Hash<T, S>> HashPtr;
    typedef std::vector<HashPtr> HashPtrVector;
    HashPtr createHash(S seed) override;
    HashPtrVector createHashVector(std::size_t num) override;
    virtual ~MMH3HashFactory() {}
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <typename T, typename S>       \
    __VA_ARGS__ MMH3HashFactory<T, S>::method_name

#define CLASS_METHOD_IMPL_TYPED(method_name, ...) \
    template <typename T, typename S>             \
    typename MMH3HashFactory<T, S>::__VA_ARGS__   \
        MMH3HashFactory<T, S>::method_name

CLASS_METHOD_IMPL_TYPED(createHash, HashPtr)
(S seed) {
    return std::make_unique<MMH3Hash<T, S>>(seed);
}

CLASS_METHOD_IMPL_TYPED(createHashVector, HashPtrVector)
(std::size_t num) {
    HashPtrVector result;
    std::set<S> seeds_set = generateDistinctRandomSeeds(num);
    std::for_each(seeds_set.cbegin(), seeds_set.cend(), [&result](int seed) {
        result.emplace_back(std::make_unique<MMH3Hash<T, S>>(seed));
    });
    return result;
}

CLASS_METHOD_IMPL(generateDistinctRandomSeeds, std::set<S>)
(std::size_t num) {
    std::set<S> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<S> dist;
    while (result.size() < num) {
        result.insert(dist(gen));
    }
    return result;
}

#undef CLASS_METHOD_IMPL
#undef CLASS_METHOD_IMPL_TYPED

#endif   // INCLUDE_HASH_MMH3_HASH_FACTORY_H_
