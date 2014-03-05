#include "tokenlist.h"
#include "token.h"

TokenList::TokenList() : 
	it_( tokens_.cend() ) {}

void TokenList::AddToken( Token newToken ) {
	if( newToken.type == TokenType::LINE_DELIMITER && !tokens_.empty() && 
		tokens_.back().type == TokenType::LINE_DELIMITER ) {
		return;
	}

	tokens_.push_back( newToken );
}

void TokenList::SkipLine() {
	while( GetNextToken().type != TokenType::LINE_DELIMITER && 
		   GetNextToken().type != TokenType::END_OF_FILE ) {
		AdvanceToNextToken();
	}
}

Token TokenList::GetNextToken( unsigned int k ) {
	std::list<Token>::const_iterator it = it_;

	for( unsigned int i = 0; i < k && it != tokens_.cend(); ++i, ++it ) {}

	if( it == tokens_.cend() ) {
		Token ret;
		ret.type = TokenType::END_OF_FILE;
		return ret;
	}

	return *it;
}
