#include "Types.hpp"
#include "MathSystem/Factorization/Berlekamp.hpp"
#include "TypeSystem/SymbolTable.hpp"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#define BUCKET_SIZE_LOG2 7
#define CONTEXT_BUCKET_SIZE_LOG2 5

#define BUCKET_SIZE ((key)1 << BUCKET_SIZE_LOG2)
#define CONTEXT_BUCKET_SIZE ((key)1 << BUCKET_SIZE_LOG2)

#define TERM_INDEX_MASK (((key)1 << BUCKET_SIZE_LOG2) - 1)
#define TERM_BUCKET_MASK ~TERM_INDEX_MASK

#define CONTEXT_INDEX_MASK (((key)1 << CONTEXT_BUCKET_SIZE_LOG2) - 1)
#define CONTEXT_BUCKET_MASK ~CONTEXT_INDEX_MASK

#define build_key(bucket, index) ((key)index | ((key)bucket << BUCKET_SIZE_LOG2))
#define build_ctx_key(bucket, index) ((key)index | ((key)bucket << CONTEXT_BUCKET_SIZE_LOG2))

#define key_bucket(k) ((k & TERM_BUCKET_MASK) >> BUCKET_SIZE_LOG2)
#define key_index(k) (k & TERM_INDEX_MASK)

#define ctx_key_bucket(k) ((k & CONTEXT_BUCKET_MASK) >> CONTEXT_BUCKET_SIZE_LOG2)
#define ctx_key_index(k) (k & CONTEXT_INDEX_MASK)

#define min(a, b) a > b ? b : a

inline static size_t ceil(size_t x, size_t y) {
  return x == 0 ? 0 : 1 + ((x - 1) / y);
}

/*
 * @brief Context structure, it links declarations terms to types terms.
 * Internally all data is stored lenarly on byckets of BUCKET_SIZE size.
 */
struct context {
	// List of buckets, each bucket holds BUCKET_SIZE keys to storage terms
	term_ref** types;
	// List of buckets, each bucket holds BUCKET_SIZE keys to storage terms.
	term_ref** terms;

	// Total number of bindings on this context
	size_t size;

	// key to the previous context on the storage
	ctx_ref prev_ctx;

};

struct storage {
  // Bucket list of terms, each bucket have BUCKET_SIZE terms
  // contiguous in memory.
  term **terms;

	// Bucket list of keys, each bucket have BUCKET_SIZE keys
	// contiguous in memory, each term 't' have a 'mem_key' that
	// is a key to a element on this bucket list, the first
	// element 'n' is the number of members on the term 't', and
	// the following 'n' keys are keys of the members of 't', what
	// the keys points to are kind dependent, for instance, it could
	// be a term on 'terms' bucket list, or a key in the 'symbols' for
	// a identifier.
  term_ref **childs;

	context** contexts;

	size_t ctx_count;

  size_t t_count;
  size_t c_count;

  symbol_registry *symbols;

  ctx_ref root_ctx = {};

  term_ref term_kind_key = {};
  term_ref term_unsp_key = {};
};


ctx_ref ctx_ref_from_key(key idx) {
	ctx_ref ref = {};
	ref.value = idx;
	return ref;
}

term_ref term_ref_from_key(key idx) {
	term_ref ref = {};
	ref.value = idx;
	return ref;
}

members_ref members_ref_from_key(key idx) {
	members_ref ref = {};
	ref.value = idx;
	return ref;
}

ctx_ref storage_new_empty_context(storage* strg, ctx_ref prev) {
	bool inc = strg->ctx_count > 0 && ((strg->ctx_count % CONTEXT_BUCKET_SIZE) == 0);

	if(inc) {
		context** old_contexts = strg->contexts;

		strg->contexts = new context*[1 + ceil(strg->ctx_count, CONTEXT_BUCKET_SIZE)];

		for(size_t i = 0; i <= (strg->ctx_count / CONTEXT_BUCKET_SIZE); i++) {
			strg->contexts[i] = old_contexts[i];
		}

		delete[] old_contexts;
	}

	key b = strg->ctx_count / CONTEXT_BUCKET_SIZE;
	key i = strg->ctx_count % CONTEXT_BUCKET_SIZE;

	context* ctx = &strg->contexts[b][i];

	ctx->size = 0;

	ctx->terms = new term_ref*[1];
  ctx->terms[0] = new term_ref[CONTEXT_BUCKET_SIZE];

	ctx->types = new term_ref*[1];
  ctx->types[0] = new term_ref[CONTEXT_BUCKET_SIZE];

	ctx->prev_ctx = prev;

	strg->ctx_count = strg->ctx_count + 1;

	ctx_ref ref = {};

	ref.value = build_ctx_key(b, i);

	return ref;
}

