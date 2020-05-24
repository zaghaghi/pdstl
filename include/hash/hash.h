#ifndef INCLUDE_HASH_HASH_H_
#define INCLUDE_HASH_HASH_H_

template <typename T, typename S>
class Hash {
   private:
    uint16_t seed_;

   public:
    explicit Hash(uint16_t seed);
    virtual ~Hash();
    virtual S Value() = 0;
};

template <typename T, typename S>
Hash<T, S>::Hash(uint16_t seed) : seed_(seed) {
}

template <typename T, typename S>
Hash<T, S>::~Hash() {
}

#endif   // INCLUDE_HASH_HASH_H_
