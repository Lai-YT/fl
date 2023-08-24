#pragma once

#include <memory>   // for unique_ptr
#include <string>   // for string
#include <utility>  // for move
#include <vector>   // for vector

#include "visitable.hpp"

namespace fl {

template <class T>
using UniquePtr = std::unique_ptr<T>;

struct Ast : public Visitable {
  virtual ~Ast() = default;
};

struct Pattern : public Visitable {
  virtual ~Pattern() = default;
};

class Branch : public Visitable {
 public:
  VOID_ACCEPT()

  const auto& pattern() const {
    return *pattern_;
  }

  const auto& ast() const {
    return *ast_;
  }

  Branch(UniquePtr<Pattern> pattern, UniquePtr<Ast> ast)
      : pattern_{std::move(pattern)}, ast_{std::move(ast)} {}

 private:
  UniquePtr<Pattern> pattern_;
  UniquePtr<Ast> ast_;
};

class TypeConstructor : public Visitable {
 public:
  VOID_ACCEPT()

  const auto& name() const {
    return name_;
  }

  const auto& types() const {
    return types_;
  }

  TypeConstructor(std::string name, std::vector<std::string> types)
      : name_{std::move(name)}, types_{std::move(types)} {}

 private:
  std::string name_;
  std::vector<std::string> types_;
};

class Int : public Ast {
 public:
  VOID_ACCEPT()

  auto value() const {
    return value_;
  }

  Int(int v) : value_{v} {}

 private:
  int value_;
};

class TypeId : public Ast {
 public:
  const auto& id() const {
    return id_;
  }

  VOID_ACCEPT()

  explicit TypeId(std::string id) : id_{std::move(id)} {}

 private:
  std::string id_;
};

class VarId : public Ast {
 public:
  const auto& id() const {
    return id_;
  }

  VOID_ACCEPT()

  explicit VarId(std::string id) : id_{std::move(id)} {}

 private:
  std::string id_;
};

class BinOp : public Ast {
 public:
  enum class Op { KPlus, KMinus, KTimes, KDivide };

  auto op() const {
    return op_;
  }

  const auto& lhs() const {
    return *lhs_;
  }

  const auto& rhs() const {
    return *rhs_;
  }

  VOID_ACCEPT()

  BinOp(Op op, UniquePtr<Ast> lhs, UniquePtr<Ast> rhs)
      : op_{op}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

 private:
  Op op_;
  UniquePtr<Ast> lhs_;
  UniquePtr<Ast> rhs_;
};

class Case : public Ast {
 public:
  const auto& of() const {
    return *of_;
  }

  const auto& branches() const {
    return branches_;
  }

  VOID_ACCEPT()

  Case(UniquePtr<Ast> of, std::vector<UniquePtr<Branch>> branches)
      : of_{std::move(of)}, branches_{std::move(branches)} {}

 private:
  UniquePtr<Ast> of_;
  std::vector<UniquePtr<Branch>> branches_;
};

class Application : public Ast {
 public:
  const auto& left() const {
    return *left_;
  }

  const auto& right() const {
    return *right_;
  }

  VOID_ACCEPT()

  Application(UniquePtr<Ast> left, UniquePtr<Ast> right)
      : left_{std::move(left)}, right_{std::move(right)} {}

 private:
  UniquePtr<Ast> left_;
  UniquePtr<Ast> right_;
};

class PatternConstructor : public Pattern {
 public:
  const auto& constructor() const {
    return ctor_;
  }

  const auto& params() const {
    return params_;
  }

  VOID_ACCEPT()

  PatternConstructor(std::string ctor, std::vector<std::string> params)
      : ctor_{std::move(ctor)}, params_{std::move(params)} {}

 private:
  std::string ctor_;
  std::vector<std::string> params_;
};

class PatternVar : public Pattern {
 public:
  const auto& var() const {
    return var_;
  }

  VOID_ACCEPT()

  PatternVar(std::string var) : var_{std::move(var)} {}

 private:
  std::string var_;
};

struct Definition : public Visitable {
  virtual ~Definition() = default;
};

class FunctionDefinition : public Definition {
 public:
  const auto& name() const {
    return name_;
  }

  const auto& params() const {
    return params_;
  }

  const auto& body() const {
    return body_;
  }

  VOID_ACCEPT()

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
  const auto& name() const {
    return name_;
  }

  const auto& constructor() const {
    return ctors_;
  }

  VOID_ACCEPT()

  TypeDefinition(std::string name,
                 std::vector<UniquePtr<TypeConstructor>> ctors)
      : name_{std::move(name)}, ctors_{std::move(ctors)} {}

 private:
  std::string name_;
  std::vector<UniquePtr<TypeConstructor>> ctors_;
};

}  // namespace fl
