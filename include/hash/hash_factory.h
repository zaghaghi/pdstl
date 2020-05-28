#ifndef INCLUDE_HASH_HASH_FACTORY_H_
#define INCLUDE_HASH_HASH_FACTORY_H_

#include <memory>
#include <vector>

#include "hash.h"

namespace pdstl {

/*! \brief Base class for hash factories.
 *
 * \tparam T - Input type of hash function (e.g. std::string)
 * \tparam S - Output type of hash function (e.g. uint32_t)
 */
template <
    typename T,
    typename S>
class hash_factory {
   public:
    typedef std::unique_ptr<hash<T, S>> hash_ptr_t;
    typedef std::vector<hash_ptr_t> hash_ptr_vector_t;

    /*! \brief creates a hash object initialized with the given seed
     *  \param seed - initialized seed for hash
     * 
     *  \return unique_ptr of a hash object initialized with \a seed
     */
    virtual hash_ptr_t create_hash(S seed) = 0;

    /*! \brief creates hash object initialized with random seed
     *
     * \return unique_ptr of a hash object initialized with a random seed
     */
    virtual hash_ptr_t create_hash() = 0;

    /*! \brief creates a vector of hashes, all initialized with random seed
     * \param num - number of hash objects
     * 
     * \return a vector of unique_ptr of hash objects, all hashes initialized with distinct random seeds
     */
    virtual hash_ptr_vector_t create_hash_vector(std::size_t num) = 0;

    //! default destructor
    virtual ~hash_factory() {}
};

}   // namespace pdstl

#endif   //INCLUDE_HASH_HASH_FACTORY_H_
