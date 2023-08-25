#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

namespace fl {

struct Type {
  using SharedPtr = std::shared_ptr<Type>;
  virtual ~Type() = default;
};

class VarType : public Type {
 public:
  auto name() const {
    return name_;
  }

  explicit VarType(std::string name) : name_{std::move(name)} {}

 private:
  std::string name_;
};

class ConstType : public Type {
 public:
  auto name() const {
    return name_;
  }

  explicit ConstType(std::string name) : name_{std::move(name)} {}

 private:
  std::string name_;
};

class ArrowType : public Type {
 public:
  const auto& left() const {
    return left_;
  }

  const auto& right() const {
    return right_;
  }

  ArrowType(SharedPtr left, SharedPtr right)
      : left_{std::move(left)}, right_{std::move(right)} {}

 private:
  SharedPtr left_;
  SharedPtr right_;
};

/// @details Generates internal unique names for the placeholders.
class TypeManager {
 public:
  /// @brief Tries to find values for placeholder variables such that they can
  /// equal.
  /// @throws `UnificationFailedError`
  void Unify(Type::SharedPtr left, Type::SharedPtr right);

  using UnificationFailedError = std::logic_error;

 private:
  int last_id_ = 0;
  std::map<std::string, Type::SharedPtr> types_;

  std::string GenTypeName();
  Type::SharedPtr GenType();
  Type::SharedPtr GenArrowType();
  /// @brief Gets to the "bottom" of a chain of equations.
  std::tuple<Type::SharedPtr, VarType*> Resolve(Type::SharedPtr type);
  /// @brief Maps a type variable of `name` to a `type`
  void Bind(const std::string& name, Type::SharedPtr type);
};

}  // namespace fl
