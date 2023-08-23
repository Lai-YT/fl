#pragma once

#include "ast.hpp"

namespace fl {

struct Visitor {
  virtual void Visit(Application&) = 0;
  virtual void Visit(BinOp&) = 0;
  virtual void Visit(Branch&) = 0;
  virtual void Visit(Case&) = 0;
  virtual void Visit(FunctionDefinition&) = 0;
  virtual void Visit(Int&) = 0;
  virtual void Visit(PatternConstructor&) = 0;
  virtual void Visit(PatternVar&) = 0;
  virtual void Visit(TypeConstructor&) = 0;
  virtual void Visit(TypeDefinition&) = 0;
  virtual void Visit(TypeId&) = 0;
  virtual void Visit(VarId&) = 0;
};

}  // namespace fl
