#include <vector>

#include "ast.hpp"
#include "parser.hpp"

extern void yylex_destroy();

std::vector<fl::UniquePtr<fl::Definition>> program;

int main() {
  yy::parser parser{};
  auto ret = parser.parse();
  yylex_destroy();
  for (auto&& p : program) {
    p->Dump(0);
  }
  return ret;
}
