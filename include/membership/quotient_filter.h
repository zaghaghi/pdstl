#ifndef INCLUDE_MEMBERSHIP_QUOTIENT_FILTER_H_
#define INCLUDE_MEMBERSHIP_QUOTIENT_FILTER_H_

#include <exception/not_supported.h>
#include <hash/mmh3_hash_factory.h>
#include <table/quotient_table.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "membership.h"

namespace pdstl {

/*! \brief Quotient Filter
 *
 * quotient_filter class implements quotient filter algorithm for solving membership problem.
 * 
 * \tparam F - Fingerprint bits, must be smaller than or equal to hash output size
 * \tparam Q - Number of bits for quotient part, remainder bit size is (F - Q)
 * \tparam HF - Hash factory method class (default: pdstl::mmh3_hash_factory)
 * \tparam T - Element type which will be inserted into quotient filter (default: std::string)
 * \tparam S - Hash output type (default: uint32_t)
 */
template <
    std::size_t F,
    std::size_t Q,
    template <typename...> class HF = mmh3_hash_factory,
    typename T = std::string,
    typename S = uint32_t>
class quotient_filter : public membership<T> {
   protected:
    std::unique_ptr<HF<T, S>> hash_factory_;
    std::unique_ptr<hash<T, S>> hash_;
    quotient_table<S, F - Q> table_;

   public:
    //! Default constructor
    quotient_filter();

    /*! \brief insert an item into quotient filter
     *
     * \param item - the item to insert into the quotient filter.
     * 
     */
    void insert(const T& item) override;

    /*! \brief Erase is not supported in standard quotient filter. will throw an exception
     *
     * \param item - the item to erase from filter.
     * 
     */
    void erase(const T& item) override;

    //! \brief Clear filter and resets its internal memory.
    void clear() override;

    /*! \biref Check the item and report that it's in the filter or not.
     *
     * \param item - the item to check for existence.
     * 
     * \return false if the item is not in the filter, true if item may be in the filter.
     */
    bool contains(const T& item) const override;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <                              \
        std::size_t F,                      \
        std::size_t Q,                      \
        template <typename...> class HF,    \
        typename T,                         \
        typename S>                         \
    __VA_ARGS__ quotient_filter<F, Q, HF, T, S>::method_name

CLASS_METHOD_IMPL(quotient_filter, )
() : table_(1 << Q) {
    static_assert(sizeof(S) * 8 >= F, "Fingerprint size is larger than hash size");
    static_assert(F > Q, "Quotient size must be smaller than fingerprint size");
    hash_factory_ = std::make_unique<HF<T, S>>();
    hash_ = hash_factory_->create_hash();
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    auto fingerprint = hash_->value(item) % (1 << F);
    auto quotient = fingerprint >> (F - Q);
    auto remainder = fingerprint & ((1 << (F - Q)) - 1);
    table_.insert(quotient, remainder);
}

CLASS_METHOD_IMPL(erase, void)
(const T& /* item */) {
    throw not_supported_exception();
}

CLASS_METHOD_IMPL(clear, void)
() {
    table_.clear();
}

CLASS_METHOD_IMPL(contains, bool)
(const T& item) const {
    auto fingerprint = hash_->value(item) % (1 << F);
    auto quotient = fingerprint >> (F - Q);
    auto remainder = fingerprint & ((1 << (F - Q)) - 1);
    return table_.contains(quotient, remainder);
}

#undef CLASS_METHOD_IMPL

}   // namespace pdstl

#endif   // INCLUDE_MEMBERSHIP_QUOTIENT_FILTER_H_
