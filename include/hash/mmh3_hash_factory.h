#ifndef INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#define INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#include <memory>
#include <random>
#include <set>

#include "hash_factory.h"
#include "mmh3_hash.h"

template <typename T, typename S = uint32_t>
class MMH3HashFactory : public HashFactory<T, S> {
    std::set<S> generateDistinctRandomSeeds(std::size_t num);

   public:
    std::unique_ptr<Hash<T, S>> createHash(S seed) override;
    std::vector<std::unique_ptr<Hash<T, S>>> createHashVector(std::size_t num) override;
    virtual ~MMH3HashFactory() {}
};

template <typename T, typename S>
std::unique_ptr<Hash<T, S>> MMH3HashFactory<T, S>::createHash(S seed) {
    return std::make_unique<MMH3Hash<T, S>>(seed);
}

template <typename T, typename S>
std::vector<std::unique_ptr<Hash<T, S>>> MMH3HashFactory<T, S>::createHashVector(std::size_t num) {
    std::vector<std::unique_ptr<Hash<T, S>>> result;
    std::set<S> seeds_set = generateDistinctRandomSeeds(num);
    std::for_each(seeds_set.cbegin(), seeds_set.cend(), [&result](int seed) {
        result.emplace_back(std::make_unique<MMH3Hash<T, S>>(seed));
    });
    return result;
}

template <typename T, typename S>
std::set<S> MMH3HashFactory<T, S>::generateDistinctRandomSeeds(std::size_t num) {
    std::set<S> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<S> dist;
    while (result.size() < num) {
        result.insert(dist(gen));
    }
    return result;
}

#endif   // INCLUDE_HASH_MMH3_HASH_FACTORY_H_
