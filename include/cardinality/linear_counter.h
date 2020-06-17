#ifndef INCLUDE_CARDINALIRT_LINEAR_COUNTER_H_
#define INCLUDE_CARDINALIRT_LINEAR_COUNTER_H_

#include <hash/mmh3_hash_factory.h>

#include <bitset>
#include <cmath>
#include <string>

#include "cardinality.h"

namespace pdstl {

/*! \brief linear counter class for solving cardinality problem
 * 
 * \tparam MC - Number of memory bits
 * \tparam HF - Hash factory method class (default: pdstl::mmh3_hash_factory)
 * \tparam T - Element type which will be inserted into bloom filter (default: std::string)
 * \tparam S - Hash output size (default: uint32_t)
 */
template <
    std::size_t MC,
    template <typename...> class HF = mmh3_hash_factory,
    typename T = std::string,
    typename S = uint32_t>
class linear_counter : public cardinality<T> {
   private:
    std::unique_ptr<HF<T, S>> hash_factory_;
    std::unique_ptr<hash<S, S>> hash_;
    std::bitset<MC> bitset_memory_;

   public:
    //! Default constructor
    linear_counter();

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
        std::size_t MC,                     \
        template <typename...> class HF,    \
        typename T,                         \
        typename S>                         \
    __VA_ARGS__ linear_counter<MC, HF, T, S>::method_name

CLASS_METHOD_IMPL(linear_counter, )
() : hash_factory_(std::make_unique<HF<T, S>>()) {
    hash_ = hash_factory->create_hash();
    bitset_memory_.reset();
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    bitset_memory_.set(hash_->value(item) % MC);
}

CLASS_METHOD_IMPL(clear, void)
() {
    bitset_memory_.reset();
}

CLASS_METHOD_IMPL(count, std::size_t)
() const {
    return -MC * std::log(MC - bitset_memory_.count() / float(MC));
}

#undef CLASS_METHOD_IMPL

}   // namespace pdstl

#endif   // INCLUDE_CARDINALIRT_LINEAR_COUNTER_H_
