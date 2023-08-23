#pragma once

#include <memory>   // for unique_ptr
#include <string>   // for string
#include <utility>  // for move
#include <vector>   // for vector

namespace fl {

struct Visitor;

template <class T>
using UniquePtr = std::unique_ptr<T>;

struct Ast {
  virtual void Accept(Visitor&) = 0;

  virtual ~Ast() = default;
};

struct Pattern {
  virtual void Accept(Visitor&) = 0;

  virtual ~Pattern() = default;
};

class Branch {
 public:
  virtual void Accept(Visitor&);

  Pattern& pattern() const {
    return *pattern_;
  }

  Ast& ast() const {
    return *ast_;
  }

  Branch(UniquePtr<Pattern> pattern, UniquePtr<Ast> ast)
      : pattern_{std::move(pattern)}, ast_{std::move(ast)} {}

 private:
  UniquePtr<Pattern> pattern_;
  UniquePtr<Ast> ast_;
};

class TypeConstructor {
 public:
  virtual void Accept(Visitor&);

  auto& name() const {
    return name_;
  }

  auto& types() const {
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
  auto& value() const {
    return value_;
  }

  virtual void Accept(Visitor&) override;

  Int(int v) : value_{v} {}

 private:
  int value_;
};

class TypeId : public Ast {
 public:
  auto& id() const {
    return id_;
  }

  virtual void Accept(Visitor&) override;

  explicit TypeId(std::string id) : id_{std::move(id)} {}

 private:
  std::string id_;
};

class VarId : public Ast {
 public:
  auto& id() const {
    return id_;
  }

  virtual void Accept(Visitor&) override;

  explicit VarId(std::string id) : id_{std::move(id)} {}

 private:
  std::string id_;
};

class BinOp : public Ast {
 public:
  enum class Op { KPlus, KMinus, KTimes, KDivide };

  auto& op() const {
    return op_;
  }

  auto& lhs() const {
    return *lhs_;
  }

  auto& rhs() const {
    return *rhs_;
  }

  virtual void Accept(Visitor&) override;

  BinOp(Op op, UniquePtr<Ast> lhs, UniquePtr<Ast> rhs)
      : op_{op}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

 private:
  Op op_;
  UniquePtr<Ast> lhs_;
  UniquePtr<Ast> rhs_;
};

class Case : public Ast {
 public:
  auto& of() const {
    return *of_;
  }

  auto& branches() const {
    return branches_;
  }

  virtual void Accept(Visitor&) override;

  Case(UniquePtr<Ast> of, std::vector<UniquePtr<Branch>> branches)
      : of_{std::move(of)}, branches_{std::move(branches)} {}

 private:
  UniquePtr<Ast> of_;
  std::vector<UniquePtr<Branch>> branches_;
};

class Application : public Ast {
 public:
  auto& left() const {
    return *left_;
  }

  auto& right() const {
    return *right_;
  }

  virtual void Accept(Visitor&) override;

  Application(UniquePtr<Ast> left, UniquePtr<Ast> right)
      : left_{std::move(left)}, right_{std::move(right)} {}

 private:
  UniquePtr<Ast> left_;
  UniquePtr<Ast> right_;
};

class PatternConstructor : public Pattern {
 public:
  auto& constructor() const {
    return ctor_;
  }

  auto& params() const {
    return params_;
  }

  virtual void Accept(Visitor&) override;

  PatternConstructor(std::string ctor, std::vector<std::string> params)
      : ctor_{std::move(ctor)}, params_{std::move(params)} {}

 private:
  std::string ctor_;
  std::vector<std::string> params_;
};

class PatternVar : public Pattern {
 public:
  auto& var() const {
    return var_;
  }

  virtual void Accept(Visitor&) override;

  PatternVar(std::string var) : var_{std::move(var)} {}

 private:
  std::string var_;
};

struct Definition {
  virtual void Accept(Visitor&) = 0;

  virtual ~Definition() = default;
};

class FunctionDefinition : public Definition {
 public:
  auto& name() const {
    return name_;
  }

  auto& params() const {
    return params_;
  }

  auto& body() const {
    return body_;
  }

  virtual void Accept(Visitor&) override;

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
  auto& name() const {
    return name_;
  }

  auto& constructor() const {
    return ctors_;
  }

  virtual void Accept(Visitor&) override;

  TypeDefinition(std::string name,
                 std::vector<UniquePtr<TypeConstructor>> ctors)
      : name_{std::move(name)}, ctors_{std::move(ctors)} {}

 private:
  std::string name_;
  std::vector<UniquePtr<TypeConstructor>> ctors_;
};

}  // namespace fl
