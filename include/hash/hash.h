#ifndef INCLUDE_HASH_HASH_H_
#define INCLUDE_HASH_HASH_H_

template <typename T, typename S>
class Hash {
   protected:
    S seed_;

   public:
    explicit Hash(S seed);
    virtual ~Hash();
    virtual S Value(const T& input) const = 0;
};

template <typename T, typename S>
Hash<T, S>::Hash(S seed) : seed_(seed) {
}

template <typename T, typename S>
Hash<T, S>::~Hash() {
}

#endif   // INCLUDE_HASH_HASH_H_
