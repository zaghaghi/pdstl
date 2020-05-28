#ifndef INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#define INCLUDE_HASH_MMH3_HASH_FACTORY_H_
#include <algorithm>
#include <memory>
#include <random>
#include <set>

#include "hash_factory.h"
#include "mmh3_hash.h"

namespace pdstl {

/*! \brief hash factory for MurmurHash3
 *
 * \tparam T - Input type of hash function
 * \tparam S - Output type of hash function (default: uint32_t)
 */
template <
    typename T,
    typename S = uint32_t>
class mmh3_hash_factory : public hash_factory<T, S> {
   private:
    std::set<S> generate_distinct_random_seeds(std::size_t num);

   public:
    using typename hash_factory<T, S>::hash_ptr_t;
    using typename hash_factory<T, S>::hash_ptr_vector_t;
    /*! \brief creates a MurmurHash3 initialized with \a seed
     * \param seed - initialized seed for hash
     * 
     * \return unique_ptr of a MurmurHash3 object initialized with \a seed
     */
    hash_ptr_t create_hash(S seed) override;

    /*! \brief creates a MurmurHash3 initialized with random seed
     *
     * \return unique_ptr of a MurmurHash3 object initialized with a random seed
     */
    hash_ptr_t create_hash() override;

    /* \brief creates a vector of MurmurHash3 instances initialized with random seed
     * @num number of hash objects
     * 
     * @return a vector of unique_ptr of Murmurhash3 objects, all hashes initialized with distinct random seeds
     */
    hash_ptr_vector_t create_hash_vector(std::size_t num) override;
    virtual ~mmh3_hash_factory() {}
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <typename T, typename S>       \
    __VA_ARGS__ mmh3_hash_factory<T, S>::method_name

#define CLASS_METHOD_IMPL_TYPED(method_name, ...) \
    template <typename T, typename S>             \
    typename mmh3_hash_factory<T, S>::__VA_ARGS__ \
        mmh3_hash_factory<T, S>::method_name

CLASS_METHOD_IMPL_TYPED(create_hash, hash_ptr_t)
(S seed) {
    return std::make_unique<mmh3_hash<T, S>>(seed);
}

CLASS_METHOD_IMPL_TYPED(create_hash, hash_ptr_t)
() {
    std::set<S> seeds_set = generate_distinct_random_seeds(1);
    return std::make_unique<mmh3_hash<T, S>>(*seeds_set.begin());
}

CLASS_METHOD_IMPL_TYPED(create_hash_vector, hash_ptr_vector_t)
(std::size_t num) {
    hash_ptr_vector_t result;
    std::set<S> seeds_set = generate_distinct_random_seeds(num);
    std::for_each(seeds_set.cbegin(), seeds_set.cend(), [&result](int seed) {
        result.emplace_back(std::make_unique<mmh3_hash<T, S>>(seed));
    });
    return result;
}

CLASS_METHOD_IMPL(generate_distinct_random_seeds, std::set<S>)
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

}   // namespace pdstl

#endif   // INCLUDE_HASH_MMH3_HASH_FACTORY_H_
