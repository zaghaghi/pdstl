#ifndef INCLUDE_EXCEPTION_NOT_IMPLEMENTED_H
#define INCLUDE_EXCEPTION_NOT_IMPLEMENTED_H

#include <stdexcept>

namespace pdstl {

//! \brief Exception used when the method or function is not yet implemented
class not_implemented_exception : public std::logic_error {
   public:
    not_implemented_exception() : std::logic_error("Function or method not implemented yet.") {}
};

}   // namespace pdstl

#endif   // INCLUDE_EXCEPTION_NOT_IMPLEMENTED_H
