#ifndef INCLUDE_EXCEPTION_NOT_SUPPORTED_H
#define INCLUDE_EXCEPTION_NOT_SUPPORTED_H

#include <stdexcept>

namespace pdstl {

//! \brief Exception used when the method or function is not supported
class not_supported_exception : public std::logic_error {
   public:
    not_supported_exception() : std::logic_error("Method not supported.") {}
};

}   // namespace pdstl

#endif   // INCLUDE_EXCEPTION_NOT_SUPPORTED_H
