#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <list>

#include "token.h"

class TokenList {
	public:
		TokenList();

		inline void AddToken( Token newToken ) {
			if( newToken.type == TokenType::LINE_DELIMITER && !tokens_.empty() && 
				tokens_.back().type == TokenType::LINE_DELIMITER ) {
				return;
			}

			tokens_.push_back( newToken );
		}

		inline void StartIterating() { it_ = tokens_.cbegin(); }
		inline void AdvanceToNextToken( unsigned int k = 1 ) {
			while( k > 0 && it_ != tokens_.cend() ) {
				++it_;
				--k;
			}
		}
		inline void SkipToEOL() {
			while( GetNextToken().type != TokenType::LINE_DELIMITER && 
				   GetNextToken().type != TokenType::END_OF_FILE ) {
				AdvanceToNextToken();
			}
		}
		Token GetNextToken( unsigned int = 0 );

	private:
		std::list<Token> tokens_;
		std::list<Token>::const_iterator it_;
};

#endif
