#ifndef INCLUDE_EXCEPTION_NOT_SUPPORTED_H
#define INCLUDE_EXCEPTION_NOT_SUPPORTED_H

#include <stdexcept>

//! \brief Exception used when the method or function is not supported
class NotSupportedException : public std::logic_error {
   public:
    NotSupportedException() : std::logic_error("Method not supported.") {}
};

#endif   // INCLUDE_EXCEPTION_NOT_SUPPORTED_H
