#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <queue>

#include "token.h"
	
class Tokenizer {
	private:
		enum class TokenizeState {
			START,
			READ_LINE_CONTINUE,
			READ_KEYWORD_OR_IDENT,
			READ_YARN_LITERAL,
			READ_NUMBR_LITERAL,
			READ_NUMBAR_LITERAL
		};

		enum class CharType {
			LETTER,
			DIGIT,
			UNDERSCORE,
			PERIOD,
			DOUBLE_QUOTE,
			YARN_ESCAPE,
			YARN_NEWLINE,
			YARN_TAB,
			EXCLAMATION,
			SPACE,
			LINE_DELIMITER,
			UNKNOWN
		};

		static CharType GetCharType(char);
		static TokenType GetTokenType(std::string);

		void AddToken(Token);
		void AddSimpleToken(TokenType, bool);

	public:
		Tokenizer();

		bool Tokenize(std::string);

		Token GetNextToken();

	private:
		std::queue<Token> tokens_;
		bool inMultiLineComment_;
		bool continueCurrentLine_;
		bool addLineDelimiter_;
};

#endif
