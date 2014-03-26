#include "tokenlist.h"
#include "token.h"

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
