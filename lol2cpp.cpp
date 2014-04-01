#include <iostream>
#include <string>

#include "token.h"
#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"

int main() {
	Tokenizer tokenizer;
	Parser parser;
	
	//std::string srcPath( "test/randtokens.lols" );
	std::string srcPath( "test/empty.lols" );

	tokenizer.Tokenize( srcPath );
#if 0
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
#else
	AST::Program *program = parser.Parse( &tokenizer.GetTokens() );
	std::cout << *program << std::endl;
	std::cout << (program != nullptr) << std::endl;
#endif
	return 0;
}
