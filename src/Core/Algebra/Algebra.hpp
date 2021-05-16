#ifndef MATH_ALGEBRA_H
#define MATH_ALGEBRA_H

#include "Core/AST/AST.hpp"

namespace algebra {

bool isConstant(ast::AST* u);

bool isTerminal(ast::AST* u);

bool isRNE(ast::AST* u);

bool orderRelation(ast::AST* a, ast::AST* b);

bool isLessEqZero(ast::AST* u);
bool isLessZero(ast::AST* u);
bool isEqZero(ast::AST* u);
bool isGreaterEqZero(ast::AST* u);
bool isGreaterZero(ast::AST* u);
bool isDivisionByZero(ast::AST* k);

ast::AST* completeSubExpressions(ast::AST* u);

ast::AST* integer(signed long val);

ast::AST* symbol(const char* identifier);

ast::AST* add(std::vector<ast::AST*>);

ast::AST* sub(std::vector<ast::AST*>);

ast::AST* mul(std::vector<ast::AST*>);

ast::AST* div(ast::AST* numerator, ast::AST* denominator);

ast::AST* power(ast::AST* bas, ast::AST* expoent);

ast::AST* fraction(signed long n, signed long d);

ast::AST* fraction(ast::AST* n, ast::AST* d);

ast::AST* factorial(ast::AST* u);

ast::AST* base(ast::AST* u);

ast::AST* expoent(ast::AST* u);

ast::AST* binomial(signed long n, std::vector<signed long> ks);

ast::AST* funCall(const char* id, std::vector<ast::AST*> args);

ast::AST* integerGCD(ast::AST* a, ast::AST* b);

ast::AST* min(ast::AST* a, ast::AST* b);

ast::AST* max(ast::AST* a, ast::AST* b);

ast::AST* undefined();


} // algebra

#endif
