#include "type.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <tuple>

namespace fl {

std::string TypeManager::GenTypeName() {
  auto temp = last_id_++;
  auto new_type_name = std::string{};
  while (temp != -1) {
    new_type_name += ('a' + (temp % 26 /* num of alphas */));
    temp = temp / 26 - 1;
  }
  std::reverse(new_type_name.begin(), new_type_name.end());
  return new_type_name;
}

Type::SharedPtr TypeManager::GenType() {
  return std::make_shared<VarType>(GenTypeName());
}

Type::SharedPtr TypeManager::GenArrowType() {
  return std::make_shared<ArrowType>(GenType(), GenType());
}

std::tuple<Type::SharedPtr, VarType*> TypeManager::Resolve(
    Type::SharedPtr type) {
  while (auto var = std::dynamic_pointer_cast<VarType>(type)) {
    if (!types_.count(var->name())) {
      // Returns the last unbound variable.
      return {type, var.get()};
    }
    type = types_.at(var->name());
  }
  return {type, nullptr};
}

/// @details
/// 1. Any type variable unifies with any type expression, and is instantiated
/// to that expression.
/// 2. Two type constants unify only if they are the same type.
/// 3. Two type constructions (arrow type) unify only if they are applications
/// of the same type constructor and all of their component types recursively
/// unify.
void TypeManager::Unify(Type::SharedPtr left, Type::SharedPtr right) {
  VarType* l_var = nullptr;
  std::tie(left, l_var) = Resolve(left);
  VarType* r_var = nullptr;
  std::tie(right, r_var) = Resolve(right);
  // 1.
  if (l_var) {
    Bind(l_var->name(), right);
    return;
  }
  if (r_var) {
    Bind(r_var->name(), left);
    return;
  }
  // 3.
  if (auto l_arr = std::dynamic_pointer_cast<ArrowType>(left)) {
    if (auto r_arr = std::dynamic_pointer_cast<ArrowType>(right)) {
      Unify(l_arr->left(), r_arr->left());
      Unify(l_arr->right(), r_arr->right());
      return;  // valid
    }
  }
  // 2.
  if (auto l_base = std::dynamic_pointer_cast<ConstType>(left)) {
    if (auto r_base = std::dynamic_pointer_cast<ConstType>(right)) {
      if (l_base->name() == r_base->name()) {
        return;  // valid
      }
    }
  }

  throw UnificationFailedError{""};
}

void TypeManager::Bind(const std::string& name, Type::SharedPtr type) {
  // Do nothing if the variable binds to itself; tautology.
  auto other = std::dynamic_pointer_cast<VarType>(type);
  if (other && other->name() == name) {
    return;
  }

  types_.at(name) = type;
}

}  // namespace fl
