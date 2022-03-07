
#include "Gauss.hpp"
#include "MathSystem/Algebra/Expression.hpp"
#include "MathSystem/Algebra/Matrix.hpp"
#include "MathSystem/Calculus/Derivative.hpp"
#include "MathSystem/GaloisField/GaloisField.hpp"
#include "MathSystem/Polynomial/Polynomial.hpp"
#include "MathSystem/Polynomial/Resultant.hpp"

using namespace gauss;
using namespace algebra;

expr number(double v, long max_den) {
  double integral, fractional;

  fractional = std::modf(v, &integral);

  unsigned long long n, d;

  alg::toFraction(fractional, max_den, n, d);

  alg::expr r = Int(integral) + alg::fraction(n, d);

  reduce(&r);

  return r;
}

expr number(const char *v) { return expr(Int::fromString(v)); }

expr number(long long v) { return expr(v); }

expr symbol(const char *s) { return expr(s); }

expr add(expr a, expr b) { return a + b; }

expr sub(expr a, expr b) { return a - b; }

expr mul(expr a, expr b) { return a * b; }

expr div(expr a, expr b) { return a / b; }

expr expand(expr a) {
  expand(&a);

  return a;
}

expr reduce(expr a) {
  reduce(&a);

  return a;
}

expr log(expr a, expr b) { return alg::log(a, b); }

expr ln(expr a) { return alg::ln(a); }

expr exp(expr a) { return alg::exp(a); }

expr trigonometry::sinh(expr x) { return alg::sinh(x); }

expr trigonometry::cosh(expr x) { return alg::cosh(x); }

expr trigonometry::tanh(expr x) { return alg::tanh(x); }

expr trigonometry::cos(expr x) { return alg::cos(x); }

expr trigonometry::sin(expr x) { return alg::sin(x); }

expr trigonometry::tan(expr x) { return alg::tan(x); }

expr trigonometry::csc(expr x) { return alg::csc(x); }

expr trigonometry::cot(expr x) { return alg::cot(x); }

expr trigonometry::sec(expr x) { return alg::sec(x); }

expr trigonometry::coth(expr x) { return alg::coth(x); }

expr trigonometry::sech(expr x) { return alg::sech(x); }

expr trigonometry::csch(expr x) { return alg::csch(x); }

expr trigonometry::arccos(expr x) { return alg::arccos(x); }

expr trigonometry::arcsin(expr x) { return alg::arcsin(x); }

expr trigonometry::arctan(expr x) { return alg::arctan(x); }

expr trigonometry::arccot(expr x) { return alg::arccot(x); }

expr trigonometry::arcsec(expr x) { return alg::arcsec(x); }

expr trigonometry::arccsc(expr x) { return alg::arccsc(x); }

expr trigonometry::arccosh(expr x) { return alg::arccosh(x); }

expr trigonometry::arctanh(expr x) { return alg::arctanh(x); }

linear::mat linear::addMat(linear::mat a, linear::mat b) { return a + b; }

linear::mat linear::subMat(linear::mat a, linear::mat b) { return a - b; }

linear::mat linear::mulMat(linear::mat a, linear::mat b) { return a * b; }

linear::mat linear::mulMatConst(linear::mat a, double b) { return a * b; }

linear::mat linear::divMatConst(linear::mat a, double b) { return a / b; }

linear::mat linear::rowEchelonFormMat(linear::mat A) {
  return alg::echelonForm(A);
}

linear::mat linear::transpMat(linear::mat A) {
  return alg::transpose(A);
}

expr polynomial::factorPoly(algebra::expr poly) {
	expr L = poly::getVariableListForPolyExpr(poly);

	expr p = poly::polyExpr(poly, L);

	return poly::factorPolyExprAndExpand(p, L, expr("Q"));
}


