#pragma once

#include <type_traits>

#include "visitor.hpp"

namespace fl {

/// @brief Default `Accept` functions for the concrete `Visitable`s.
#define DEFINE_DEFAULT_ACCEPT() \
  virtual void Accept(BaseVisitor& v) { \
    Visitable::AcceptImpl(*this, v); \
  } \
  virtual void Accept(BaseVisitor& v) const { \
    Visitable::AcceptImpl(*this, v); \
  }

struct Visitable {
  virtual void Accept(BaseVisitor&) = 0;
  virtual void Accept(BaseVisitor&) const = 0;

 protected:
  template <class T>
  static void AcceptImpl(T& visitable, BaseVisitor& visitor) {
    if (auto* v = dynamic_cast<
            Visitor<std::remove_cv_t<std::remove_reference_t<T>>, true>*>(
            &visitor)) {
      v->Visit(visitable);
    } else if (auto* v = dynamic_cast<Visitor<
                   std::remove_cv_t<std::remove_reference_t<T>>, false>*>(
                   &visitor)) {
      // A modifying Visitor can act like a non-modifying one.
      v->Visit(visitable);
    }
  }

  template <class T>
  static void AcceptImpl(const T& visitable, BaseVisitor& visitor) {
    if (auto* v = dynamic_cast<
            Visitor<std::remove_cv_t<std::remove_reference_t<T>>, false>*>(
            &visitor)) {
      v->Visit(visitable);
    }
  }
};

}  // namespace fl
