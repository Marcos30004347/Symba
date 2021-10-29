#include <assert.h>

#include "Core/Algebra/List.hpp"
#include "Core/Algebra/Set.hpp"
#include "Core/Algebra/Algebra.hpp"
#include "Core/Polynomial/Polynomial.hpp"
#include "Core/Polynomial/Factorization.hpp"
#include "Core/Factorization/Wang.hpp"
#include "Core/Simplification/Simplification.hpp"

using namespace ast;
using namespace algebra;
using namespace polynomial;
using namespace simplification;
using namespace factorization;

void should_get_nondivisors()
{
	AST* F = list({
		integer(-14),
		integer(3),
		integer(-11),
		integer(-17),
	});

	AST* L = list({
		symbol("y"),
		symbol("z"),
	});

	AST* K = symbol("Z");

	AST* d = nondivisors(4, F, 1, L, K);
	// assert(success == 1);

	assert(d->operand(0)->value() == 7);
	assert(d->operand(1)->value() == 3);
	assert(d->operand(2)->value() == 11);
	assert(d->operand(3)->value() == 17);

	delete F;
	delete L;
	delete K;
	delete d;
}

void shoud_get_ground_lead_coeff()
{
	AST* t = add({
		mul({
			add({
				mul({integer(2), power(symbol("y"), integer(2))}),
				mul({integer(3), power(symbol("y"), integer(1))}),
				integer(4)
			}),
			power(symbol("x"), integer(2))
		}),
		integer(5)
	});
	
	AST* L = list({ symbol("x"), symbol("y") });
	
	AST* lc = groundLeadCoeff(t, L);

	assert(lc->is(2));

	delete t;
	delete L;
	delete lc;
}

void should_factor_poly()
{
	AST* t = add({
		mul({
			power(symbol("x"), integer(2)),
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(2)),
		}),
		integer(-9)
	});
	
	AST* L = list({symbol("x"), symbol("y"), symbol("z")});

	AST* K = symbol("Z");

	AST* F0 = factors(t, L, K);

	delete t;
	delete L;
	delete K;
	delete F0;
}


