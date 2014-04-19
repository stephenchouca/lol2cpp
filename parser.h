#ifndef PARSER_H
#define PARSER_H

#include "tokenlist.h"
#include "token.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"

class Parser {
	public:
		Parser() : tokens_( nullptr ), lineGood_( true ) {}

		AST::Program *Parse( TokenList * );

	private:
		enum class StatementBlockType { 
			PROGRAM_BODY,
			PROGRAM_GLOBALS,
			FUNKSHUN_BODY,
			ORLY_YA,
			ORLY_MEBBE,
			ORLY_NO,
			WTF_OMG,
			WTF_OMGWTF,
			LOOP_BODY,
			PLZ_BODY,
			PLZ_ONOES,
			PLZ_OWEL
		};
			
		AST::StatementBlock *ParseStatementBlock( const StatementBlockType );
		AST::StatementBlock *ParseProgramBody() {
			ParseStatementBlock( StatementBlockType::PROGRAM_BODY );
		}
		AST::StatementBlock *ParseProgramGlobals(){
			ParseStatementBlock( StatementBlockType::PROGRAM_GLOBALS );
		}
		AST::StatementBlock *ParseFunkshunBody(){
			ParseStatementBlock( StatementBlockType::FUNKSHUN_BODY );
		}
		AST::StatementBlock *ParseORlyYaBody(){
			ParseStatementBlock( StatementBlockType::ORLY_YA );
		}
		AST::StatementBlock *ParseORlyMebbeBody(){
			ParseStatementBlock( StatementBlockType::ORLY_MEBBE );
		}
		AST::StatementBlock *ParseORlyNoBody(){
			ParseStatementBlock( StatementBlockType::ORLY_NO );
		}
		AST::StatementBlock *ParseWtfOmgBody(){
			ParseStatementBlock( StatementBlockType::WTF_OMG );
		}
		AST::StatementBlock *ParseWtfOmgwtfBody(){
			ParseStatementBlock( StatementBlockType::WTF_OMGWTF );
		}
		AST::StatementBlock *ParseLoopBody(){
			ParseStatementBlock( StatementBlockType::LOOP_BODY );
		}
		AST::StatementBlock *ParsePlzBody(){
			ParseStatementBlock( StatementBlockType::PLZ_BODY );
		}
		AST::StatementBlock *ParsePlzONoesBody(){
			ParseStatementBlock( StatementBlockType::PLZ_ONOES );
		}
		AST::StatementBlock *ParsePlzOWelBody(){
			ParseStatementBlock( StatementBlockType::PLZ_OWEL );
		}

		AST::ORlyBlock *ParseORlyBlock();
		AST::WtfBlock *ParseWtfBlock();
		AST::LoopBlock *ParseLoopBlock();
		AST::FunkshunBlock *ParseFunkshunBlock();
		AST::PlzBlock *ParsePlzBlock();
		AST::Statement *ParseStatement();

		AST::VarDeclare *ParseVarDeclare();
		AST::VarAssign *ParseVarAssign( AST::Identifier * );
		AST::VarAssign *ParseVarCast( AST::Identifier * );
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
		
		bool AcceptToken( TokenType, Token * = nullptr );
	
	private:
		TokenList *tokens_;
		
		AST::FunkshunList funkshuns_;

		bool lineGood_;
};

#endif

