#include <iostream>
#include <string>

#include "token.h"
#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"
#include "codegen.h"

int main( int argc, char *argv[] ) {
	Tokenizer tokenizer;
	Parser parser;
	
	//std::string srcPath( "test/randtokens.lols" );
	//std::string srcPath( "test/large.lols" );
	std::string srcPath( "test/oddeven.lols" );
	//std::string srcPath( "test.lol" );
	//std::string srcPath( "test/99beers.lols" );
	//std::string srcPath( "test/empty.lols" );

	tokenizer.Tokenize( srcPath );
#if 0
	tokenizer.GetTokens().StartIterating();
	while( true ) {
		Token token = tokenizer.GetTokens().PeekToken();
		Token nextToken = tokenizer.GetTokens().PeekToken( 1 );
		std::cout << token << " " << nextToken << std::endl;
		if( token.type == TokenType::END_OF_FILE ) {
			break;
		}
		tokenizer.GetTokens().AdvanceToNextToken();
	}
	AST::Program *program = parser.Parse( &tokenizer.GetTokens() );
	if( program != nullptr ) {
		std::cout << *program << std::endl;
	} else {
		std::cout << "NULL" << std::endl;
	}
#else
	AST::Program *program = parser.Parse( &tokenizer.GetTokens() );
	if( program != nullptr ) {
		CodeGenerator codeGen;
		codeGen.Visit( program );
		std::cout << codeGen.GetEmittedCode() << std::endl;
	}
#endif
	return 0;
}
