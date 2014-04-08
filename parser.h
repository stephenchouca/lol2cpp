#ifndef PARSER_H
#define PARSER_H

#include "tokenlist.h"
#include "token.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"

class Parser {
	public:
		Parser();

		AST::Program *Parse( TokenList * );

	private:
		AST::StatementBlock *ParseStatementBlock( const AST::StatementBlock::Type );

		AST::ORlyBlock *ParseORlyBlock();
		AST::WtfBlock *ParseWtfBlock();
		AST::LoopBlock *ParseLoopBlock();
		AST::FunkshunBlock *ParseFunkshunBlock();
		AST::PlzBlock *ParsePlzBlock();
		AST::Statement *ParseStatement();

		AST::VarDeclare *ParseVarDeclare();
		AST::VarAssign *ParseVarAssign( AST::Identifier * );
		AST::VarCast *ParseVarCast( AST::Identifier * );
		AST::VisibleStatement *ParseVisibleStatement();
		AST::GimmehStatement *ParseGimmehStatement();
		AST::FunkshunReturn *ParseFunkshunReturn();
		
		AST::Expression *ParseExpression();
		AST::FunkshunCall *ParseFunkshunCall();
		AST::CastExpression *ParseCastExpression();
		AST::SmooshExpression *ParseSmooshExpression();
		AST::UnaryExpression *ParseUnaryOperator();
		AST::BinaryExpression *ParseBinaryOperator();
		AST::NaryExpression *ParseNaryOperator();
		AST::Expression *ParseBukkitReference();
		
		AST::Literal *ParseLiteral();
		AST::TroofLiteral *ParseTroofLiteral();
		AST::NumbrLiteral *ParseNumbrLiteral();
		AST::NumbarLiteral *ParseNumbarLiteral();
		AST::YarnLiteral *ParseYarnLiteral();
		
		AST::Identifier *ParseIdentifier();
		AST::Identifier *ParseExplicitIdentifier();
		AST::Identifier *ParseLoopVarIdentifier();
		AST::LiteralIdentifier *ParseLiteralIdentifier();
		AST::SrsIdentifier *ParseSrsIdentifier();
		AST::ItIdentifier *ParseItIdentifier();
		AST::SlotIdentifier *ParseSlotIdentifier();
		AST::TypeIdentifier *ParseTypeIdentifier();
		
		bool CheckTokenAndAdvance( TokenType, Token * = nullptr );
	
	private:
		TokenList *tokens_;

		bool lineGood_;
};

#endif