void should_solve_diophant()
{
	AST* H1 = list({
		add({
			mul({integer(44), power(symbol("x"), integer(2))}),
			mul({integer(42), symbol("x")}),
			integer(1)
		}),
		add({
			mul({integer(126), power(symbol("x"), integer(2))}),
			mul({integer(-9), symbol("x")}),
			integer(28)
		}),
		add({
			mul({integer(187), power(symbol("x"), integer(2))}),
			integer(-23)
		}),
	});


	AST* H2 = list({
		add({
			mul({integer(-4), power(symbol("x"), integer(2)), symbol("y")}),
			mul({integer(-12), power(symbol("x"), integer(2))}),
			mul({integer(-3), symbol("x"), symbol("y")}),
			integer(1)
		}),
		add({
			mul({integer(-9), power(symbol("x"), integer(2)), symbol("y")}),
			mul({integer(-9), symbol("x")}),
			mul({integer(-2), symbol("y")}),
		}),
		add({
			mul({power(symbol("x"), integer(2)), power(symbol("y"), integer(2))}),
			mul({integer(-9), power(symbol("x"), integer(2))}),
			symbol("y"),
			integer(-9)
		}),
	}); 


	AST* H3 = list({
		add({
			mul({integer(-4), power(symbol("x"), integer(2)), symbol("y")}),
			mul({integer(-12), power(symbol("x"), integer(2))}),
			mul({integer(-3), symbol("x"), symbol("y")}),
			integer(1)
		}),
		add({
			mul({integer(-9), power(symbol("x"), integer(2)), symbol("y")}),
			mul({integer(-9), symbol("x")}),
			mul({integer(-2), symbol("y")}),
		}),
		add({
			mul({power(symbol("x"), integer(2)), power(symbol("y"), integer(2))}),
			mul({integer(-9), power(symbol("x"), integer(2))}),
			symbol("y"),
			integer(-9)
		}),
	}); 

	AST* c1 = add({
		mul({integer(-70686), power(symbol("x"), integer(5))}),
		mul({integer(-5863), power(symbol("x"), integer(4))}),
		mul({integer(-17826), power(symbol("x"), integer(3))}),
		mul({integer(2009), power(symbol("x"), integer(2))}),
		mul({integer(5031), symbol("x")}),
		integer(74)
	});

	AST* c2 = add({
		mul({integer(9), power(symbol("x"), integer(5)), power(symbol("y"), integer(4))}),
		mul({integer(12), power(symbol("x"), integer(5)), power(symbol("y"), integer(3))}),
		mul({integer(-45), power(symbol("x"), integer(5)), power(symbol("y"), integer(2))}),
		mul({integer(-108), power(symbol("x"), integer(5)), symbol("y")}),
		mul({integer(-324), power(symbol("x"), integer(5))}),
		mul({integer(18), power(symbol("x"), integer(4)), power(symbol("y"), integer(3))}),
		mul({integer(-216), power(symbol("x"), integer(4)), power(symbol("y"), integer(2))}),
		mul({integer(-810), power(symbol("x"), integer(4)), symbol("y")}),
		mul({integer(2), power(symbol("x"), integer(3)), power(symbol("y"), integer(4))}),
		mul({integer(9), power(symbol("x"), integer(3)), power(symbol("y"), integer(3))}),
		mul({integer(-252), power(symbol("x"), integer(3)), power(symbol("y"), integer(2))}),
		mul({integer(-288), power(symbol("x"), integer(3)), symbol("y")}),
		mul({integer(-945), power(symbol("x"), integer(3))}),
		mul({integer(-30), power(symbol("x"), integer(2)), power(symbol("y"), integer(2)) }),
		mul({integer(-414), power(symbol("x"), integer(2)), symbol("y") }),
		mul({integer(2), symbol("x"), power(symbol("y"), integer(3)) }),
		mul({integer(-54), symbol("x"), power(symbol("y"), integer(2)) }),
		mul({integer(-3), symbol("x"), symbol("y") }),
		mul({integer(81), symbol("x") }),
		mul({integer(12), symbol("y") }),
	});

	AST* c3 = add({
		mul({integer(-36), power(symbol("x"), integer(4)), power(symbol("y"), integer(2))}),
		mul({integer(-108), power(symbol("x"), integer(4)), symbol("y")}),
		mul({integer(-27), power(symbol("x"), integer(3)),  power(symbol("y"), integer(2))}),
		mul({integer(-36), power(symbol("x"), integer(3)),  symbol("y")}),
		mul({integer(-108), power(symbol("x"), integer(3))}),
		mul({integer(-8), power(symbol("x"), integer(2)), power(symbol("y"), integer(2))}),
		mul({integer(-42), power(symbol("x"), integer(2)), symbol("y")}),
		mul({integer(-6), symbol("x"), power(symbol("y"), integer(2))}),
		mul({integer(9), symbol("x")}),
		mul({integer(2), symbol("y")}),
	});

	AST* L1 = list({ symbol("x") });
	AST* I1 = list({});
 	AST* D1 = multivariateDiophant(H1, c1, L1, I1, 5, 6291469, 1);

	AST* R1 = list({
		mul({integer(-3), symbol("x")}),
		integer(-2),
		integer(1),
	});

	assert(D1->match(R1));

	AST* L2 = list({ symbol("x"), symbol("y") });
	AST* I2 = list({integer(-14)});
 	AST* D2 = multivariateDiophant(H2, c2, L2, I2, 5, 6291469, 1);
	
	AST* R2 = list({
		mul({integer(-1), symbol("x"), symbol("y")}),
		mul({integer(-3), symbol("x")}),
		integer(-6),
	});

	assert(D2->match(R2));

 	AST* D3 = multivariateDiophant(H3, c3, L2, I2, 5, 6291469, 1);
	
	AST* R3 = list({
		integer(0),
		integer(0),
		integer(-1),
	});

	assert(D3->match(R3));

	delete H1;
	delete H2;
	delete H3;
	delete c1;
	delete c2;
	delete c3;
	delete L1;
	delete L2;
	delete I1;
	delete I2;
	delete R1;
	delete R2;
	delete R3;
	delete D1;
	delete D2;
	delete D3;
}

