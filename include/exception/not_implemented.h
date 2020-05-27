#ifndef INCLUDE_EXCEPTION_NOT_IMPLEMENTED_H
#define INCLUDE_EXCEPTION_NOT_IMPLEMENTED_H

#include <stdexcept>

//! \brief Exception used when the method or function is not yet implemented
class NotImplementedException : public std::logic_error {
   public:
    NotImplementedException() : std::logic_error("Function or method not implemented yet.") {}
};

#endif   // INCLUDE_EXCEPTION_NOT_IMPLEMENTED_H
