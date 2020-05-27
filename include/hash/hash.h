#ifndef INCLUDE_HASH_HASH_H_
#define INCLUDE_HASH_HASH_H_

/*! \brief base class for hash classes
 *
 * \tparam T - Input type to hash function (e.g. std::string)
 * \tparam S - Output type from hash function (e.g. uint32_t)
 */
template <
    typename T,
    typename S>
class Hash {
   protected:
    S seed_;

   public:
    /*! \brief constructor for creating a hash initialized with \a seed
     *
     * \param seed - seed used in hash instanse creation
     */
    explicit Hash(S seed);

    //! default destructor
    virtual ~Hash();

    /*! \brief get hash value of \a input
     *
     * \param input - input of type \a T
     * \return hash value of type \a S
     */
    virtual S Value(const T& input) const = 0;
};

#define CLASS_METHOD_IMPL(method_name, ...) \
    template <typename T, typename S>       \
    __VA_ARGS__ Hash<T, S>::method_name

CLASS_METHOD_IMPL(Hash, )
(S seed) : seed_(seed) {
}

CLASS_METHOD_IMPL(~Hash, )
() {
}

#undef CLASS_METHOD_IMPL

#endif   // INCLUDE_HASH_HASH_H_
