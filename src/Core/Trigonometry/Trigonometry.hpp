#ifndef SUBSTITUTION_TRIGONOMETRY
#define SUBSTITUTION_TRIGONOMETRY

#include "Core/Algebra/Algebra.hpp"

namespace trigonometry {

ast::AST* substituteTrig(ast::AST* u);
ast::AST* expandExponential(ast::AST* u);
ast::AST* contractExponential(ast::AST* u);
ast::AST* expandTrig(ast::AST* u);
ast::AST* contractTrig(ast::AST* u);
}

#endif
