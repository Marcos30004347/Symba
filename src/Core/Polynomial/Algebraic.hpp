#ifndef MATH_ALGEBRA_POLYNOMIALS_ALGEBRAIC_H
#define MATH_ALGEBRA_POLYNOMIALS_ALGEBRAIC_H

#include "Polynomial.hpp"

#include <vector>

namespace polynomial {

/**
 * @brief Simplifies each coefficient of u using the side relation p(a) = 0.
 * 
 * @param u A polynomial defined in Q(a)[x]
 * @param x The symbol x
 * @param p The caracteristic polynomial of Q(a) such that p(a) = 0
 * @param a The algebraic number
 * @return The polynomial u with simplified coefficients in Q(a)[x] 
 */
ast::AST* algCoeffSimp(ast::AST* u, ast::AST* x, ast::AST* p, ast::AST* a);


/**
 * @brief Computes the multiplicative inverse of v in Q(a)
 * 
 * @param v A Polynomial in Q(a) with degree(v) < degree(p) 
 * @param p The caracteristic polynomial of Q(a) 
 * 					with degree(p) >= 2
 * @param a The algebraic number
 * @return The multiplicative inverse of v in Q(a)
 */
ast::AST* algMulInverse(ast::AST* v, ast::AST* p, ast::AST* a);

/**
 * @brief Computes the quotient of u divided by v in Q(a)
 * 
 * @param u A Polynomial in Q(a) with degree(u) < degree(p) 
 * @param v A Polynomial in Q(a) with degree(v) < degree(p) 
 * @param p The caracteristic polynomial of Q(a) 
 * 					with degree(p) >= 2
 * @param a The algebraic number
 * @return The quotient of u divided by v in Q(a)
 */
ast::AST* algDivide(ast::AST* u, ast::AST* v, ast::AST* p, ast::AST* a);

/**
 * @brief Computes the quotient and remaider of the 
 * 				division of u(x) by v(x) in the algebraic
 * 				number field Q(a)[x].
 * 
 * @param u The dividend polynomial in Q(a)[X]
 * @param v The divisor polynomial in Q(a)[X]
 * @param x The symbol x
 * @param p The caracteristic polynomial of a
 * @param a The algebraic number
 * @return A list with quotient and remainder
 */
ast::AST* algPolynomialDivision(ast::AST* u, ast::AST* v, ast::AST* x, ast::AST* p, ast::AST* a);

/**
 * @brief Computes the remaider of the division 
 * 				of u(x) by v(x) in the algebraic number
 * 				field Q(a)[x].
 * 
 * @param u The dividend polynomial in Q(a)[X]
 * @param v The divisor polynomial in Q(a)[X]
 * @param x The symbol x
 * @param p The caracteristic polynomial of Q(a)
 * @param a The algebraic number
 * @return The remainder
 */
ast::AST* algPolynomialRemainder(ast::AST* u, ast::AST* v, ast::AST* x, ast::AST* p, ast::AST* a);

/**
 * @brief Computes the quotient of the division 
 * 				of u(x) by v(x) in the algebraic number
 * 				field Q(a)[x].
 * 
 * @param u The dividend polynomial in Q(a)[X]
 * @param v The divisor polynomial in Q(a)[X]
 * @param x The symbol x
 * @param p The caracteristic polynomial of Q(a)
 * @param a The algebraic number
 * @return The quotient
 */
ast::AST* algPolynomialQuotient(ast::AST* u, ast::AST* v, ast::AST* x, ast::AST* p, ast::AST* a);

/**
 * @brief Computes the GCD between u(x) and v(x) 
 * 				in the algebraic number field Q(a)[x].
 * 
 * @param u A polynomial in Q(a)[X]
 * @param v A polynomial in Q(a)[X]
 * @param x The symbol x
 * @param p The caracteristic polynomial of Q(a)
 * @param a The algebraic number
 * @return The GCD between u(x) and v(x)
 */
ast::AST* algPolynomialGCD(ast::AST* u, ast::AST* v, ast::AST* x, ast::AST* p, ast::AST* a);

/**
 * @brief Transforms the polynomial u into monic form 
 * 				in the algebraic number field Q(a)[x].
 * 
 * @param u A polynomial in Q(a)[X].
 * @param x The symbol x
 * @param p The caracteristic polynomial of Q(a) .
 * @param a The algebraic number
 * @return The monic form of u(x) in Q(a)[x]
 */
ast::AST* algMonic(ast::AST* u,ast::AST* x, ast::AST* p, ast::AST* a);


}

#endif
