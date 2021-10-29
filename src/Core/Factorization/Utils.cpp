#include "Core/Polynomial/Polynomial.hpp"
#include "Core/Simplification/Simplification.hpp"
#include "Core/Debug/Assert.hpp"

#include <cmath>
#include <limits>
#include <random>

using namespace ast;
using namespace algebra;
using namespace polynomial;
using namespace simplification;

namespace factorization {

long long fact(long long p)
{
	long long f = 1;

	while(p > 0)
	{
		f = f * p;
		p = p - 1;
	}

	return f;
}

long long comb(long long n, long long k)
{
	return fact(n) / (fact(k) * fact(n - k));
}

long long landauMignotteBound(ast::AST* u, ast::AST* x)
{
	double P;
	long long d, cn;
	AST *p, *lc, *n, *t1, *t2;

	p = algebraicExpand(u);

	n = degree(p, x);

	lc = leadCoeff(p, x);

	assert(
		lc->kind() == Kind::Integer, 
		"Landau Mignote Bound works on polynomial"
		"with integer coefficients only"
	);

	P = 0;

	d = std::floor(n->value() * 0.5);

	cn = lc->value();
	
	delete n;
	delete lc;

	// iterate over all factors of u(x)
	while(p->isNot(0))
	{
		lc = leadCoeff(p, x);

		assert(
			lc->kind() == Kind::Integer, 
			"Landau Mignote Bound works on polynomial"
			"with integer coefficients only"
		);

		P = P + lc->value() * lc->value();

		t1 = power(x->copy(), n->copy());
	
		t2 = mulPoly(lc, t1);
		
		delete t1;

		t1 = subPoly(p, t2);

		delete p;

		p = t1;

		delete lc;
	}	

	delete p;

	P = std::sqrt(P);

	double B = 0.0;

	B += comb(d - 1, std::floor(d * 0.5) - 1) * P;
	B += comb(d - 1, std::floor(d * 0.5)) * cn;

	return std::ceil(B);
}

// AST* repeatSquaring(AST* a, AST* x, long n, long m)
// {
// 	AST *t1, *t2, *b[64];

// 	long v = n;

// 	long k = 0;

// 	while (v >>= 1) k++;

// 	b[k] = a->copy();

// 	for (long i = k - 1; i>= 0; i--)
// 	{
// 		t1 = mulPoly(b[i + 1], b[i + 1]);

// 		t2 = reduceAST(t1);

// 		delete t1;

// 		t1 = sZp(t2, x, m);
	
// 		delete t2;

// 		if(n & (1 << i))
// 		{
// 			t2 = mulPoly(t1, a);

// 			b[i] = sZp(t2, x, m);

// 			delete t2;
// 		}
// 		else
// 		{
// 			b[i] = sZp(t1, x, m);
// 		}

// 		delete t1;
// 	}

// 	for(int i = 1; i<=k; i++) delete b[i];
	
// 	return b[0];
// }



long long norm(AST* u, AST* L, AST* K, long long i)
{
	if(i == L->numberOfOperands())
	{
		assert(
			u->kind() == Kind::Integer, 
			"Polynomial needs to have"
			"integer coefficients in K[L...]"
		);

		return u->value();
	}

	long long k = 0;

	AST *q, *p, *t, *e, *c, *n;
	
	n = degree(u, L->operand(i));
	
	p = algebraicExpand(u);

	for(long j = n->value(); j >= 0; j--)
	{
		e = integer(j);
	
		c = coeff(u, L->operand(i), e);

		k = std::max(std::abs(norm(c, L, K, i + 1)), std::abs(k));
	
		t = mul({c, power(L->operand(i)->copy(), e)});
	
		q = subPoly(p, t);

		delete p;
	
		p = algebraicExpand(q);	
	
		delete t;
	
		delete q;
	}

	delete p;

	delete n;

	return k;
}



long long norm(AST* u, AST* x)
{
	long long k = 0;

	AST *q, *p, *t, *e, *c, *n;
	
	n = degree(u, x);
	
	p = algebraicExpand(u);

	for(long j = n->value(); j >= 0; j--)
	{
		e = integer(j);
	
		c = coeff(u, x, e);

		assert(c->kind() == Kind::Integer, "coeffs needs to be integers");
	
		k = std::max(std::abs(c->value()), std::abs(k));
	
		t = mul({c, power(x->copy(), e)});
	
		q = subPoly(p, t);

		delete p;
	
		p = algebraicExpand(q);	
	
		delete t;
	
		delete q;
	}

	delete p;

	delete n;

	return k;
}

long long l1norm(AST* u, AST* L, AST* K, long long i)
{
	if(i == L->numberOfOperands())
	{
		assert(
			u->kind() == Kind::Integer, 
			"Polynomial needs to have"
			"integer coefficients in K[L...]"
		);

		return std::abs(u->value());
	}

	long long k = 0;

	AST *q, *p, *t, *e, *c, *n;
	
	n = degree(u, L->operand(i));
	
	p = algebraicExpand(u);

	for(long j = n->value(); j >= 0; j--)
	{
		e = integer(j);
	
		c = coeff(u, L->operand(i), e);
	
		k = std::abs(norm(c, L, K, i + 1)) + k;
	
		t = mul({c, power(L->operand(i)->copy(), e)});
	
		q = subPoly(p, t);

		delete p;
	
		p = algebraicExpand(q);	
	
		delete t;
	
		delete q;
	}

	delete p;

	delete n;

	return k;
}


long long l1norm(AST* u, AST* x)
{

	long long k = 0;

	AST *q, *p, *t, *e, *c, *n;
	
	n = degree(u, x);
	
	p = algebraicExpand(u);

	for(long j = n->value(); j >= 0; j--)
	{
		e = integer(j);
	
		c = coeff(u, x, e);

		assert(c->kind() == Kind::Integer, "coeffs needs to be integers");

		k = std::abs(c->value()) + k;
	
		t = mul({c, power(x->copy(), e)});
	
		q = subPoly(p, t);

		delete p;
	
		p = algebraicExpand(q);	
	
		delete t;
	
		delete q;
	}

	delete p;

	delete n;

	return k;
}

long long random(long long min, long long max)
{
	std::random_device dev;
	
	std::mt19937 rng(dev());
	
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
	
	return dist(rng);
}

}
