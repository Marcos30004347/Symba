#include "Subtraction.hpp"
#include "Addition.hpp"
#include "Multiplication.hpp"

#include "Core/Expand/Expand.hpp"

using namespace ast;
using namespace algebra;

namespace simplification {

// negate[sub[add[a, b, c], sub[d, e]]] 						-> add[a + b + c + -d + -e]
// negate[sub[add[a, b, c], sub[d, e], sub[f, g]]]  -> add[a + b + c + -d + -e + -f + -g]
// negate[sub[sub[a, b, c], sub[d, e], sub[f, g]]]	-> add[a + -b + -c -d + e + -f + g]
Expr subRec(Expr u)
{
	Expr v = nullptr;

	if(u[0].kind() == Kind::Subtraction)
	{
		v = subRec(u[0]);
	}
	else if(u[0].kind() == Kind::Addition)
	{
		v = reduceAdditionAST(u[0]);
	}
	else
	{
		v = u[0];
	}

	Expr r = add({});

	for(unsigned int j = 1; j < u.size(); j++)
	{
		if(u[j].kind() == Kind::Subtraction)
		{
			r.insert(subRec(u[j]));
		}
		else if(u[j].kind() == Kind::Addition)
		{
			r.insert(reduceAdditionAST(u[j]));
		}
		else
		{
			r.insert(u[j]);
		}
	}

	if(r.size() > 0 && v.kind() != Kind::Addition)
	{
		v = add({ v });
	}

	for(unsigned int j = 0; j < r.size(); j++)
	{
		Expr rj = r[j];

		if(rj.kind() == Kind::Addition)
		{
			for(unsigned int i = 0; i < rj.size(); i++)
			{
				v.insert(reduceMultiplicationAST(-1 * rj[i]));
			}
		}
		else
		{
			v.insert(reduceMultiplicationAST(-1 * rj));
		}
	}

	return v;
}

Expr reduceSubtractionAST(Expr u) 
{
	if(u.kind() != Kind::Subtraction)
	{
		return u;
	}

	return reduceAdditionAST(subRec(u));
}

}
