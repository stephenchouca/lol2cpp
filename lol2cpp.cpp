#include <iostream>
#include <string>

#include "token.h"
#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"
#include "loopinc_operand_setter.h"
#include "implicit_assigner.h"
#include "codegen.h"

int main() {
	Tokenizer tokenizer;
	Parser parser;
	
	//std::string srcPath( "test/randtokens.lols" );
	std::string srcPath( "test/large.lols" );
	//std::string srcPath( "test/99beers.lols" );
	//std::string srcPath( "test/empty.lols" );

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
	
	if( program != nullptr ) {
		std::cout << *program << std::endl;
	} else {
		std::cout << "NULL" << std::endl;
	}
	std::cout << "----------------------------------------------" << std::endl;
	//GlobalsHoister globalsHoister;
	//globalsHoister.Visit( program );
	//std::cout << *program << std::endl;
	//std::cout << "----------------------------------------------" << std::endl;
	/*LoopIncOperandSetter incOpSetter;
	incOpSetter.Visit( program );
	std::cout << *program << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	ImplicitAssigner assigner;
	assigner.Visit( program );
	std::cout << *program << std::endl;
	std::cout << "----------------------------------------------" << std::endl;*/
	CodeGenerator codeGen;
	codeGen.Visit( program );
	std::cout << codeGen.GetEmittedCode() << std::endl;
#endif
	return 0;
}
