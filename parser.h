#ifndef PARSER_H
#define PARSER_H

#include "tokenlist.h"
#include "token.h"
#include "ast.h"

class Parser {
	public:
		Parser();

		AST::Program *Parse( TokenList * );

	private:
		AST::StatementBlock *ParseStatementBlock();

		AST::ORlyBlock *ParseORlyBlock();
		AST::WtfBlock *ParseWtfBlock();
		AST::LoopBlock *ParseLoopBlock();
		AST::FunkshunBlock *ParseFunkshunBlock();
		AST::PlzBlock *ParsePlzBlock();
		AST::Statement *ParseStatement();

		AST::VarDeclare *ParseVarDeclare();
		AST::VarAssign *ParseVarAssign();

		AST::TypeIdentifier *ParseTypeIdentifier();
		bool ParseFunkshunCall();
		AST::Expression *ParseExpression();
		AST::Literal *ParseLiteral();
		AST::LiteralIdentifier *ParseLiteralIdentifier();
		
		bool CheckTokenAndAdvance( TokenType, Token * = nullptr );
	
	private:
		TokenList *tokens_;

		bool lineGood_;
};

#endif

