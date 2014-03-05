#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <list>

#include "token.h"

class TokenList {
	public:
		TokenList();

		void AddToken( Token );

		inline void StartIterating() { it_ = tokens_.cbegin(); }
		inline void AdvanceToNextToken() {
			if( it_ != tokens_.cend() ) {
				++it_;
			}
		}
		void SkipLine();
		Token GetNextToken( unsigned int = 0 );

	private:
		std::list<Token> tokens_;
		std::list<Token>::const_iterator it_;
};

#endif