expr polynomial::resultantOfPoly(algebra::expr a, algebra::expr b) {
	expr T = poly::normalizeToPolyExprs(a, b);
	return poly::resultantPolyExpr(T[1], T[2], T[0], expr("Q"));
}


expr polynomial::addPoly(algebra::expr a, algebra::expr b) {
	alg::expand(&a);
	alg::expand(&b);

	expr c = a + b;

	alg::reduce(&c);

	return c;
}


expr polynomial::subPoly(algebra::expr a, algebra::expr b) {
	alg::expand(&a);
	alg::expand(&b);

	expr c = a - b;

	alg::reduce(&c);

	return c;
}

expr polynomial::mulPoly(algebra::expr a, algebra::expr b) {
	alg::expand(&a);
	alg::expand(&b);

	expr c = a * b;

	alg::reduce(&c);

	return c;
}

expr polynomial::divPoly(algebra::expr a, algebra::expr b) {
  expr T = poly::normalizeToPolyExprs(a, b);
	expr L = T[0];

	expr K = expr("Z");

	expr D = poly::divPolyExpr(T[1], T[2], L, K);

	return D[0] + D[1];
}

expr polynomial::quoPoly(algebra::expr a, algebra::expr b) {
  expr T = poly::normalizeToPolyExprs(a, b);
	expr L = T[0];

	expr K = expr("Z");

	expr D = poly::divPolyExpr(T[1], T[2], L, K);

	return D[0];
}


expr polynomial::remPoly(algebra::expr a, algebra::expr b) {
  expr T = poly::normalizeToPolyExprs(a, b);
	expr L = T[0];

	expr K = expr("Z");

	expr D = poly::divPolyExpr(T[1], T[2], L, K);

	return D[1];
}

expr polynomial::gcdPoly(algebra::expr a, algebra::expr b) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr L = T[0];

	expr K = expr("Q");

	expr D = poly::gcdPolyExpr(T[1], T[2], L, K);

	return D[1];
}

expr polynomial::lcmPoly(algebra::expr a, algebra::expr b) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr L = T[0];

	expr K = expr("Q");

	expr D = poly::lcmPolyExpr(T[1], T[2], L, K);

	return D[1];
}


expr polynomial::finiteField::mod(algebra::expr a, long long p) {
	expr L = poly::getVariableListForPolyExpr(a);

	expr u = poly::polyExpr(a, L);

	return galoisField::gfPolyExpr(u, p, false);
}


expr polynomial::finiteField::addPolyFiniteField(algebra::expr a, algebra::expr b, long long p) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr D = galoisField::addPolyExprGf(T[1], T[2], p);

	return D;
}


expr polynomial::finiteField::subPolyFiniteField(algebra::expr a, algebra::expr b, long long p) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr D = galoisField::subPolyExprGf(T[1], T[2], p);

	return D;
}


expr polynomial::finiteField::mulPolyFiniteField(algebra::expr a, algebra::expr b, long long p) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr D = galoisField::mulPolyExprGf(T[1], T[2], p);

	return D;
}

expr polynomial::finiteField::divPolyFiniteField(algebra::expr a, algebra::expr b, long long p) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr L = T[0];

	expr D = galoisField::divPolyExprGf(T[1], T[2], L, p);

	return D[0] + D[1];
}

expr polynomial::finiteField::quoPolyFiniteField(algebra::expr a, algebra::expr b, long long p) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr L = T[0];

	expr D = galoisField::quoPolyExprGf(T[1], T[2], L, p);

	return D;
}

expr polynomial::finiteField::remPolyFiniteField(algebra::expr a, algebra::expr b, long long p) {
  expr T = poly::normalizeToPolyExprs(a, b);

	expr L = T[0];

	expr D = galoisField::remPolyExprGf(T[1], T[2], L, p);

	return D;
}

expr calculus::derivative(algebra::expr a, algebra::expr x) {
	return calc::derivate(a, x);
}

std::string toString(algebra::expr a) {
	return alg::to_string(&a);
}
