#include "Lexer.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace alg;

Token::Token(Token::kind type, string value, unsigned line, unsigned pos)
    : value{value} {
  this->type = type;
  this->line = line;
  this->position = pos;
}

Lexer::Lexer(string source) : source{source}, current{Token::TOKEN_UNDEFINED, "", 0, 0} {
  this->head = 0;

  this->line = 1;

  this->character = source[this->head];

  this->eof = source.length();

  this->source_size = eof;
}

Token Lexer::collectNumberLiteral() {
  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  unsigned h = head;

  while (isdigit(this->character)) {
    value = (char *)realloc(value, (strlen(value) + 1) * sizeof(char));
    value[strlen(value)] = this->character;
    this->advance();
  }

  if (this->character == '.') {
    // Float literal
    value = (char *)realloc(value, (strlen(value) + 1) * sizeof(char));
    value[strlen(value)] = this->character;
    this->advance();

    while (isdigit(this->character)) {
      value = (char *)realloc(value, (strlen(value) + 1) * sizeof(char));
      value[strlen(value)] = this->character;
      this->advance();
    }
    if (this->character == 'f') {
      this->advance();
    }

    return Token(Token::TOKEN_FLOAT_LITERAL, value, this->line, h);
  } else {
    if (this->character == 'f') {
      this->advance();

      return Token(Token::TOKEN_FLOAT_LITERAL, value, this->line, h);
    }

    return Token(Token::TOKEN_INT_LITERAL, value, this->line, h);
  }
}

Token Lexer::collectIdentifier() {
  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  unsigned h = head;

  while (isalnum(this->character) || isdigit(this->character) ||
         this->character == '_') {
    value = (char *)realloc(value, (strlen(value) + 1) * sizeof(char));
    value[strlen(value)] = this->character;
    this->advance();
  }

	// if(strcmp(value, "mod") == 0) {
	// 	return Token(Token::TOKEN_MOD, value, this->line, h);
	// }

  return Token(Token::TOKEN_STRING_LITERAL, value, this->line, h);
}

bool Lexer::skipSpaces() {
  bool result = false;

  while (this->character == ' ' || this->character == 10) {
    if (this->character == 10)
      this->line++;
    result = true;
    this->advance();
  }

  return result;
}

void Lexer::advance() {
  this->head = this->head + 1;
  this->character = this->source[this->head];
}

Token Lexer::getToken() {
  if (this->character != '\0' || this->head < eof) {
    while (this->skipSpaces()) {
    }

    if (this->character == '.' && this->head + 1 < eof &&
        isdigit(this->source[this->head + 1]))
      return this->collectNumberLiteral();
    else if (isdigit(this->character))
      return this->collectNumberLiteral();
    else if (isalnum(this->character) || this->character == '_')
      return this->collectIdentifier();
    else if (this->character == '*') {
      Token r = Token(Token::TOKEN_TIMES, "*", this->line, head);
      this->advance();
      return r;
    } else if (this->character == '/') {
      Token r = Token(Token::TOKEN_DIV, "/", this->line, head);
      this->advance();
      return r;
    } else if (this->character == '-') {
      Token r = Token(Token::TOKEN_MINUS, "-", this->line, head);
      this->advance();
      return r;
    } else if (this->character == '+') {
      Token r = Token(Token::TOKEN_PLUS, "+", this->line, head);
      this->advance();
      return r;
    } else if (this->character == '(') {
      Token r = Token(Token::TOKEN_OPEN_PARENTESIS, "(", this->line, head);
      this->advance();
      return r;
    } else if (this->character == ')') {
      Token r = Token(Token::TOKEN_CLOSE_PARENTESIS, ")", this->line, head);
      this->advance();
      return r;
    } else if (this->character == '[') {
      Token r = Token(Token::TOKEN_OPEN_SQUARE_BRACKETS, "[", this->line, head);
      this->advance();
      return r;
    } else if (this->character == ']') {
      Token r =
          Token(Token::TOKEN_CLOSE_SQUARE_BRACKETS, "]", this->line, head);
      this->advance();
      return r;
    }
  }

  return Token(Token::TOKEN_EOF, "eof", line, head);
}
