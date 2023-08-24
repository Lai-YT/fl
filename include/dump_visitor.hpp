#pragma once

#include <cstddef>

#include "ast.hpp"
#include "visitor.hpp"

namespace fl {

class DumpVisitor : public BaseVisitor,
                    public Visitor<Application>,
                    public Visitor<BinOp>,
                    public Visitor<Branch>,
                    public Visitor<Case>,
                    public Visitor<FunctionDefinition>,
                    public Visitor<Int>,
                    public Visitor<PatternConstructor>,
                    public Visitor<PatternVar>,
                    public Visitor<TypeConstructor>,
                    public Visitor<TypeDefinition>,
                    public Visitor<TypeId>,
                    public Visitor<VarId> {
 public:
  void Visit(const Application&) override;
  void Visit(const BinOp&) override;
  void Visit(const Branch&) override;
  void Visit(const Case&) override;
  void Visit(const FunctionDefinition&) override;
  void Visit(const Int&) override;
  void Visit(const PatternConstructor&) override;
  void Visit(const PatternVar&) override;
  void Visit(const TypeConstructor&) override;
  void Visit(const TypeDefinition&) override;
  void Visit(const TypeId&) override;
  void Visit(const VarId&) override;

 private:
  static const char* const PADDING_;
  std::size_t curr_pad_ = 0;

  /// @param n The length of the padding, saturated on the boundary of [0, 80].
  static const char* Padding_(int n);
};

}  // namespace fl
