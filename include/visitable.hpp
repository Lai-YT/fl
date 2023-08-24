#pragma once

#include <stdexcept>
#include <type_traits>

#include "visitor.hpp"

namespace fl {

struct Visitable {
  virtual void Accept(BaseVisitor&) = 0;
  virtual void Accept(BaseVisitor&) const = 0;

  using AcceptError = std::runtime_error;

 protected:
  /// @note Should be called by `Accept` unless you want to implement
  /// non-default behaviors.
  /// @tparam T the runtime type of the visitable.
  /// @param visitable usually passed with `*this`.
  /// @param visitor
  /// @throws `AcceptError` if the `visitor` cannot visit a visitable of type
  /// `T`.
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
    } else {
      throw AcceptError{"Unvisitable type"};
    }
  }

  /// @note Should be called by `Accept` unless you want to implement
  /// non-default behaviors.
  /// @tparam T the runtime type of the visitable.
  /// @param visitable usually passed with `*this`.
  /// @param visitor
  /// @throws `AcceptError` if the `visitor` cannot visit a visitable of type
  /// `T`.
  template <class T>
  static void AcceptImpl(const T& visitable, BaseVisitor& visitor) {
    if (auto* v = dynamic_cast<
            Visitor<std::remove_cv_t<std::remove_reference_t<T>>, false>*>(
            &visitor)) {
      v->Visit(visitable);
    } else {
      throw AcceptError{"Unvisitable type"};
    }
  }
};

}  // namespace fl
