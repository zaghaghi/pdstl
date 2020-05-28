#ifndef INCLUDE_MEMBERSHIP_QUOTIENT_FILTER_H_
#define INCLUDE_MEMBERSHIP_QUOTIENT_FILTER_H_

#include <exception/not_supported.h>
#include <hash/mmh3_hash_factory.h>

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
   private:
    typedef struct {
        bool is_occupied : 1;
        bool is_continuation : 1;
        bool is_shifted : 1;
        unsigned int fingerprint_remainder : F - Q;
        bool is_empty() { return !is_occupied && !is_continuation && !is_shifted; }
    } bucket;

    typedef struct {
        size_t start;
        size_t end;
    } run;

    typedef struct {
        size_t start;
        size_t end;
    } cluster;

    void shift_right(size_t bucket_index);
    run find_run(size_t bucket_index) const;

   protected:
    std::unique_ptr<HF<T, S>> hash_factory_;
    std::unique_ptr<hash<T, S>> hash_;
    std::vector<bucket> buckets_;

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

#define CLASS_METHOD_IMPL_TYPED(method_name, ...)         \
    template <                                            \
        std::size_t F,                                    \
        std::size_t Q,                                    \
        template <typename...> class HF,                  \
        typename T,                                       \
        typename S>                                       \
    typename quotient_filter<F, Q, HF, T, S>::__VA_ARGS__ \
        quotient_filter<F, Q, HF, T, S>::method_name

CLASS_METHOD_IMPL(quotient_filter, )
() : buckets_(std::vector<bucket>(1 << Q, {0, 0, 0, 0})) {
    static_assert(sizeof(S) * 8 >= F, "Fingerprint size is larger than hash size");
    static_assert(F > Q, "Quotien size must be smaller than fingerprint size");
    hash_factory_ = std::make_unique<HF<T, S>>();
    hash_ = hash_factory_->create_hash();
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    auto fingerprint = hash_->value(item) % (1 << F);
    auto quotient = fingerprint >> (F - Q);
    auto remainder = fingerprint & ((1 << (F - Q)) - 1);
    if (buckets_[quotient].is_empty()) {
        buckets_[quotient].fingerprint_remainder = remainder;
        buckets_[quotient].is_occupied = 1;
        return;
    }
    buckets_[quotient].is_occupied = 1;
    run this_run = find_run(quotient);
    for (size_t run_idx = this_run.start; run_idx < this_run.end; ++run_idx) {
        if (buckets_[run_idx].fingerprint_remainder == remainder) {
            return;
        }
        if (buckets_[run_idx].fingerprint_remainder > remainder) {
            shift_right(run_idx);
            buckets_[run_idx].fingerprint_remainder = remainder;
            return;
        }
    }
    shift_right(this_run.end);
    buckets_[this_run.end].fingerprint_remainder = remainder;
}

CLASS_METHOD_IMPL(erase, void)
(const T& /* item */) {
    throw not_supported_exception();
}

CLASS_METHOD_IMPL(clear, void)
() {
    std::for_each(buckets_.begin(), buckets_.end(), [](bucket& item) {
        item.is_occupied = 0;
        item.is_continuation = 0;
        item.is_shifted = 0;
        item.fingerprint_remainder = 0;
    });
}

CLASS_METHOD_IMPL(contains, bool)
(const T& item) const {
    auto fingerprint = hash_->value(item) % (1 << F);
    auto quotient = fingerprint >> (F - Q);
    auto remainder = fingerprint & ((1 << (F - Q)) - 1);
    if (!buckets_[quotient].is_occupied) {
        return false;
    }
    run this_run = find_run(quotient);
    for (size_t run_idx = this_run.start; run_idx < this_run.end; ++run_idx) {
        if (buckets_[run_idx].fingerprint_remainder == remainder) {
            return true;
        }
    }
    return false;
}

CLASS_METHOD_IMPL(shift_right, void)
(size_t bucket_index) {
    bucket prev = buckets_[bucket_index];
    size_t i = bucket_index + 1;
    while (true) {
        if (buckets_[i].is_empty()) {
            buckets_[i].fingerprint_remainder = prev.fingerprint_remainder;
            buckets_[i].is_continuation = 1;
            buckets_[i].is_shifted = 1;
            break;
        } else {
            bucket curr = buckets_[i];
            buckets_[i].fingerprint_remainder = prev.fingerprint_remainder;
            buckets_[i].is_continuation = prev.is_continuation;
            buckets_[i].is_shifted = prev.is_shifted;
            prev = curr;
        }
        i = (i + 1) % buckets_.size();
    }
}

CLASS_METHOD_IMPL_TYPED(find_run, run)
(size_t bucket_index) const {
    size_t j = bucket_index;
    for (; buckets_[j].is_shifted; j--) {
    }
    size_t run_start = j;
    while (j != bucket_index) {
        run_start++;
        while (buckets_[run_start].is_continuation) {
            run_start++;
        }
        j++;
        while (!buckets_[j].is_occupied) {
            j++;
        }
    }
    size_t run_end = run_start;
    run_end++;
    while (buckets_[run_end].is_continuation) {
        run_end++;
    }
    return run{run_start, run_end};
}

#undef CLASS_METHOD_IMPL
#undef CLASS_METHOD_IMPL_TYPED

}   // namespace pdstl

#endif   // INCLUDE_MEMBERSHIP_QUOTIENT_FILTER_H_
