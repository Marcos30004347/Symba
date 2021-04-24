#ifndef MATH_ALGEBRA_H
#define MATH_ALGEBRA_H

#include "Core/AbstractSyntaxTree/AST.hpp"

namespace algebra {

// true if u is a constant expression, false otherwise
bool isConstant(ast::AST* u);

// true if u is a rational number expression, false otherwise
bool isRNE(ast::AST* u);

bool orderRelation(ast::AST* a, ast::AST* b);

// integer
ast::AST* inte(signed long val);

// symbol
ast::AST* symb(const char* identifier);

// summation between all members of list
ast::AST* add(std::list<ast::AST*>);

// subtraction between all members of list
ast::AST* sub(std::list<ast::AST*>);

// product between all members of list
ast::AST* mul(std::list<ast::AST*>);

// divisition of num by den
ast::AST* div(ast::AST* num, ast::AST* den);

// power bas^exp
ast::AST* pow(ast::AST* bas, ast::AST* exp);

// fraction between num and den
ast::AST* frac(signed long n, signed long d);
ast::AST* frac(ast::AST* n, ast::AST* d);

// factorial of u
ast::AST* fact(ast::AST* u);

// greatest commom denominator between a and b
ast::AST* gcd(ast::AST* a, ast::AST* b);

// numerator of u
ast::AST* num(ast::AST* u);

// denominator of u
ast::AST* den(ast::AST* u);

// base of u
ast::AST* base(ast::AST* u);

// expoent of u
ast::AST* exp(ast::AST* u);


} // algebra

#endif