void should_get_lead_coeffs()
{
	AST* f = add({
		mul({
			integer(4),
			power(symbol("x"), integer(6)), 
			power(symbol("y"), integer(4)), 
			power(symbol("z"), integer(2))
		}),
		mul({
			integer(4),
			power(symbol("x"), integer(6)), 
			power(symbol("y"), integer(3)), 
			power(symbol("z"), integer(3))
		}),
		mul({
			integer(-4),
			power(symbol("x"), integer(6)), 
			power(symbol("y"), integer(2)), 
			power(symbol("z"), integer(4))
		}),
		mul({
			integer(-4),
			power(symbol("x"), integer(6)), 
			symbol("y"),
			power(symbol("z"), integer(5))
		}),
		mul({
			power(symbol("x"), integer(5)), 
			power(symbol("y"), integer(4)), 
			power(symbol("z"), integer(3))
		}),
		mul({
			integer(12),
			power(symbol("x"), integer(5)), 
			power(symbol("y"), integer(3)),
			symbol("z") 
		}),
		mul({
			integer(-1),
			power(symbol("x"), integer(5)), 
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(5)),
		}),
		mul({
			integer(12),
			power(symbol("x"), integer(5)), 
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(2)),
		}),
		mul({
			integer(-12),
			power(symbol("x"), integer(5)), 
			symbol("y"),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(-12),
			power(symbol("x"), integer(5)), 
			power(symbol("z"), integer(4)),
		}),
		mul({
			integer(8),
			power(symbol("x"), integer(4)), 
			power(symbol("y"), integer(4)),
		}),
		mul({
			integer(6),
			power(symbol("x"), integer(4)), 
			power(symbol("y"), integer(3)), 
			power(symbol("z"), integer(2)),
		}),
		mul({
			integer(8),
			power(symbol("x"), integer(4)), 
			power(symbol("y"), integer(3)), 
			symbol("z")
		}),
		mul({
			integer(-4),
			power(symbol("x"), integer(4)), 
			power(symbol("y"), integer(2)), 
			power(symbol("z"), integer(4)), 
		}),
		mul({
			integer(4),
			power(symbol("x"), integer(4)), 
			power(symbol("y"), integer(2)), 
			power(symbol("z"), integer(3)), 
		}),
		mul({
			integer(-8),
			power(symbol("x"), integer(4)), 
			power(symbol("y"), integer(2)), 
			power(symbol("z"), integer(2)), 
		}),
		mul({
			integer(-4),
			power(symbol("x"), integer(4)), 
			symbol("y"),
			power(symbol("z"), integer(5)), 
		}),
		mul({
			integer(-2),
			power(symbol("x"), integer(4)), 
			symbol("y"),
			power(symbol("z"), integer(4)), 
		}),
		mul({
			integer(-8),
			power(symbol("x"), integer(4)), 
			symbol("y"),
			power(symbol("z"), integer(3)), 
		}),
		mul({
			integer(2),
			power(symbol("x"), integer(3)), 
			power(symbol("y"), integer(4)),
			symbol("z") 
		}),
		mul({
			power(symbol("x"), integer(3)), 
			power(symbol("y"), integer(3)),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(-1),
			power(symbol("x"), integer(3)), 
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(5)),
		}),
		mul({
			integer(-2),
			power(symbol("x"), integer(3)), 
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(9),
			power(symbol("x"), integer(3)), 
			power(symbol("y"), integer(2)),
			symbol("z")
		}),
		mul({
			integer(-12),
			power(symbol("x"), integer(3)), 
			symbol("y"),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(12),
			power(symbol("x"), integer(3)), 
			symbol("y"),
			power(symbol("z"), integer(2)),
		}),
		mul({
			integer(-12),
			power(symbol("x"), integer(3)), 
			power(symbol("z"), integer(4)),
		}),
		mul({
			integer(3),
			power(symbol("x"), integer(3)), 
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(6),
			power(symbol("x"), integer(2)), 
			power(symbol("y"), integer(3)),
		}),
		mul({
			integer(-6),
			power(symbol("x"), integer(2)), 
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(2)),
		}),
		mul({
			integer(8),
			power(symbol("x"), integer(2)), 
			power(symbol("y"), integer(2)),
			symbol("z")
		}),
		mul({
			integer(-2),
			power(symbol("x"), integer(2)), 
			symbol("y"),
			power(symbol("z"), integer(4)),
		}),
		mul({
			integer(-8),
			power(symbol("x"), integer(2)), 
			symbol("y"),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(2),
			power(symbol("x"), integer(2)), 
			symbol("y"),
			power(symbol("z"), integer(2)),
		}),
		mul({
			integer(2),
			symbol("x"),
			power(symbol("y"), integer(3)),
			symbol("z"),
		}),
		mul({
			integer(-2),
			symbol("x"),
			power(symbol("y"), integer(2)),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(-3),
			symbol("x"),
			symbol("y"),
			symbol("z"),
		}),
		mul({
			integer(3),
			symbol("x"),
			power(symbol("z"), integer(3)),
		}),
		mul({
			integer(-2),
			power(symbol("y"), integer(2)),
		}),
		mul({
			integer(2),
			symbol("y"),
			power(symbol("z"), integer(2)),
		}),
	});

	AST* K = symbol("Z");

	AST* L = list({symbol("x"), symbol("y"), symbol("z")});
	AST* a = list({integer(-14), integer(3)});
	AST* p = deepReplace(f, L->operand(1), a->operand(0));
	AST* t = deepReplace(p, L->operand(2), a->operand(1));
	AST* k = reduceAST(t);

	AST* d = cont(k, L->operand(0), K);
	AST* s = pp(k, d, L->operand(0), K);

	printf("delta = %s\n", d->toString().c_str());
	printf("primt = %s\n", s->toString().c_str());

	AST* F = list({
		list({symbol("y"), integer(1)}),
		list({symbol("z"), integer(2) }),
		list({add({symbol("y"), symbol("z")}), integer(2) }),
		list({add({symbol("y"), mul({integer(-1), symbol("z")})}), integer(1) }),
	});

	AST* sF = list({
		integer(-14),
		integer(3),
		integer(-11),
		integer(-17),
	});

	AST* u = list({
		add({
			mul({integer(42), power(symbol("x"), integer(2))}),
			mul({integer(42), symbol("x")}),
			integer(1)
		}),
		add({
			mul({integer(126), power(symbol("x"), integer(2))}),
			mul({integer(-9), symbol("x")}),
			integer(28)
		}),
		add({
			mul({integer(187), power(symbol("x"), integer(2))}),
			integer(-23)
		}),
	});

	printf("AAAAAA	\n");
	AST* sqf = sqfFactors(s, L->operand(0), K);
	printf("AAAAAA	\n");
	printf("sqffs = %s\n", sqf->toString().c_str());
	printf("sqffs = %s\n", u->toString().c_str());

	// assert sqf_factors(s, L->operand(0), K) == H

	AST* wlc = wangLeadingCoeff(u, d, u, F, sF, a, L, K);

	printf("lc = %s\n", wlc->toString().c_str());
}

int main()
{

	// should_get_nondivisors();
	// should_solve_diophant();
	should_get_lead_coeffs();
	return 0;
}
