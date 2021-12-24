#include "Core/AST/AST.hpp"
#include "Core/Algebra/Algebra.hpp"
#include "Utils.hpp"
#include "Hensel.hpp"
#include "Berlekamp.hpp"
#include "Zassenhaus.hpp"
#include "SquareFree.hpp"

#include "Core/Algebra/List.hpp"
#include "Core/Algebra/Set.hpp"
#include "Core/Debug/Assert.hpp"
#include "Core/Primes/Primes.hpp"
#include "Core/Polynomial/Polynomial.hpp"
#include "Core/Simplification/Simplification.hpp"
#include "Core/Calculus/Calculus.hpp"
#include "Core/GaloisField/GaloisField.hpp"

#include <cmath>

using namespace ast;
using namespace algebra;
using namespace calculus;
using namespace polynomial;
using namespace galoisField;
using namespace simplification;

namespace factorization {

void subsetsRec(Expr& arr, Expr& data, Expr& s, Int start, Int end, Int index, Int r)
{
	Expr c;

	long i, j;

	if (index == r)
	{
		s.insert(set({}));

		c = s[s.size() - 1];

		for(j = 0; j < r; j++)
		{
			c.insert(data[j]);
		}
	}
	else
	{
		for (i = start.longValue(); i <= end && end.longValue() - i + 1 >= r - index; i++)
		{
			data.insert(arr[i]);
			subsetsRec(arr, data, s, i+1, end, index+1, r);
			data.remove(data.size() - 1);
		}
	}
}

Expr subset(Expr s, Int r)
{
	long n = s.size();

	Expr d, res;

	d = set({});

	res = set({});

	subsetsRec(s, d, res, 0, n - 1, 0, r);

	return res;
}

// from Algorithms for Computer Algebra Geddes
Expr cantorZassenhausDDF(Expr ax, Expr x, Int p)
{
	long i;

	Expr wx, t, G, gx, n;

	printf("%s\n", ax.toString().c_str());

	i = 1;

	wx = x;

	G = list({});

	gx = 1;

	n = degree(ax, x);

	while(n != -inf() && n.value() >= 2*i)
	{

		printf("p = %s\n", p.to_string().c_str());
		printf("w(x) = %s\n", wx.toString().c_str());
		printf("f(x) = %s\n", ax.toString().c_str());
		wx = powModPolyGf(wx, ax, x, p, p, true);
		printf("t(x) = %s\n", wx.toString().c_str());

		t = subPolyGf(wx, x, x, p, true);

		printf("%s\n", t.toString().c_str());
		printf("%s\n", ax.toString().c_str());

		gx = gcdPolyGf(ax, t, x, p, true);

		printf("gx = %s\n", gx.toString().c_str());
		if(gx != 1)
		{
			G.insert(list({ gx, i }));

			ax = quoPolyGf(ax, gx, x, p, true);

			t = remPolyGf(wx, ax, x, p, true);

			wx = t;
		}

		n = degree(ax, x);

		i = i + 1;
	};

	if(ax != 1)
	{
		G.insert(list({ ax, degree(ax, x) }));
	}

	return G;
}

