#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <queue>

#include "token.h"

class Tokenizer {
	public:
		void Tokenize(std::string);
		Token GetNextToken() const;

	private:
		std::queue<Token> tokens_;
};

#endif
