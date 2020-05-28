#ifndef INCLUDE_HASH_MMH3_HASH_H_
#define INCLUDE_HASH_MMH3_HASH_H_
#include <MurmurHash3.h>

#include <string>

#include "hash.h"

namespace pdstl {

/*! \brief MurmurHash3 class
 *
 * \tparam T - Input type to hash function
 * \tparam S -Output type from hash function (default: uint32_t)
 */
template <
    typename T,
    typename S = uint32_t>
class mmh3_hash : public hash<T, S> {
   public:
    /*! \brief constructor for creating a MurmurHash3 initialized with \a seed
     *
     * \param seed - seed used in MurmurHash3 instanse creation
     */
    explicit mmh3_hash(S seed);

    //! default destructor
    ~mmh3_hash();

    /*! \brief get hash value of \a input
     *
     * \param input - input of type \a T
     * \return hash value of type \a S
     */
    S value(const T& input) const override;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <typename T, typename S>       \
    __VA_ARGS__ mmh3_hash<T, S>::method_name

CLASS_METHOD_IMPL(mmh3_hash, )
(S seed) : hash<T, S>(seed) {
}

CLASS_METHOD_IMPL(~mmh3_hash, )
() {
}

#undef CLASS_METHOD_IMPL

template <>
uint32_t mmh3_hash<std::string, uint32_t>::value(const std::string& input) const {
    uint32_t output;
    MurmurHash3_x86_32(input.c_str(), input.size(), seed_, &output);
    return output;
}

}   // namespace pdstl

#endif   // INCLUDE_HASH_MMH3_HASH_H_
