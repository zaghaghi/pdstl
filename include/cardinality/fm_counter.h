#ifndef INCLUDE_CARDINALIRT_FM_COUNTER_H_
#define INCLUDE_CARDINALIRT_FM_COUNTER_H_

#include <hash/mmh3_hash_factory.h>

#include <bitset>
#include <cmath>
#include <string>

#include "cardinality.h"

namespace pdstl {

/*! \brief Flajolet–Martin algorithm (a.k.a Probabilistic Counting algorithm with stochastic averaging) for solving cardinality problem
 * 
 * \tparam SC - Number of internal simple counters
 * \tparam MC - Number of memory bits
 * \tparam HF - Hash factory method class (default: pdstl::mmh3_hash_factory)
 * \tparam T - Element type which will be inserted into bloom filter (default: std::string)
 * \tparam S - Hash output size (default: uint32_t)
 */
template <
    std::size_t SC,
    std::size_t MC = sizeof(uint32_t) * 8,
    template <typename...> class HF = mmh3_hash_factory,
    typename T = std::string,
    typename S = uint32_t>
class flajolet_martin_counter : public cardinality<T> {
   private:
    std::unique_ptr<HF<T, S>> hash_factory_;
    std::unique_ptr<hash<S, S>> hash_;
    std::bitset<MC> bitset_memories_[SC];

    static constexpr float phi = 0.77351;

    inline S rank(S quotient) const noexcept {
        if (quotient == 0) {
            return MC - 1;
        }
        int result = 0;
        for (; (quotient & 1) == 0; quotient >>= 1, ++result)
            ;
        return result;
    }

   public:
    //! Default constructor
    flajolet_martin_counter();

    /*! \brief insert an item into the counter
     * 
     * \param item - the item to insert into the counter.
     */
    void insert(const T& item) override;

    //! \biref clear counter and resets its internal memory.
    void clear() override;

    /*! \brief computes and returns cardinality of the inserted items
     * 
     * \return cardinality of the inserted items.
     */
    std::size_t count() const override;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <                              \
        std::size_t SC,                     \
        std::size_t MC,                     \
        template <typename...> class HF,    \
        typename T,                         \
        typename S>                         \
    __VA_ARGS__ flajolet_martin_counter<SC, MC, HF, T, S>::method_name

CLASS_METHOD_IMPL(flajolet_martin_counter, )
() : hash_factory_(std::make_unique<HF<T, S>>()) {
    hash_ = hash_factory_->create_hash();
    for (size_t idx = 0; idx < SC; ++idx) {
        bitset_memories_[idx].reset();
    }
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    S fingerprint = hash_->value(item) % (1 << MC);
    S quotient = fingerprint / SC;
    S remainder = fingerprint % SC;
    bitset_memories_[remainder].set(rank(quotient));
}

CLASS_METHOD_IMPL(clear, void)
() {
    for (size_t idx = 0; idx < SC; ++idx) {
        bitset_memories_[idx].reset();
    }
}

CLASS_METHOD_IMPL(count, std::size_t)
() const {
    size_t sum = 0;
    for (size_t idx = 0; idx < SC; ++idx) {
        for (size_t bit = 0; bit < MC; ++bit) {
            if (!bitset_memories_[idx].test(bit)) {
                sum += bit;
                break;
            }
        }
    }
    return SC / phi * (1 << (sum / SC));
}

#undef CLASS_METHOD_IMPL

}   // namespace pdstl

#endif   // INCLUDE_CARDINALIRT_FM_COUNTER_H_
