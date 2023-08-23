#pragma once

#include <type_traits>

namespace fl {

/// @brief A degenerate base class. Concrete Visitors should inherit this.
struct BaseVisitor {
  virtual ~BaseVisitor() = 0;
};

/// @brief An acyclic Visitor that allows partial visitation.
/// @tparam T the visitable class.
/// @tparam is_modifying If True, `Visit()` takes a non-const reference of the
/// visitable; otherwise, a const reference. Default to false.
template <class T, bool is_modifying = false>
struct Visitor {
  using Visitable = std::conditional_t<is_modifying, T, const T>;

  virtual void Visit(Visitable& v) = 0;
};

}  // namespace fl