context* storage_get_context(storage* s, ctx_ref ctx_key) {
	return &s->contexts[ctx_key_bucket(ctx_key.value)][ctx_key_index(ctx_key.value)];
}

binding_ref context_add_binding(storage* strg, ctx_ref ctx_key, term_ref term_key, term_ref type_key) {
	// TODO: check if term_key is already inserted on the current ctx

	context* ctx = storage_get_context(strg, ctx_key);

	bool inc = ctx->size > 0 && ((ctx->size % CONTEXT_BUCKET_SIZE) == 0);

	if(inc) {
		term_ref** old_terms = ctx->terms;
		term_ref** old_types = ctx->types;

		ctx->terms = new term_ref*[1 + ceil(ctx->size, CONTEXT_BUCKET_SIZE)];
		ctx->types = new term_ref*[1 + ceil(ctx->size, CONTEXT_BUCKET_SIZE)];

		for(size_t i = 0; i < 1 + ceil(ctx->size, CONTEXT_BUCKET_SIZE); i++) {
			ctx->terms[i] = old_terms[i];
			ctx->types[i] = old_types[i];
		}

		delete[] old_terms;
		delete[] old_types;
	}

	key b = ctx->size / CONTEXT_BUCKET_SIZE;
	key i = ctx->size % CONTEXT_BUCKET_SIZE;

	ctx->terms[b][i] = term_key;
	ctx->types[b][i] = type_key;

	ctx->size = ctx->size + 1;

	binding_ref ref;

	ref.value = build_ctx_key(b, i);

	return ref;
}

void context_destroy(context* ctx) {
	for(size_t i = 0; i <= ctx->size / CONTEXT_BUCKET_SIZE; i++) {
		delete[] ctx->types[i];
		delete[] ctx->terms[i];
	}

	delete[] ctx->types;
	delete[] ctx->terms;
}

term_ref context_get_binding_type_key(storage* strg, ctx_ref ctx_key, term_ref decl) {
	if(ctx_key.value == INVALID_KEY) {
		return term_ref_from_key(INVALID_KEY);
	}

	context* ctx = storage_get_context(strg, ctx_key);

	long long i = ctx->size / CONTEXT_BUCKET_SIZE;

	for(; i >= 0; i--) {
		long long j = CONTEXT_BUCKET_SIZE - 1;

		for(; j >= 0; j--) {
			if(ctx->terms[i][j].value == decl.value) {
				return ctx->types[i][j];
			}
		}
	}

	return context_get_binding_type_key(strg, ctx->prev_ctx, decl);
}

storage *storage_create() {
  storage *strg = new storage();

  strg->terms = new term *[1];
  strg->terms[0] = new term[BUCKET_SIZE];

  strg->childs = new term_ref *[1];
  strg->childs[0] = new term_ref[BUCKET_SIZE];

  strg->contexts = new context*[1];
  strg->contexts[0] = new context[CONTEXT_BUCKET_SIZE];

  strg->t_count = 0;
  strg->c_count = 0;
	strg->ctx_count = 0;

  strg->symbols = symbol_registry_create();

	strg->root_ctx = storage_new_empty_context(strg, ctx_ref_from_key(INVALID_KEY));

	// Here we add all the base types and terms to the root context.

	// Add Unspecified Type, this is used on declaration that types are not
	// specified directly, it will be the responsability of the type checker
	// to infer this type.
	strg->term_unsp_key = storage_insert(strg, strg->root_ctx, term::TYPE_UNS, 0, 0);

	// Add Any Type, that is the kind of Types.
  strg->term_kind_key = storage_insert(strg, strg->root_ctx, term::TYPE_ANY, 0, 0);

  return strg;
}

void storage_destroy(storage *strg) {
  for (size_t i = 0; i <= strg->t_count / BUCKET_SIZE; i++) {
    delete[] strg->terms[i];
  }

  for (size_t i = 0; i <= strg->c_count / BUCKET_SIZE; i++) {
    delete[] strg->childs[i];
  }

	for (size_t i = 0; i <= strg->ctx_count / CONTEXT_BUCKET_SIZE; i++) {
		size_t bs = min(CONTEXT_BUCKET_SIZE, strg->ctx_count);

		for(size_t j = 0; j < bs; j++) {
		  context_destroy(&strg->contexts[i][j]);
		}

		strg->ctx_count = strg->ctx_count - bs;

		delete[] strg->contexts[i];
  }

  delete[] strg->terms;
  delete[] strg->childs;
  delete[] strg->contexts;

  symbol_registry_destroy(strg->symbols);

	delete strg;
}

