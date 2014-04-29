#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <queue>

#include "token.h"
#include "tokenlist.h"

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
			MINUS,
			UNDERSCORE,
			PERIOD,
			DOUBLE_QUOTE,
			YARN_ESCAPE,
			YARN_NEWLINE,
			YARN_TAB,
			EXCLAMATION,
			QUESTION,
			SPACE,
			LINE_DELIMITER,
			UNKNOWN
		};

		static CharType GetCharType( char );
		static TokenType GetTokenType( const std::string & );

		void AddToken( Token );
		void AddSimpleToken( TokenType, bool = false );

	public:
		Tokenizer();

		bool Tokenize( std::string );

		inline TokenList &GetTokens() { return tokens_; }

	private:
		bool inMultiLineComment_;
		bool continueCurrentLine_;
		bool addLineDelimiter_;

		TokenList tokens_;
};

#endif
