#include "Core/Polynomial/Polynomial.hpp"
#include "Core/Simplification/Simplification.hpp"
#include "Core/Debug/Assert.hpp"
#include "Resultant.hpp"

using namespace ast;
using namespace algebra;
using namespace simplification;

namespace polynomial {

Expr univariateResultant(Expr ux, Expr vx, Expr x)
{
	Expr m = degree(ux, x);
	Expr n = degree(vx, x);

	if(n== 0)
	{
		
	
		return power(vx, m);
	}

	Expr r = remainderGPE(ux, vx, x);
	
	if(r== 0)
	{
		
		
		

		return integer(0);
	}

	Expr s = degree(r, x);
	Expr l = coeff(vx, x, n);

	Expr k = mul({
		power(integer(-1), mul({m, n})),
		power(l, sub({m, s})),
		univariateResultant(vx, r, x)
	});

	Expr e = algebraicExpand(k);

	
	
	
	
	
	

	return e;
}

Expr multivariateResultant(Expr u, Expr v, Expr L, Expr K)
{
	Expr x = L[0];

	Expr m = degree(u, x);
	Expr n = degree(v, x);

	// if(m->isLessThan(n))
	if(m.value() < n.value())
	{
		Expr k = mul({
			power(integer(-1), mul({m, n})),
			multivariateResultant(v, u, L, K)
		});

		
		

		Expr t = algebraicExpand(k);

		
		

		return t;
	}

	if(n== 0)
	{
		Expr k =  power(v, m);
	
		
		
		
	
		return k;
	}

	Expr delta = add({
		m,
		mul({ integer(-1), n }),
		integer(1)
	});

	Expr r = pseudoRemainder(u, v, x);

	if(r== 0)
	{
		
		
	
		
		
		

		return integer(0);
	}

	Expr s = degree(r, x);

	Expr e = mul({
		power(integer(-1), mul({m, n})),
		multivariateResultant(v, r, L, K)
	});

	Expr w = algebraicExpand(e);
	
	

	Expr l = leadCoeff(v, x);

	Expr k = add({ mul({ delta, n }), mul({integer(-1), m}), s});

	Expr z = power(l, k);

	Expr f = algebraicExpand(z);

	

	Expr g = recQuotient(w, f, L, K);

	
	
	

	
	
	
	

	return g;
}


Expr srPolynomialResultantRec(Expr u, Expr v, Expr L, Expr K, Expr i, Expr delta_prev, Expr gamma_prev)
{
	assert(u!= 0, "Polynomial should be non-zero");
	assert(v!= 0, "Polynomial should be non-zero");

	Expr x = L[0];
	Expr m = degree(u, x);
	Expr n = degree(v, x);


	if(m.value() < n.value())
	{
		Expr e = mul({
			power(integer(-1), mul({m, n})),
			srPolynomialResultantRec(v, u, L, K, i, delta_prev, gamma_prev)
		});
	
	
		Expr k = reduceAST(e);

		
		
		
		
	
		return k;
	}

	if(n== 0)
	{
		Expr e = power(v, m);

		Expr k = reduceAST(e);

		
		
		
		

		return k;
	}

	Expr r = pseudoRemainder(u, v, x);

	if(r== 0)
	{
	
		
		
		
		
	
		return integer(0);
	}

	Expr delta = add({
		m,
		mul({integer(-1), n}),
		integer(1)
	});

	Expr R = rest(L);

	Expr gamma 	= undefined();
	Expr beta 	= undefined();

	if(i== 1)
	{
		gamma = integer(-1);

		Expr tmp = power(integer(-1), delta);

		beta = reduceAST(tmp);

		
	}
	else
	{
		Expr f = leadCoeff(u, x);
	
		Expr tmp1 = power(mul({integer(-1), f}), sub({delta_prev, integer(1)}));
		Expr tmp2 = power(gamma_prev, sub({delta_prev, integer(2)}));
	
		Expr tmp3 = algebraicExpand(tmp1);
		Expr tmp4 = algebraicExpand(tmp2);
		
		
		
		
		gamma = recQuotient(tmp3, tmp4, R, K);
	
		
		
	
		Expr tmp5 = mul({
			integer(-1),
			f,
			power(gamma, sub({ delta, integer(1) }))
		});
	
		beta = algebraicExpand(tmp5);

		

		
	}

	Expr t = recQuotient(r, beta, L, K);

	
	r = t;

	// Note: original algorithm didnt have this here,
	// but on testing there is a case where r = 0, 
	// so it will not be possible to run the 
	// srPolynomialResultantRec method with r
	// in the following lines.
	if(r== 0)
	{
		
		
		
		
		
		
		
		

		return integer(0);
	}

	Expr tmp1 = add({i, integer(1)});
	Expr tmp2 = reduceAST(tmp1);
	
	

	Expr tmp3 = mul({
		power(integer(-1), mul({ m, n })),
		power(beta, n),
		srPolynomialResultantRec(v, r, L, K, tmp2, delta, gamma)
	});

	

	Expr w = algebraicExpand(tmp3);

	

	Expr l = coeff(v, x, n);

	Expr s = degree(r, x);

	

	Expr k = add({
	 mul({ delta, n }),
	 mul({integer(-1), m}),
	 s
	});

	Expr tmp4 = power(l, k);

	
	

	Expr f = algebraicExpand(tmp4);

	

	Expr o = recQuotient(w, f, L, K);
	
	

	

	
	

	
	

	
	

	

	

	return o;
}

Expr srPolynomialResultant(Expr	u, Expr v, Expr L, Expr K)
{
	Expr x = L[0];
	// Expr R = rest(L);

	Expr m = degree(u, x);
	Expr n = degree(v, x);

	Expr cont_u = cont(u, L, K);
	Expr pp_u = recQuotient(u, cont_u, L, K);
	Expr cont_v = cont(v, L, K);
	Expr pp_v = recQuotient(v, cont_v, L, K);
	
	Expr i = integer(1);
	Expr delta = integer(0);
	Expr g = integer(0);

	Expr s = srPolynomialResultantRec(pp_u, pp_v, L, K, i, delta, g);

	
	
	

	Expr t = mul({
		power(cont_u, n),
		power(cont_v, m),
		s
	});

	Expr k = reduceAST(t);

	
	
	// 
	
	

	return k;
}


Expr polyRemSeqRec(Expr Gi2, Expr Gi1, Expr L, Expr hi2, Expr K)
{
	Expr Gi, hi1, di2, t1, t2, t3, t4, t5, t6, nk, cnt, ppk, r, x;

	x = L[0];

	if(Gi1== 0)
	{
		return list({ integer(1), integer(0) });
	}

	t4 = pseudoRemainder(Gi2, Gi1, x);

	// t4 = pdiv(Gi2, Gi1, x)[1];

	// printf("r = %s\n", t4->toString().c_str());

	if(t4== 0)
	{
		

		nk = degree(Gi1, x);

		if(nk.value() > 0)
		{
			// R = rest(L);
			
			cnt = cont(Gi1, L, K);
		
			ppk = recQuotient(Gi1, cnt, L, K);
			
			r = list({ ppk, integer(0) });

			
			// 
		}
		else
		{
			r = list({integer(1), Gi1});
		}		

		
	
		return r;
	}

	di2 = sub({ degree(Gi2, x), degree(Gi1, x) });

	t1 = add({ di2, integer(1) });
	t2 = power(integer(-1), t1);
	t3 = mul({t2, t4});
	t4  = algebraicExpand(t3);

	
	
	t1 = leadCoeff(Gi2, x);
	t2 = power(hi2, di2);
	t3 = mul({t1, t2});

	t5 = algebraicExpand(t3);

	

	Gi = recQuotient(t4, t5, L, K);

	
	

	t1 = leadCoeff(Gi1, x);
	t2 = power(t1, di2);

	t3 = hi2;
	t4 = sub({integer(1), di2});
	t5 = power(t3, t4);
	t6 = mulPoly(t2, t5);

	// t6 = mul({t2, t5});
	// hi1 = algebraicExpand(t6); // h4

	hi1 = reduceAST(t6); // h4

	
	
	

	// Expr hi, *di1, *gi, *Hi;
	// Expr _Gi = op[1];

	// di1 = sub({ degree(Gi1, x), degree(_Gi, x) });
	
	// t1 = leadCoeff(_Gi, x);
	// t2 = power(t1, di1); // gi^d[i-1]

	// t3 = sub({integer(1), di1});
	// t4 = power(hi1, t3);
	// t5 = mul({t2, t4});

	// hi = algebraicExpand(t5);

	// 

	// gi = leadCoeff(_Gi, x);
	
	// t1 = mul({hi, _Gi});
	// t2 = algebraicExpand(t1);


	// Hi = recQuotient(t2, gi, L, K);

	// printf("Hi = %s\n", Hi->toString().c_str());

	// 
	// 

	t3 = polyRemSeqRec(Gi1, Gi, L, hi1, K);

	
	
	

	return t3;
}

Expr polyRemSeq(Expr F1, Expr F2, Expr L, Expr K)
{

	if(F1.kind() == Kind::Integer && F2.kind() == Kind::Integer)
	{
		return integerGCD(F1, F2);
	}

	Expr x = L[0];
	Expr m = degree(F1, x);
	Expr n = degree(F2, x);
	
	if(m.value() < n.value())
	{
		
		

		return polyRemSeq(F2, F1, L, K);
	}
	
	// printf("f = %s\n", F1->toString().c_str());
	// printf("g = %s\n", F2->toString().c_str());
	
	

	Expr t1, t2, t3, t4, t5;
	Expr G1, G2, G3, h2, nk, ppk, cnt, r;

	G1 = F1;
	G2 = F2;

	// compute G[3]
	t1 = sub({ degree(F1, x), degree(F2, x) });
	t2 = add({ t1, integer(1) });

	t3 = power(integer(-1), t2);

	t4 = pseudoRemainder(G1, G2, x);

	// printf("r = %s\n", t4->toString().c_str());

	t5 = mulPoly(t3, t4); // mul({t3, t4});

	G3 = reduceAST(t5);

	

	if(G3== 0)
	{
		nk = degree(G2, x);

		if(nk.value() > 0)
		{
			// R = rest(L);
			cnt = cont(G2, L, K);
			ppk = recQuotient(G2, cnt, L, K);
		
			r = list({ppk, integer(0)});

			
			// 
		}
		else
		{
			r = list({integer(1), G2});
		}		

		
	
		return r;
	}

	// compute h[2]
	t1 = leadCoeff(G2, x);
	t2 = sub({ degree(F1, x), degree(F2, x) });
	t3 = power(t1, t2);
	h2 = reduceAST(t3);

	
	// printf("G3 %s\n", G3->toString().c_str());
	// printf("L %s\n", L->toString().c_str());
	t3 = polyRemSeqRec(G2, G3, L, h2, K);
	
	
	

	return t3;
}

}
