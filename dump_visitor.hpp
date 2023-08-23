#pragma once

#include <cstddef>

#include "ast.hpp"
#include "visitor.hpp"

namespace fl {

class DumpVisitor : public Visitor {
 public:
  virtual void Visit(Application&);
  virtual void Visit(BinOp&);
  virtual void Visit(Branch&);
  virtual void Visit(Case&);
  virtual void Visit(FunctionDefinition&);
  virtual void Visit(Int&);
  virtual void Visit(PatternConstructor&);
  virtual void Visit(PatternVar&);
  virtual void Visit(TypeConstructor&);
  virtual void Visit(TypeDefinition&);
  virtual void Visit(TypeId&);
  virtual void Visit(VarId&);

 private:
  static const char* const PADDING_;
  std::size_t curr_pad_ = 0;

  /// @param n The length of the padding, saturated on the boundary of [0, 80].
  static const char* Padding_(int n);
};

}  // namespace fl
