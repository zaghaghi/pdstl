#ifndef INCLUDE_EXCEPTION_NOT_SUPPORTED_H
#define INCLUDE_EXCEPTION_NOT_SUPPORTED_H

#include <stdexcept>

class NotSupportedException : public std::logic_error {
   public:
    NotSupportedException() : std::logic_error("Method not supported.") {}
};

#endif   // INCLUDE_EXCEPTION_NOT_SUPPORTED_H
