#ifndef INCLUDE_HASH_HASH_H_
#define INCLUDE_HASH_HASH_H_

namespace pdstl {

/*! \brief base class for hash classes
 *
 * \tparam T - Input type to hash function (e.g. std::string)
 * \tparam S - Output type from hash function (e.g. uint32_t)
 */
template <
    typename T,
    typename S>
class hash {
   protected:
    S seed_;

   public:
    /*! \brief constructor for creating a hash initialized with \a seed
     *
     * \param seed - seed used in hash instanse creation
     */
    explicit hash(S seed);

    //! default destructor
    virtual ~hash();

    /*! \brief get hash value of \a input
     *
     * \param input - input of type \a T
     * \return hash value of type \a S
     */
    virtual S value(const T& input) const = 0;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <typename T, typename S>       \
    __VA_ARGS__ hash<T, S>::method_name

CLASS_METHOD_IMPL(hash, )
(S seed) : seed_(seed) {
}

CLASS_METHOD_IMPL(~hash, )
() {
}

#undef CLASS_METHOD_IMPL

}   // namespace pdstl

#endif   // INCLUDE_HASH_HASH_H_
