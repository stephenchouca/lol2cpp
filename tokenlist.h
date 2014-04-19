#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <list>

#include "token.h"

class TokenList {
	public:
		TokenList() : it_( tokens_.cend() ) {}

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
			while( PeekToken().type != TokenType::LINE_DELIMITER && 
				   PeekToken().type != TokenType::END_OF_FILE ) {
				AdvanceToNextToken();
			}
		}
		inline void SkipToNextLine() {
			SkipToEOL();
			AdvanceToNextToken();
		}
		Token PeekToken( unsigned int = 0 );

	private:
		std::list<Token> tokens_;
		std::list<Token>::const_iterator it_;
};

#endif
