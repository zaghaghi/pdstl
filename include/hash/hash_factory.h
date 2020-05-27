#ifndef INCLUDE_HASH_HASH_FACTORY_H_
#define INCLUDE_HASH_HASH_FACTORY_H_

#include <memory>
#include <vector>

#include "hash.h"

/*! \brief Base class for hash factories.
 *
 * \tparam T - Input type of hash function (e.g. std::string)
 * \tparam S - Output type of hash function (e.g. uint32_t)
 */
template <
    typename T,
    typename S>
class HashFactory {
   public:
    typedef std::unique_ptr<Hash<T, S>> HashPtr;
    typedef std::vector<HashPtr> HashPtrVector;

    /*! \brief creates a hash object initialized with the given seed
     *  \param seed - initialized seed for hash
     * 
     *  \return unique_ptr of a hash object initialized with \a seed
     */
    virtual HashPtr createHash(S seed) = 0;

    /*! \brief creates hash object initialized with random seed
     *
     * \return unique_ptr of a hash object initialized with a random seed
     */
    virtual HashPtr createHash() = 0;

    /*! \brief creates a vector of hashes, all initialized with random seed
     * \param num - number of hash objects
     * 
     * \return a vector of unique_ptr of hash objects, all hashes initialized with distinct random seeds
     */
    virtual HashPtrVector createHashVector(std::size_t num) = 0;

    //! default destructor
    virtual ~HashFactory() {}
};

#endif   //INCLUDE_HASH_HASH_FACTORY_H_
