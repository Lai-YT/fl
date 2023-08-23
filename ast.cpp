#include "ast.hpp"

#include "visitor.hpp"

using namespace fl;

void Application::Accept(Visitor& v) {
  v.Visit(*this);
}

void BinOp::Accept(Visitor& v) {
  v.Visit(*this);
}

void Branch::Accept(Visitor& v) {
  v.Visit(*this);
}

void Case::Accept(Visitor& v) {
  v.Visit(*this);
}

void FunctionDefinition::Accept(Visitor& v) {
  v.Visit(*this);
}

void Int::Accept(Visitor& v) {
  v.Visit(*this);
}

void PatternConstructor::Accept(Visitor& v) {
  v.Visit(*this);
}

void PatternVar::Accept(Visitor& v) {
  v.Visit(*this);
}

void TypeConstructor::Accept(Visitor& v) {
  v.Visit(*this);
}

void TypeDefinition::Accept(Visitor& v) {
  v.Visit(*this);
}

void TypeId::Accept(Visitor& v) {
  v.Visit(*this);
}

void VarId::Accept(Visitor& v) {
  v.Visit(*this);
}
