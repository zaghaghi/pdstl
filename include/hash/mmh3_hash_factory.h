#ifndef INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#define INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#include <algorithm>
#include <memory>
#include <random>
#include <set>

#include "hash_factory.h"
#include "mmh3_hash.h"

/*! \brief hash factory for MurmurHash3
 *
 * \tparam T - Input type of hash function
 * \tparam S - Output type of hash function (default: uint32_t)
 */
template <
    typename T,
    typename S = uint32_t>
class MMH3HashFactory : public HashFactory<T, S> {
   private:
    std::set<S> generateDistinctRandomSeeds(std::size_t num);

   public:
    using typename HashFactory<T, S>::HashPtr;
    using typename HashFactory<T, S>::HashPtrVector;
    /*! \brief creates a MurmurHash3 initialized with \a seed
     * \param seed - initialized seed for hash
     * 
     * \return unique_ptr of a MurmurHash3 object initialized with \a seed
     */
    HashPtr createHash(S seed) override;

    /*! \brief creates a MurmurHash3 initialized with random seed
     *
     * \return unique_ptr of a MurmurHash3 object initialized with a random seed
     */
    HashPtr createHash() override;

    /* \brief creates a vector of MurmurHash3 instances initialized with random seed
     * @num number of hash objects
     * 
     * @return a vector of unique_ptr of Murmurhash3 objects, all hashes initialized with distinct random seeds
     */
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

CLASS_METHOD_IMPL_TYPED(createHash, HashPtr)
() {
    std::set<S> seeds_set = generateDistinctRandomSeeds(1);
    return std::make_unique<MMH3Hash<T, S>>(*seeds_set.begin());
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
