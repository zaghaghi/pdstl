#ifndef INCLUDE_TABLE_QUOTIENT_TEABLE_H_
#define INCLUDE_TABLE_QUOTIENT_TEABLE_H_

#include <exception/not_supported.h>

#include <algorithm>
#include <memory>
#include <vector>

namespace pdstl {

/*! \brief Quotient Table
 *
 * quotient_table class implements hash table defained in quotient filter algorithm.
 * 
 * \tparam T - Type of value
 * \tparam E - Number of bits used for each value (default: sizeof(T) * 8 -3)
 */
template <
    typename T,
    std::size_t E = sizeof(T) * 8 - 3>
class quotient_table {
   protected:
    typedef struct {
        unsigned char is_occupied : 1;
        unsigned char is_continuation : 1;
        unsigned char is_shifted : 1;
        T value : E;
        bool is_empty() { return !is_occupied && !is_continuation && !is_shifted; }
    } bucket;

    typedef struct {
        size_t start;
        size_t end;
    } run;

    inline size_t next(size_t table_idx) const { return (table_idx + 1) % table_.size(); }
    inline size_t prev(size_t table_idx) const { return (table_.size() + table_idx - 1) % table_.size(); }
    void shift_right(size_t bucket_index);
    run find_run(size_t bucket_index) const;

   protected:
    std::vector<bucket> table_;

   public:
    /*! \brief Default constructor
     *
     * \param size - Number of keys in the table
     */
    explicit quotient_table(size_t size);

    /*! \brief insert a key-value in the table
     *
     * \param key - the key to insert into the table.
     * \param value - the value to insert into the table
     * 
     */
    void insert(size_t key, T value);

    /*! \brief Erase a key from the table.
     *
     * \param key - the key to erase from table.
     * 
     */
    void erase(size_t key);

    //! \brief Clear table and resets its internal memory.
    void clear();

    /*! \biref Check the table for key-value existence.
     *
     * \param key - the key to insert into the table.
     * \param value - the value to insert into the table
     *
     * \return true if the key-value is in the table, false otherwise.
     */
    bool contains(size_t key, T value) const;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <typename T, std::size_t E>    \
    __VA_ARGS__ quotient_table<T, E>::method_name

#define CLASS_METHOD_IMPL_TYPED(method_name, ...) \
    template <typename T, std::size_t E>          \
    typename quotient_table<T, E>::__VA_ARGS__ quotient_table<T, E>::method_name

CLASS_METHOD_IMPL(quotient_table, )
(size_t size) : table_(std::vector<bucket>(size, {0, 0, 0, 0})) {
    static_assert(E <= sizeof(T) * 8, "Invalid number of bits");
}

CLASS_METHOD_IMPL(insert, void)
(size_t key, T value) {
    if (table_[key].is_empty()) {
        table_[key].value = value;
        table_[key].is_occupied = 1;
        return;
    }
    table_[key].is_occupied = 1;
    run this_run = find_run(key);
    if (table_[this_run.start].is_empty()) {
        table_[this_run.start].value = value;
        table_[this_run.start].is_shifted = 1;
        return;
    }
    for (size_t run_idx = this_run.start; run_idx != this_run.end; run_idx = next(run_idx)) {
        if (table_[run_idx].value == value) {
            return;
        }
        if (!table_[run_idx].is_empty() && table_[run_idx].value > value) {
            if (!table_[run_idx].is_continuation) {
                table_[run_idx].is_continuation = 1;
                unsigned char is_shifted = table_[run_idx].is_shifted;
                shift_right(run_idx);
                table_[run_idx].value = value;
                table_[run_idx].is_shifted = is_shifted;
                table_[run_idx].is_continuation = 0;
            } else {
                shift_right(run_idx);
                table_[run_idx].value = value;
                table_[run_idx].is_shifted = 1;
                table_[run_idx].is_continuation = 1;
            }
            return;
        }
    }
    shift_right(this_run.end);
    table_[this_run.end].value = value;
    table_[this_run.end].is_shifted = 1;
    table_[this_run.end].is_continuation = 1;
}

CLASS_METHOD_IMPL(erase, void)
(size_t /* key */) {
    throw not_supported_exception();
}

CLASS_METHOD_IMPL(clear, void)
() {
    std::for_each(table_.begin(), table_.end(), [](bucket& item) {
        item.is_occupied = 0;
        item.is_continuation = 0;
        item.is_shifted = 0;
        item.value = 0;
    });
}

CLASS_METHOD_IMPL(contains, bool)
(size_t key, T value) const {
    if (!table_[key].is_occupied) {
        return false;
    }
    run this_run = find_run(key);
    for (size_t run_idx = this_run.start; run_idx != this_run.end; run_idx = next(run_idx)) {
        if (table_[run_idx].value == value) {
            return true;
        }
    }
    return false;
}

CLASS_METHOD_IMPL(shift_right, void)
(size_t bucket_index) {
    bucket curr = table_[bucket_index];
    if (curr.is_empty()) {
        return;
    }
    size_t next_idx = next(bucket_index);
    while (true) {
        if (table_[next_idx].is_empty()) {
            table_[next_idx].value = curr.value;
            table_[next_idx].is_continuation = curr.is_continuation;
            table_[next_idx].is_shifted = 1;
            break;
        } else {
            bucket tmp = table_[next_idx];
            table_[next_idx].value = curr.value;
            table_[next_idx].is_continuation = curr.is_continuation;
            table_[next_idx].is_shifted = 1;
            curr = tmp;
        }
        next_idx = next(next_idx);
    }
}

CLASS_METHOD_IMPL_TYPED(find_run, run)
(size_t bucket_index) const {
    size_t j = bucket_index;
    for (; table_[j].is_shifted; j = prev(j)) {
    }
    size_t run_start = j;
    while (j != bucket_index) {
        do {
            run_start = next(run_start);
        } while (table_[run_start].is_continuation);

        do {
            j = next(j);
        } while (!table_[j].is_occupied);
    }
    size_t run_end = run_start;
    do {
        run_end = next(run_end);
    } while (table_[run_end].is_continuation);

    return run{run_start, run_end};
}

#undef CLASS_METHOD_IMPL
#undef CLASS_METHOD_IMPL_TYPED

}   // namespace pdstl

#endif   // INCLUDE_TABLE_QUOTIENT_TEABLE_H_