	int tabs = 0;

// from Algorithms for Computer Algebra Geddes
Expr cantorZassenhausEDF(Expr a, Expr x, Int n, Int p)
{
	Int m, i;

	Expr g, da, F, v, h, k, f1, f2, t;
	tabs += 2;

	da = degree(a, x);

	//assert(da.kind() == Kind::Integer, "only integer degrees allowed");

	if(da.value() <= n)
	{
		tabs -= 2;

		return list({ a });
	}

	m = da.value() / n;

	F = list({ a });

	while(F.size() < m) {
		v = randPolyGf(2*n - 1, x, p);
		printf("d = %s\n", (2*n - 1).to_string().c_str());
		printf("v(x) = %s\n", v.toString().c_str());

		if(p == 2)
		{
			h = v;
			for(i = 0; i < pow(2, n * m - 1); i++)
			{
				h = powModPolyGf(h, a, x, 2, p, true);
				v = addPolyGf(v, h, x, p, true);
			}
		}
		else
		{
			//TODO: remove
			v = x + 4;
			printf("deg = %s\n", ((pow(p, n) - 1)/2).to_string().c_str());
			printf("v(x) = %s\n", v.toString().c_str());
			printf("a(x) = %s\n", a.toString().c_str());

			v = powModPolyGf(v, a, x, (pow(p, n) - 1) / 2, p, true);
			printf("b(x) = %s\n", v.toString().c_str());
			v = subPolyGf(v, 1, x, p, true);
			printf("--> a(x) = %s\n", a.toString().c_str());
			printf("--> v(x) = %s\n", v.toString().c_str());
				g = gcdPolyGf(a, v, x, p, true);
			printf("g(x) = %s\n", g.toString().c_str());
			abort();
		}

		g = gcdPolyGf(a, v, x, p, true);

		if(g != 1 && g != a)
		{
			k = quoPolyGf(a, g, x, p, true);


			f1 = cantorZassenhausEDF(g, x, n, p);
			f2 = cantorZassenhausEDF(k, x, n, p);

			F = append(f1, f2);

		}
	}

	tabs -= 2;

	return F;
}

Expr cantorZassenhaus(Expr u, Expr x, Int m)
{
	// [lc, f]= monicGf(f, p)
	// if  deg(f) < 1: return [lc, []]

	Expr F = cantorZassenhausDDF(u, x, m);

	Expr f = list({});
	printf("%s\n", F.toString().c_str());
	for(long i = 0; i < F.size(); i++)
	{
		Expr k = F[i][0];

		Int n = F[i][1].value();

		Expr T = cantorZassenhausEDF(k, x, n, m);

		for(Int i = 0; i < T.size(); i++) {
			f.insert(T[i]);
		}
	}

	return f;
}


Expr squareFreeFactoringGf(Expr u, Expr x, Int m)
{
	Expr T = monicPolyGf(u, x, m, false);

	Expr lc = T[0];
	Expr f = T[1];

	Expr n = degree(f, x);

	if(n.value() == 1)
	{
		return list({lc, list({})});
	}

	Expr F = cantorZassenhaus(f, x, m);

	return list({lc, F});
}

// From modern computer algebra by Gathen
Expr zassenhaus(Expr f, Expr x)
{
	bool stop = false;

	Int s, i, j, l, p, A, B, C, gamma, gcd;

	Expr g, n, b, F, D, E, H, Z, G, T, S, M, u, v, gi, L, K, I;

	L = list({ x });

	K = Expr("Z");

	n = degree(f, x);

	if(n == 1)
	{
		return list({ f });
	}

	A = norm(f, x);

	b = leadCoeff(f, x);

	B = Int(std::abs(std::sqrt(n.value().longValue() + 1))) * Int(pow(2, n.value())) * A * b.value();

	C = pow(n.value() + 1, 2 * n.value()) * pow(A, 2 * n.value() - 1);

	gamma = std::ceil(2 * (2 * n.value().longValue() * log2(n.value().longValue()+1) + (2 * n.value().longValue() - 1) * log2(A.longValue())));

	// choose a prime number p such that f be square free in Zp[x]
	// and such that p dont divide lc(f)

	for(i = 1; primes[i.longValue()] <= 2 * gamma.longValue() * std::log(gamma.longValue()); i++)
	{
		p = primes[i.longValue()];

		if(b.value() % p == 0)
		{
			continue;
		}

		F = gf(f, p, true);

		D = derivate(F, x);

		E = gf(D, p, true);

		D = gcdPolyGf(F, E, x, p, false);

		gcd = D.value();

		if(b.value() % p > 0 && gcd == 1)
		{
			break;
		}
	}

	l = std::ceil(std::log(2*B.longValue() + 1) / std::log(p.longValue()));

	I = squareFreeFactoringGf(f, x, p);

	Z = I[1];

	I.remove(1);

	g = multifactorHenselLifting(f, Z, x, p, l);

	T = set({});

	for(i = 0; i < g.size(); i++)
	{
		T.insert(integer(i));
	}

	F = list({});

	s = 1;

	while(2*s <= T.size())
	{
		stop = false;

		M = subset(T, s);

		for(j = 0; j < M.size(); j++)
		{
			S = M[j];

			H = mul({b});
			G = mul({b});

			Z = difference(T, S);

			for(i = 0; i < S.size(); i++)
			{
				gi = g[S[i].value()];

				G.insert(gi);
			}

			for(i = 0; i < Z.size(); i++)
			{
				gi = g[Z[i].value()];

				H.insert(gi);
			}

			u = gf(G, pow(p, l), true);
			v = gf(H, pow(p, l), true);

			G = u;
			H = v;

			if(norm(G, x) > pow(p, l) / 2)
			{
				continue;
			}

			if(norm(H, x) > pow(p, l) / 2)
			{
				continue;
			}

			if(l1norm(G, x) * l1norm(H, x) <= B)
			{
				T = Z;

				F.insert(pp(G, L, K));

				f = pp(H, L, K);

				b = leadCoeff(f, x);

				stop = true;
			}

			if(stop) break;
		}

		if(!stop) s = s + 1;
	}

	F.insert(f);

	return F;
}


}
