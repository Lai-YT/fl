#include "dump_visitor.hpp"

#include <cassert>
#include <iostream>
#include <iterator>

#include "ast.hpp"

namespace fl {

void DumpVisitor::Visit(Application& app) {
  std::cout << Padding_(curr_pad_);
  auto tmp = curr_pad_;
  curr_pad_ = 0;
  app.left().Accept(*this);
  std::cout << " ";
  app.right().Accept(*this);
  curr_pad_ = tmp;
}

void DumpVisitor::Visit(BinOp& op) {
  std::cout << Padding_(curr_pad_) << "(";
  auto op_symbol = '\0';
  switch (op.op()) {
    case BinOp::Op::KPlus:
      op_symbol = '+';
      break;
    case BinOp::Op::KMinus:
      op_symbol = '-';
      break;
    case BinOp::Op::KTimes:
      op_symbol = '*';
      break;
    case BinOp::Op::KDivide:
      op_symbol = '/';
      break;
    default:
      assert(false && "Unknown Op");
  }
  std::cout << op_symbol << " ";
  auto tmp = curr_pad_;
  curr_pad_ = 0;
  op.lhs().Accept(*this);
  std::cout << " ";
  op.rhs().Accept(*this);
  std::cout << ")";
  curr_pad_ = tmp;
}

void DumpVisitor::Visit(Branch& branch) {
  std::cout << Padding_(curr_pad_);
  auto tmp = curr_pad_;
  curr_pad_ = 0;
  branch.pattern().Accept(*this);
  std::cout << " -> ";
  branch.ast().Accept(*this);
  std::cout << '\n';
  curr_pad_ = tmp;
}

void DumpVisitor::Visit(Case& c) {
  std::cout << Padding_(curr_pad_) << "case: ";
  auto tmp = curr_pad_;
  curr_pad_ = 0;
  c.of().Accept(*this);
  std::cout << " of" << '\n';
  curr_pad_ = tmp;
  curr_pad_ += 2;
  for (auto&& branch : c.branches()) {
    branch->Accept(*this);
  }
  curr_pad_ -= 2;
  std::cout << Padding_(curr_pad_) << "esac";
}

void DumpVisitor::Visit(FunctionDefinition& func) {
  std::cout << Padding_(curr_pad_) << "func: " << func.name() << " (";
  for (auto it = func.params().cbegin(); it != func.params().cend(); ++it) {
    std::cout << *it;
    if (it != std::prev(func.params().cend(), 1)) {
      std::cout << ", ";
    }
  }
  std::cout << ") {" << '\n';
  curr_pad_ += 2;
  func.body()->Accept(*this);
  curr_pad_ -= 2;
  std::cout << '\n';
  std::cout << Padding_(curr_pad_) << "}" << '\n';
}

void DumpVisitor::Visit(Int& i) {
  std::cout << Padding_(curr_pad_) << "Int(" << i.value() << ")";
}

void DumpVisitor::Visit(PatternConstructor& ctor) {
  std::cout << Padding_(curr_pad_) << ctor.constructor();
  for (auto&& param : ctor.params()) {
    std::cout << " " << param;
  }
}

void DumpVisitor::Visit(PatternVar& var) {
  std::cout << Padding_(curr_pad_) << "PatternVar(" << var.var() << ")";
}

void DumpVisitor::Visit(TypeConstructor& ctor) {
  std::cout << Padding_(curr_pad_) << "ctor: " << ctor.name();
  for (auto&& type : ctor.types()) {
    std::cout << " " << type;
  }
}

void DumpVisitor::Visit(TypeDefinition& type) {
  std::cout << Padding_(curr_pad_) << "data: " << type.name() << " {" << '\n';
  curr_pad_ += 2;
  for (auto&& ctor : type.constructor()) {
    ctor->Accept(*this);
    std::cout << '\n';
  }
  curr_pad_ -= 2;
  std::cout << Padding_(curr_pad_) << "}" << '\n';
}

void DumpVisitor::Visit(TypeId& id) {
  std::cout << Padding_(curr_pad_) << "Type(" << id.id() << ")";
}

void DumpVisitor::Visit(VarId& id) {
  std::cout << Padding_(curr_pad_) << "Var(" << id.id() << ")";
}

// clang-format off
// Not to format the padding to emphasize the actual length.

/// @details                               01234567890123456789012345678901234567890123456789012345678901234567890123456789
const char* const DumpVisitor::PADDING_ = "                                                                                ";

// clang-format on

const char* DumpVisitor::Padding_(int n) {
  if (n > 80) {
    n = 80;
  } else if (n < 0) {
    n = 0;
  }
  return PADDING_ + (80 - n);
}

}  // namespace fl