name_ref storage_name_create(storage *strg, const char *id) {
  name_ref n;

  n.value = symbol_registry_set_entry(strg->symbols, id);

  return n;
}

term_ref storage_insert(storage *strg, ctx_ref ctx, term::kind kind, term_ref *childs, size_t count) {
  bool inc = (strg->t_count > 0) && ((strg->t_count % BUCKET_SIZE) == 0);

	if (inc) {
		// increase strg->terms
    size_t n = 1 + ceil(strg->t_count, BUCKET_SIZE);

		term **old_terms = strg->terms;

    strg->terms = new term *[n];

    size_t i = 0;

    for (; i < n - 1; i++) {
      strg->terms[i] = old_terms[i];
    }

		strg->terms[n - 1] = new term[BUCKET_SIZE];

    delete[] old_terms;
  }

	key t_b = strg->t_count / BUCKET_SIZE;
  key t_i = strg->t_count % BUCKET_SIZE;

  strg->terms[t_b][t_i].term_kind = kind;
	strg->terms[t_b][t_i].ctx_key = ctx;

	inc = (strg->c_count / BUCKET_SIZE) < ((strg->c_count + count + 1) / BUCKET_SIZE);

	if (inc) {
    // increase strg->childs
		size_t q = 1 + (strg->c_count / BUCKET_SIZE);
		size_t k = 1 + ceil((strg->c_count + count + 1) / BUCKET_SIZE);

		// size_t n = ceil(strg->c_count + count + 1, BUCKET_SIZE);
    term_ref **old_childs = strg->childs;

    strg->childs = new term_ref *[k];

    size_t i = 0;

    for (; i < q; i++) {
      strg->childs[i] = old_childs[i];
    }

    for (; i < k; i++) {
      strg->childs[i] = new term_ref[BUCKET_SIZE];
    }

    delete[] old_childs;
  }

	key c_b = strg->c_count / BUCKET_SIZE;
	key c_i = strg->c_count % BUCKET_SIZE;

  strg->childs[c_b][c_i] = term_ref_from_key(count);

	for (size_t i = 0; i < count; i++) {
	 	size_t b = (c_i + 1 + i) / BUCKET_SIZE;
	 	strg->childs[c_b + b][(c_i + 1 + i) % BUCKET_SIZE] = childs[i];
	}

  strg->c_count = strg->c_count + count + 1;
  strg->t_count = strg->t_count + 1;

  strg->terms[t_b][t_i].mem_key = members_ref_from_key(build_key(c_b, c_i));

  return term_ref_from_key(build_key(t_b, t_i));
}

term_ref storage_get_child_key(storage *strg, term_ref trm, size_t i) {
	term& t = storage_get_term_from_key(strg, trm);

  members_ref c_key = t.mem_key;

  size_t bck = key_bucket(c_key.value);
  size_t idx = key_index(c_key.value);

  assert(strg->childs[bck][idx].value > i /* out of bounds */);

  idx += 1;

  bck += (idx + i) / BUCKET_SIZE;

  return strg->childs[bck][(idx + i) % BUCKET_SIZE];
}

term_ref storage_get_child_key(storage *strg, term &t, size_t i) {
  members_ref c_key = t.mem_key;

  size_t bck = key_bucket(c_key.value);

  size_t idx = key_index(c_key.value);

	assert(strg->childs[bck][idx].value > i);

  idx += 1;

  bck += (idx + i) / BUCKET_SIZE;

  return strg->childs[bck][(idx + i) % BUCKET_SIZE];
}

term &storage_get_term_from_key(storage *strg, term_ref idx) {
  return strg->terms[key_bucket(idx.value)][key_index(idx.value)];
}

term &storage_get_child_term(storage *strg, term &t, size_t i) {
  return storage_get_term_from_key(strg, storage_get_child_key(strg, t, i));
}

