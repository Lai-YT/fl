%{
#include <vector>

#include "ast.hpp"

extern std::vector<fl::UniquePtr<fl::Definition>> program;
%}

%code requires {
#include <string>
#include <vector>

#include "ast.hpp"
}

%code {
  extern yy::parser::symbol_type yylex();
}

%require "3.2"
%language "c++"

// Use complete symbols (parser::symbol_type).
%define api.token.constructor
// Allow non-pointer-based rich types.
%define api.value.type variant
// Check whether symbols are constructed and destructed using RTTI.
%define parse.assert
// Copy may be expensive when using rich types, such as std::vector.
// Also with automove, smart pointers can be moved implicity without boilerplate std::move.
// NOTE: can no longer reference a $x twice since it's moved in the first place.
%define api.value.automove
%define api.token.prefix {TOK_}
%define parse.error verbose

%token PLUS TIMES MINUS DIVIDE FUNC DATA CASE OF ARROW
%token EOF 0
%token <int> INT
%token <std::string> TYPE_ID VAR_ID

%nterm program
%nterm <std::vector<std::string>> type_params var_params
%nterm <std::vector<fl::UniquePtr<fl::Definition>>> definitions
%nterm <std::vector<fl::UniquePtr<fl::Branch>>> branches
%nterm <std::vector<fl::UniquePtr<fl::DataConstructor>>> constructors
%nterm <fl::UniquePtr<fl::Ast>> expr bin_op case app term
%nterm <fl::UniquePtr<fl::Definition>> definition func data
%nterm <fl::UniquePtr<fl::Branch>> branch
%nterm <fl::UniquePtr<fl::Pattern>> pattern
%nterm <fl::UniquePtr<fl::DataConstructor>> constructor

%left '+' '-'
%left '*' '/'

%start program

%%

program
  : definitions {
    program = $1;
  }
  ;

definitions
  : definitions definition {
    $$ = $1;
    $$.push_back($2);
  }
  | definition {
    $$ = std::vector<fl::UniquePtr<fl::Definition>>{};
    $$.push_back($1);
  }
  ;

definition
  : func /* default action */
  | data /* default action */
  ;

func
  : FUNC VAR_ID var_params '=' '{' expr '}' {
    $$ = std::make_unique<fl::FunctionDefinition>($2, $3, $6);
  }
  ;

var_params
  : epsilon {
    $$ = std::vector<std::string>{};
  }
  | var_params VAR_ID {
    $$ = $1;
    $$.push_back($2);
  }
  ;

expr
  : bin_op
  | app
  ;

app
  : app term {
    $$ = std::make_unique<fl::Application>($1, $2);
  }
  | term /* default action */
  ;

bin_op
  : term '+' term {
    $$ = std::make_unique<fl::BinOp>(fl::BinOp::Op::KPlus, $1, $3);
  }
  | term '-' term {
    $$ = std::make_unique<fl::BinOp>(fl::BinOp::Op::KMinus, $1, $3);
  }
  | term '*' term {
    $$ = std::make_unique<fl::BinOp>(fl::BinOp::Op::KTimes, $1, $3);
  }
  | term '/' term {
    $$ = std::make_unique<fl::BinOp>(fl::BinOp::Op::KDivide, $1, $3);
  }
  ;

term
  : INT {
    $$ = std::make_unique<fl::Int>($1);
  }
  | VAR_ID {
    $$ = std::make_unique<fl::VarId>($1);
  }
  | TYPE_ID {
    $$ = std::make_unique<fl::TypeId>($1);
  }
  | '(' expr ')' {
    $$ = $2;
  }
  | case /* default action */
  ;

case
  : CASE expr OF '{' branches '}' {
    $$ = std::make_unique<fl::Case>($2, $5);
  }
  ;

branches
  : branches branch {
    $$ = $1;
    $$.push_back($2);
  }
  | branch {
    $$ = std::vector<fl::UniquePtr<fl::Branch>>{};
    $$.push_back($1);
  }
  ;

branch
  : pattern ARROW '{' expr '}' {
    $$ = std::make_unique<fl::Branch>($1, $4);
  }
  ;

pattern
  : VAR_ID {
    $$ = std::make_unique<fl::VarPattern>($1);
  }
  | TYPE_ID var_params {
    $$ = std::make_unique<fl::ConstructorPattern>($1, $2);
  }
  ;

type_params
  : epsilon {
    $$ = std::vector<std::string>{};
  }
  | type_params TYPE_ID {
    $$ = $1;
    $$.push_back($2);
  }
  ;

data
  : DATA TYPE_ID '=' '{' constructors '}' {
    $$ = std::make_unique<fl::DataDefinition>($2, $5);
  }
  ;

constructors
  : constructors ',' constructor {
    $$ = $1;
    $$.push_back($3);
  }
  | constructor {
    $$ = std::vector<fl::UniquePtr<fl::DataConstructor>>{};
    $$.push_back($1);
  }
  ;

constructor
  : TYPE_ID type_params {
    $$ = std::make_unique<fl::DataConstructor>($1, $2);
  }
  ;

epsilon: /* empty */ ;

%%

void yy::parser::error(const std::string& err) {
  std::cerr << err << '\n';
}
