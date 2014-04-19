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
		AST::ProgramBody *ParseProgramBody() {
			return dynamic_cast<AST::ProgramBody *>( 
				ParseStatementBlock( StatementBlockType::PROGRAM_BODY ) );
		}
		AST::ProgramGlobals *ParseProgramGlobals(){
			return dynamic_cast<AST::ProgramGlobals *>( 
				ParseStatementBlock( StatementBlockType::PROGRAM_GLOBALS ) );
		}
		AST::FunkshunBody *ParseFunkshunBody(){
			return dynamic_cast<AST::FunkshunBody *>( 
				ParseStatementBlock( StatementBlockType::FUNKSHUN_BODY ) );
		}
		AST::ORlyYaBody *ParseORlyYaBody(){
			return dynamic_cast<AST::ORlyYaBody *>( 
				ParseStatementBlock( StatementBlockType::ORLY_YA ) );
		}
		AST::ORlyMebbeBody *ParseORlyMebbeBody(){
			return dynamic_cast<AST::ORlyMebbeBody *>( 
				ParseStatementBlock( StatementBlockType::ORLY_MEBBE ) );
		}
		AST::ORlyNoBody *ParseORlyNoBody(){
			return dynamic_cast<AST::ORlyNoBody *>( 
				ParseStatementBlock( StatementBlockType::ORLY_NO ) );
		}
		AST::WtfOmgBody *ParseWtfOmgBody(){
			return dynamic_cast<AST::WtfOmgBody *>( 
				ParseStatementBlock( StatementBlockType::WTF_OMG ) );
		}
		AST::WtfOmgwtfBody *ParseWtfOmgwtfBody(){
			return dynamic_cast<AST::WtfOmgwtfBody *>( 
				ParseStatementBlock( StatementBlockType::WTF_OMGWTF ) );
		}
		AST::LoopBody *ParseLoopBody(){
			return dynamic_cast<AST::LoopBody *>( 
				ParseStatementBlock( StatementBlockType::LOOP_BODY ) );
		}
		AST::PlzBody *ParsePlzBody(){
			return dynamic_cast<AST::PlzBody *>( 
				ParseStatementBlock( StatementBlockType::PLZ_BODY ) );
		}
		AST::PlzONoesBody *ParsePlzONoesBody(){
			return dynamic_cast<AST::PlzONoesBody *>( 
				ParseStatementBlock( StatementBlockType::PLZ_ONOES ) );
		}
		AST::PlzOWelBody *ParsePlzOWelBody(){
			return dynamic_cast<AST::PlzOWelBody *>( 
				ParseStatementBlock( StatementBlockType::PLZ_OWEL ) );
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
		
		bool CheckTokenAndAdvance( TokenType, Token * = nullptr );
	
	private:
		TokenList *tokens_;
		
		AST::FunkshunList funkshuns_;

		bool lineGood_;
};

#endif

