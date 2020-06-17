#ifndef INCLUDE_CARDINALIRT_CARDINALIRT_H_
#define INCLUDE_CARDINALIRT_CARDINALIRT_H_

#include <cstddef>

namespace pdstl {

/*! \brief Base class for solving cardinality problem
 * 
 */
template <typename T>
class cardinality {
   public:
    //! Default destructor
    virtual ~cardinality() {}

    /*! \brief insert an item into the counter
     * 
     * \param item - the item to insert into the counter.
     */
    virtual void insert(const T& item) = 0;

    //! \biref clear counter and resets its internal memory.
    virtual void clear() = 0;

    /*! \brief computes and returns cardinality of the inserted items
     * 
     * \return cardinality of the inserted items.
     */
    virtual std::size_t count() const = 0;
};

}   // namespace pdstl

#endif   // INCLUDE_CARDINALIRT_CARDINALIRT_H_
