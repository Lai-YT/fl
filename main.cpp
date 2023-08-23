#include <vector>

#include "ast.hpp"
#include "dump_visitor.hpp"
#include "parser.hpp"

extern void yylex_destroy();

std::vector<fl::UniquePtr<fl::Definition>> program;

int main() {
  yy::parser parser{};
  auto ret = parser.parse();
  yylex_destroy();
  auto dump_visitor = fl::DumpVisitor{};
  for (auto&& p : program) {
    p->Accept(dump_visitor);
  }
  return ret;
}
