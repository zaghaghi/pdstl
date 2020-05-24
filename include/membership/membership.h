#ifndef INCLUDE_MEMBERSHIP_MEMBERSHIP_H_
#define INCLUDE_MEMBERSHIP_MEMBERSHIP_H_

template <typename T>
class Membership {
   public:
    virtual ~Membership() {}
    virtual void insert(const T& item) = 0;
    virtual void erase(const T& item) = 0;
    virtual void clear() = 0;
    virtual bool contains(const T& item) const = 0;
};

#endif   // INCLUDE_MEMBERSHIP_MEMBERSHIP_H_
