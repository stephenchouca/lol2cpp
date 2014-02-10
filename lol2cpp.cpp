#include <iostream>
#include <string>

#include "token.h"
#include "tokenizer.h"

int main() {
	Tokenizer tokenizer;
	std::string srcPath( "test/empty.lols" );

	tokenizer.Tokenize( srcPath );
	while( true ) {
		Token token = tokenizer.GetNextToken();
		if( token.type == TokenType::END_OF_FILE ) {
			break;
		}
		std::cout << token << std::endl;
	}
	return 0;
}
