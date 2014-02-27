#include <iostream>
#include <string>

#include "token.h"
#include "tokenizer.h"

int main() {
	Tokenizer tokenizer;
	std::string srcPath( "test/randtokens.lols" );

	tokenizer.Tokenize( srcPath );
	tokenizer.GetTokens().StartIterating();
	while( true ) {
		Token token = tokenizer.GetTokens().GetNextToken();
		Token nextToken = tokenizer.GetTokens().GetNextToken( 1 );
		std::cout << token << " " << nextToken << std::endl;
		if( token.type == TokenType::END_OF_FILE ) {
			break;
		}
		tokenizer.GetTokens().AdvanceToNextToken();
	}
	return 0;
}
