%option noyywrap never-interactive noinput nounput

%{
#include <cstdlib>
#include <iostream>

#include "parser.hpp"

/// @brief Give Flex the prototype of yylex we want.
/// @note This define isn't emitted to the header file, i.e., files including
/// the header will see the old function prototype `int yylex()`. This seems to
/// have no solution currently. A way to resolve this is to not generate the
/// header file and have users `extern` the prototypes.
# define YY_DECL \
  yy::parser::symbol_type yylex()
%}

type_id [A-Z][a-zA-Z]+
var_id [a-z][a-z_]*
operator [-+*/=]
symbol [\{\}\(\),]

%%

[ \t\n]+ {}

  /* operators */
{operator} { return yytext[0]; }

  /* integers */
[0-9]+ { return yy::parser::make_INT(std::atoi(yytext)); }

  /* keywords */
func { return yy::parser::make_FUNC(); }
data { return yy::parser::make_DATA(); }
case { return yy::parser::make_CASE(); }
of { return yy::parser::make_OF(); }

  /* symbols */
{symbol} { return yytext[0]; }
"->" { return yy::parser::make_ARROW(); }

  /* identifiers */
{var_id} { return yy::parser::make_VAR_ID(yytext); }
{type_id} { return yy::parser::make_TYPE_ID(yytext); }

<<EOF>> { return yy::parser::make_EOF(); }

. {
    std::cerr << "Invalid symbol: " << yytext << std::endl;
    std::exit(-1);
}
%%
