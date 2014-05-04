#include <iostream>
#include <fstream>
#include <string>

#include "tokenizer.h"
#include "parser.h"
#include "codegen.h"

int main( int argc, char *argv[] ) {
	std::string srcPath( argv[1] );
	
	if( srcPath == "--help" ) {
		std::cout << "Usage: lol2cpp <file>" << std::endl << std::endl;
		std::cout << "After translation, compile generated C++ source using gcc "
				  << "with: -std=c++11 -O2" << std::endl;
		return 0;
	}

	Tokenizer tokenizer;
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
#endif

	Parser parser;
	AST::Program *program = parser.Parse( &tokenizer.GetTokens() );
	
	if( program == nullptr ) {
		std::cout << "Error: syntax error" << std::endl;
		return -1;
	}
	
	CodeGenerator codeGen;
	codeGen.Visit( program );
	
	std::ofstream cppOut( "a.cpp" );
	cppOut << codeGen.GetEmittedCode();
	
	return 0;
}
