#pragma once

#include <cassert>   // for assert
#include <iostream>  // for cout
#include <iterator>  // for prev
#include <memory>    // for unique_ptr
#include <string>    // for string
#include <utility>   // for move
#include <vector>    // for vector

// clang-format off
// Not to format the padding to emphasize the actual length.

// 80 spaces for padding      01234567890123456789012345678901234567890123456789012345678901234567890123456789
static const char* padding = "                                                                                ";

// clang-format on

/// @param n The length of the padding, saturated on the boundary of [0, 80].
static const char* Pad(int n);

namespace fl {

template <class T>
using UniquePtr = std::unique_ptr<T>;

struct Ast {
  virtual void Dump(int pad) const = 0;
  virtual ~Ast() = default;
};

struct Pattern {
  virtual void Dump(int pad) const = 0;
  virtual ~Pattern() = default;
};

class Branch {
 public:
  virtual void Dump(int pad) const {
    std::cout << Pad(pad);
    pattern_->Dump(0);
    std::cout << " -> ";
    ast_->Dump(0);
    std::cout << '\n';
  }

  Branch(UniquePtr<Pattern> pattern, UniquePtr<Ast> ast)
      : pattern_{std::move(pattern)}, ast_{std::move(ast)} {}

 private:
  UniquePtr<Pattern> pattern_;
  UniquePtr<Ast> ast_;
};

class TypeConstructor {
 public:
  virtual void Dump(int pad) const {
    std::cout << Pad(pad) << "ctor: " << name_ << " ";
    for (auto&& type : types_) {
      std::cout << type << " ";
    }
  }

  TypeConstructor(std::string name, std::vector<std::string> types)
      : name_{std::move(name)}, types_{std::move(types)} {}

 private:
  std::string name_;
  std::vector<std::string> types_;
};

class Int : public Ast {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "Int(" << value_ << ")";
  }

  Int(int v) : value_{v} {}

 private:
  int value_;
};

class TypeId : public Ast {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "Type(" << id_ << ")";
  }

  explicit TypeId(std::string id) : id_{std::move(id)} {}

 private:
  std::string id_;
};

class VarId : public Ast {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "Var(" << id_ << ")";
  }

  explicit VarId(std::string id) : id_{std::move(id)} {}

 private:
  std::string id_;
};

class BinOp : public Ast {
 public:
  enum class Op { KPlus, KMinus, KTimes, KDivide };

  void Dump(int pad) const override {
    std::cout << Pad(pad) << "(";
    auto op_symbol = '\0';
    switch (op_) {
      case Op::KPlus:
        op_symbol = '+';
        break;
      case Op::KMinus:
        op_symbol = '-';
        break;
      case Op::KTimes:
        op_symbol = '*';
        break;
      case Op::KDivide:
        op_symbol = '/';
        break;
      default:
        assert(false && "Unknown Op");
    }
    std::cout << op_symbol << " ";
    lhs_->Dump(0);
    std::cout << " ";
    rhs_->Dump(0);
    std::cout << ")";
  }

  BinOp(Op op, UniquePtr<Ast> lhs, UniquePtr<Ast> rhs)
      : op_{op}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

 private:
  Op op_;
  UniquePtr<Ast> lhs_;
  UniquePtr<Ast> rhs_;
};

class Case : public Ast {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "case: ";
    of_->Dump(0);
    std::cout << " of" << '\n';
    for (auto&& branch : branches_) {
      branch->Dump(pad + 2);
    }
    std::cout << Pad(pad) << "esac";
  }

  Case(UniquePtr<Ast> of, std::vector<UniquePtr<Branch>> branches)
      : of_{std::move(of)}, branches_{std::move(branches)} {}

 private:
  UniquePtr<Ast> of_;
  std::vector<UniquePtr<Branch>> branches_;
};

class Application : public Ast {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad);
    left_->Dump(0);
    std::cout << " ";
    right_->Dump(0);
  }

  Application(UniquePtr<Ast> left, UniquePtr<Ast> right)
      : left_{std::move(left)}, right_{std::move(right)} {}

 private:
  UniquePtr<Ast> left_;
  UniquePtr<Ast> right_;
};

class PatternConstructor : public Pattern {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << ctor_;
    for (auto&& param : params_) {
      std::cout << " " << param;
    }
  }

  PatternConstructor(std::string ctor, std::vector<std::string> params)
      : ctor_{std::move(ctor)}, params_{std::move(params)} {}

 private:
  std::string ctor_;
  std::vector<std::string> params_;
};

class PatternVar : public Pattern {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "PatternVar(" << var_ << ")";
  }

  PatternVar(std::string var) : var_{std::move(var)} {}

 private:
  std::string var_;
};

struct Definition {
  virtual void Dump(int pad) const = 0;
  virtual ~Definition() = default;
};

class FunctionDefinition : public Definition {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "func: " << name_ << " (";
    for (auto it = params_.cbegin(); it != params_.cend(); ++it) {
      std::cout << *it;
      if (it != std::prev(params_.cend(), 1)) {
        std::cout << ", ";
      }
    }
    std::cout << ") {" << '\n';
    body_->Dump(pad + 2);
    std::cout << '\n';
    std::cout << Pad(pad) << "}" << '\n';
  }

  FunctionDefinition(std::string name, std::vector<std::string> params,
                     UniquePtr<Ast> body)
      : name_{std::move(name)},
        params_{std::move(params)},
        body_{std::move(body)} {}

 private:
  std::string name_;
  std::vector<std::string> params_;
  UniquePtr<Ast> body_;
};

class TypeDefinition : public Definition {
 public:
  void Dump(int pad) const override {
    std::cout << Pad(pad) << "data: " << name_ << " {" << '\n';
    for (auto&& ctor : ctors_) {
      ctor->Dump(pad + 2);
      std::cout << '\n';
    }
    std::cout << Pad(pad) << "}" << '\n';
  }

  TypeDefinition(std::string name,
                 std::vector<UniquePtr<TypeConstructor>> ctors)
      : name_{std::move(name)}, ctors_{std::move(ctors)} {}

 private:
  std::string name_;
  std::vector<UniquePtr<TypeConstructor>> ctors_;
};

}  // namespace fl

static const char* Pad(int n) {
  if (n > 80) {
    n = 80;
  } else if (n < 0) {
    n = 0;
  }
  return padding + (80 - n);
}
