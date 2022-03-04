#ifndef SIMPLIFICATION_FACTORIALS_H
#define SIMPLIFICATION_FACTORIALS_H

#include "MathSystem/Algebra/Algebra.hpp"

namespace simplification {

ast::Expr reduceFactorialAST(ast::Expr& u);
ast::Expr reduceFactorialAST(ast::Expr&& u);

}

#endif