void print_term_rec(storage *strg, term_ref term_key) {
	term &t = storage_get_term_from_key(strg, term_key);

	ctx_ref ctx = t.ctx_key;

	term_ref app_func, app_argu;

	term_ref arg_name, arg_type;

	term_ref fun_name, fun_argu, fun_body, fun_type;

	term_ref lam_argu, lam_body;

	term_ref var_name, var_type, var_valu;

	term_ref arr_arg0, arr_arg1;

	term_ref sym_symb;

	switch (t.term_kind) {

	case term::TERM_APP:
		app_func = storage_get_child_key(strg, t, 0);
	  app_argu = storage_get_child_key(strg, t, 1);

		print_term(strg, app_func);

    printf(" ");

    print_term(strg, app_argu);

    break;

	case term::TERM_ARG:
		arg_name = storage_get_child_key(strg, t, 0);
    arg_type = context_get_binding_type_key(strg, ctx, arg_name);

		print_term(strg, arg_name);
		printf(" : ");
		print_term(strg, arg_type);

		break;

	case term::TERM_FUN:
    fun_name = storage_get_child_key(strg, t, 0);
    fun_argu = storage_get_child_key(strg, t, 1);
    fun_body = storage_get_child_key(strg, t, 2);

    fun_type = context_get_binding_type_key(strg, ctx, fun_name);

    printf("func ");

    print_term(strg, fun_name);

    printf(" ( ");

    print_term(strg, fun_argu);

    printf(" ) ");

    printf(" = ( ");
    print_term(strg, fun_body);
    printf(" ) ");

    printf(" : ");

    print_term(strg, fun_type);

    printf(";");

		break;

	case term::TERM_LAM:
		lam_argu = storage_get_child_key(strg, t, 0);
		lam_body = storage_get_child_key(strg, t, 1);

		printf("λ");

    print_term(strg, lam_argu);

		printf(".");

    print_term(strg, lam_body);

    printf(";");

		break;

  case term::TERM_VAR:
		var_name = storage_get_child_key(strg, t, 0);
		var_valu = storage_get_child_key(strg, t, 1);
		var_type = context_get_binding_type_key(strg, ctx, term_key);

		printf("let ");

    print_term(strg, var_name);

		printf(" : ");

		print_term(strg, var_type);

		printf(" = ");

    print_term(strg, var_valu);

    printf(";");

    break;

	case term::TERM_SYM:
		sym_symb = storage_get_child_key(strg, t, 0);

    printf("%s", symbol_registry_get_symbol(strg->symbols, sym_symb.value));

		break;

	case term::TYPE_SYM:
    print_term(strg, storage_get_child_key(strg, t, 0));
    break;

	case term::TYPE_ANY:
    printf("Any");
    break;

	case term::TYPE_UNS:
    printf("?");
    break;

	case term::TYPE_ARR:
		arr_arg0 = storage_get_child_key(strg, t, 0);
		arr_arg1 = storage_get_child_key(strg, t, 1);

		printf("(");
    print_term(strg, arr_arg0);
		printf(" -> ");
    print_term(strg, arr_arg1);
    printf(")");

    break;
  }
}

void print_term(storage *strg, term_ref term) {
	print_term_rec(strg, term);
}

ctx_ref root_context(storage *strg) { return strg->root_ctx; }

term_ref type_kind(storage *strg) { return strg->term_kind_key; }

term_ref type_unspecified(storage *strg) { return strg->term_unsp_key; }

term_ref term_symbol(storage *strg, ctx_ref ctx, const char *id) {
  name_ref n = storage_name_create(strg, id);

  term_ref c[1] = { term_ref_from_key(n.value) };

  return storage_insert(strg, ctx, term::TERM_SYM, c, 1);
}

term_ref term_var_decl(storage *strg, ctx_ref ctx, const char *id, term_ref value, term_ref type) {

  term_ref sym = term_symbol(strg, ctx, id);
  term_ref c[] = { sym, value };

	term_ref var = storage_insert(strg, ctx, term::TERM_VAR, c, 2);

	context_add_binding(strg, ctx, sym, type);

	return var;
}


// key term_lambda_decl(storage *strg, key ctx, key arg_symbol, key arg_type, key body, key body_type) {}

// key term_function_decl(storage *strg, key ctx, key arg_symbol, key arg_type, key body, key body_type) {
// 	key found = context_get_binding_type_key(strg, ctx, arg_type);

// 	if(found == INVALID_KEY) {
// 		// TODO: handle error because type is not defined
// 		exit(1);
// 	}

// 	key c[2] = { arg_symbol, body };

// 	key func_ctx = storage_new_empty_context(strg, ctx);

// 	context_add_binding(strg, func_ctx, arg_symbol, arg_type);

// 	return storage_insert(strg, ctx, term::TERM_FUN, c, 2);
// }

term_ref term_call(storage *strg, ctx_ref ctx, term_ref fun, term_ref arg) {
  term_ref c[2] = {fun, arg};

  return storage_insert(strg, ctx, term::TERM_APP, c, 2);
}

// void create_call() {}
