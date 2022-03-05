#ifndef PARSER_HPP
#define PARSER_HPP

#include "Expression.hpp"
#include "Lexer.hpp"

namespace alg {

class Parser {
private:
  Lexer lexer;

public:
  Parser(string src);

  void readToken(Token::kind tokenType);

  void readNumeric();

	bool isNumeric();

	Token getToken();



	inline Token currentToken() { return lexer.currentToken(); }

  expr parse();
};

} // namespace alg
#endif
