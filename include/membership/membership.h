#ifndef INCLUDE_MEMBERSHIP_MEMBERSHIP_H_
#define INCLUDE_MEMBERSHIP_MEMBERSHIP_H_

namespace pdstl {

/*! \brief Base class for solving membership problem
 * 
 */
template <typename T>
class membership {
   public:
    //! Default destructor
    virtual ~membership() {}

    /*! \brief insert an item into the filter
     * 
     * \param item - the item to insert into the filter.
     */
    virtual void insert(const T& item) = 0;

    /*! \brief erase an item from the filter
     * 
     * \param item - the item to erase from the filter.
     */
    virtual void erase(const T& item) = 0;

    //! \biref clear filter and resets its internal memory.
    virtual void clear() = 0;

    /*! \brief Check the item and report that it's in the filter or not
     * 
     * \param item - the item to check for existence.
     * 
     * \return false if the item is not in the filter, true if item may be in the filter.
     */
    virtual bool contains(const T& item) const = 0;
};

}   // namespace pdstl

#endif   // INCLUDE_MEMBERSHIP_MEMBERSHIP_H_
