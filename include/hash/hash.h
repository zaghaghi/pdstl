#ifndef INCLUDE_HASH_HASH_H_
#define INCLUDE_HASH_HASH_H_

template <
    // Input type to hash function
    typename T,
    // Output type from hash function
    typename S>
class Hash {
   protected:
    S seed_;

   public:
    explicit Hash(S seed);
    virtual ~Hash();
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
