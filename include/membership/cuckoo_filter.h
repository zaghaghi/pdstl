#ifndef INCLUDE_MEMBERSHIP_CUCKOO_FILTER_H_
#define INCLUDE_MEMBERSHIP_CUCKOO_FILTER_H_

#include <exception/not_supported.h>
#include <hash/mmh3_hash_factory.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "membership.h"

namespace pdstl {

template <size_t FB, size_t IC>
struct bucket_info {
    size_t finger_print_bits = FB;
    size_t items_in_bucket = IC;
};

template <size_t FB, size_t IC>
class array_bucket {};

template <size_t IC>
class array_bucket<4, IC> {
   private:
    const size_t k_array_size;
    uint8_t data[IC / 2 + 1];

   public:
    array_bucket() : k_array_size(IC / 2 + 1) {
        std::fill(data, data + k_array_size, 0);
    }

    uint8_t insert(uint8_t item, bool kick = false) {
        uint8_t m_item = item & 0xF;
        for (int index = 0; index < k_array_size; ++index) {
            if ((data[index] & 0x0F) == 0) {
                data[index] |= m_item;
                return 0;
            } else if ((data[index] & 0xF0) == 0) {
                data[index] |= m_item << 4;
                return 0;
            }
        }
        if (!kick) {
            return item;
        }
        uint8_t kickout_item = data[0] & 0x0F;
        data[0] = (data[0] & 0xF0) | m_item;
        return kickout_item;
    }

    void erase(uint8_t item) {
        item = item & 0xF;
        for (int index = 0; index < k_array_size; ++index) {
            if ((data[index] & 0xF0) == item) {
                data[index] &= 0x0F;
            }
            if ((data[index] & 0x0F) == item) {
                data[index] &= 0xF0;
            }
        }
    }

    bool contains(uint8_t item) const {
        item = item & 0xF;
        for (int index = 0; index < k_array_size; ++index) {
            if ((data[index] & 0xF0) == item || (data[index] & 0x0F) == item) {
                return true;
            }
        }
        return false;
    }
};

template <size_t IC>
class array_bucket<8, IC> {
};

template <size_t IC>
class array_bucket<16, IC> {
};

template <size_t IC>
class array_bucket<32, IC> {
};

template <size_t FB, size_t IC, typename S = uint32_t>
class cuckoo_table : public bucket_info<FB, IC> {
   private:
    std::vector<array_bucket<FB, IC>> table_;
    inline size_t round_up(size_t num) const {
        num--;
        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        num |= num >> 32;
        num++;
        return num;
    }

   public:
    explicit cuckoo_table(size_t num_buckets) : bucket_info<FB, IC>(), table_(round_up(num_buckets)) {
        static_assert(FB > 0 && ((FB - 1) & FB) == 0,
                      "FB template parameters must be positive power of two number");
    }

    size_t size() const { return table_.size(); }

    S insert(size_t index, S item, bool kick = false) {
        if (index >= table_.size()) {
            return 0;
        }
        return table_[index].insert(item, kick);
    }

    void erase(size_t index, S item) {
        if (index >= table_.size()) {
            return;
        }
        table_[index].erase(item);
    }
    bool contains(size_t index, S item) const {
        if (index >= table_.size()) {
            return false;
        }
        return table_[index].contains(item);
    }
};

/*! \brief Cuckoo Filter
 *
 * cuckoo_filter class implements cuckoo filter algorithm for solving membership problem.
 * 
 * \tparam CT - cuckoo table
 * \tparam HF - Hash factory method class (default: pdstl::mmh3_hash_factory)
 * \tparam T - Element type which will be inserted into cuckoo filter (default: std::string)
 * \tparam S - Hash output type (default: uint32_t)
 */
template <
    typename CT,
    template <typename...> class HF = mmh3_hash_factory,
    typename T = std::string,
    typename S = uint32_t>
class cuckoo_filter : public membership<T> {
   private:
    bool add_item_to_bucket(size_t index, S item);

   protected:
    std::unique_ptr<HF<T, S>> finger_print_factory_;
    std::unique_ptr<hash<T, S>> finger_print_;
    std::unique_ptr<HF<S, S>> hash_factory_;
    std::unique_ptr<hash<S, S>> hash_;
    CT table_;
    const size_t k_max_kicks_;

   public:
    /*! \brief Default constructor
     *
     * \param num_buckets - number of buckets in this filter
     */
    explicit cuckoo_filter(size_t num_buckets, size_t max_kicks);

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

    /*! \brief Check the item and report that it's in the filter or not.
     *
     * \param item - the item to check for existence.
     * 
     * \return false if the item is not in the filter, true if item may be in the filter.
     */
    bool contains(const T& item) const override;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <                              \
        typename CT,                        \
        template <typename...> class HF,    \
        typename T,                         \
        typename S>                         \
    __VA_ARGS__ cuckoo_filter<CT, HF, T, S>::method_name

CLASS_METHOD_IMPL(cuckoo_filter, )
(size_t num_buckets, size_t max_kicks) : finger_print_factory_(std::make_unique<HF<T, S>>()),
                                         hash_factory_(std::make_unique<HF<S, S>>()),
                                         table_(num_buckets),
                                         k_max_kicks_(max_kicks) {
    hash_ = hash_factory_->create_hash();
    finger_print_ = finger_print_factory_->create_hash();
}

CLASS_METHOD_IMPL(insert, void)
(const T& item) {
    S finger_print = finger_print_->value(item) % (1 << table_.finger_print_bits);
    S i = finger_print_->value(item) & (table_.size() - 1);
    S j = i ^ (hash_->value(finger_print) & (table_.size() - 1));
    if (table_.insert(i, finger_print) == 0 || table_.insert(j, finger_print) == 0) {
        return;
    }
    S cur_index = j;
    for (int n = 0; n < k_max_kicks_; ++n) {
        finger_print = table_.insert(cur_index, finger_print);
        if (finger_print == 0) {
            return;
        }
        cur_index = cur_index ^ (hash_->value(finger_print) & (table_.size() - 1));
    }
}

CLASS_METHOD_IMPL(erase, void)
(const T& item) {
    S finger_print = finger_print_->value(item) % (1 << table_.finger_print_bits);
    S i = finger_print_->value(item) & (table_.size() - 1);
    S j = i ^ (hash_->value(finger_print) & (table_.size() - 1));
    table_.erase(i, finger_print);
    table_.erase(j, finger_print);
}

CLASS_METHOD_IMPL(clear, void)
() {
    // table_.clear();
}

CLASS_METHOD_IMPL(contains, bool)
(const T& item) const {
    S finger_print = finger_print_->value(item) % (1 << table_.finger_print_bits);
    S i = finger_print_->value(item) & (table_.size() - 1);
    S j = i ^ (hash_->value(finger_print) & (table_.size() - 1));
    return table_.contains(i, finger_print) || table_.contains(j, finger_print);
}

#undef CLASS_METHOD_IMPL

}   // namespace pdstl

#endif   // INCLUDE_MEMBERSHIP_CUCKOO_FILTER_H_
