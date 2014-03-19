#include <cassert>

#include "parser.h"
#include "tokenlist.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"

Parser::Parser() : 
	tokens_( nullptr ),
	lineGood_( true ) {}

AST::Program *Parser::Parse( TokenList *tokens ) {
	tokens_ = tokens;
	lineGood_ = true;
	tokens_->StartIterating();

	CheckTokenAndAdvance( TokenType::HAI );
	CheckTokenAndAdvance( TokenType::LINE_DELIMITER );

	AST::StatementBlock *stmtBlock = ParseStatementBlock();
	if( stmtBlock == nullptr ) {
		return nullptr;
	}
	AST::Program *program = new AST::Program();

	CheckTokenAndAdvance( TokenType::KTHXBYE );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ||
		!CheckTokenAndAdvance( TokenType::END_OF_FILE ) ) {
		delete program;
		return nullptr;
	}

	return program;
}

AST::StatementBlock *Parser::ParseStatementBlock() {
	AST::StatementBlock *stmtBlock = new AST::StatementBlock();
	AST::Statement *stmt = nullptr;

	while( true ) {
		switch( tokens_->GetNextToken().type ) {
			case TokenType::KTHXBYE:
			case TokenType::END_OF_FILE:
				return stmtBlock;
			case TokenType::O:
				stmt = ParseORlyBlock();
				break;
			case TokenType::WTF:
				stmt = ParseWtfBlock();
				break;
			case TokenType::IM:
				stmt = ParseLoopBlock();
				break;
			case TokenType::PLZ:
				stmt = ParsePlzBlock();
				break;
			default:
				stmt = ParseStatement();
				break;
		}
		if( stmt == nullptr ) {
			delete stmtBlock;
			stmtBlock = nullptr;
		}
		if( stmtBlock == nullptr ) {
			continue;
		}
		// TODO: Add statement to block.
	}
}

AST::StatementBlock *Parser::ParseProgramStatementBlock() {
	AST::StatementBlock *stmtBlock = new AST::StatementBlock();
	AST::Statement *stmt = nullptr;

	while( true ) {
		switch( tokens_->GetNextToken().type ) {
			case TokenType::KTHXBYE:
			case TokenType::END_OF_FILE:
				return stmtBlock;
			case TokenType::O:
				stmt = ParseORlyBlock();
				break;
			case TokenType::WTF:
				stmt = ParseWtfBlock();
				break;
			case TokenType::IM:
				stmt = ParseLoopBlock();
				break;
			case TokenType::HOW:
				stmt = ParseFunkshunBlock();
				break;
			case TokenType::PLZ:
				stmt = ParsePlzBlock();
				break;
			default:
				stmt = ParseStatement();
				break;
		}
		if( stmt == nullptr ) {
			delete stmtBlock;
			stmtBlock = nullptr;
		}
		if( stmtBlock == nullptr ) {
			continue;
		}
		// TODO: Add statement to block.
	}
}

AST::ORlyBlock *Parser::ParseORlyBlock() {
	CheckTokenAndAdvance( TokenType::O );
	CheckTokenAndAdvance( TokenType::RLY );
	CheckTokenAndAdvance( TokenType::QUESTION );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		return nullptr;
	}

	CheckTokenAndAdvance( TokenType::YA );
	CheckTokenAndAdvance( TokenType::RLY );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		return nullptr;
	}

	AST::StatementBlock *stmtBlock = ParseStatementBlock();
	if( stmtBlock == nullptr ) {
		return nullptr;
	}
	AST::ORlyBlock *oRlyBlock = new AST::ORlyBlock();

	while( tokens_->GetNextToken().type == TokenType::MEEBE ) {
		tokens_->AdvanceToNextToken();
		AST::Expression *cond = ParseExpression();
		if( cond == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			delete oRlyBlock;
			return nullptr;
		}
		stmtBlock = ParseStatementBlock();
		if( stmtBlock == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
	}

	if( tokens_->GetNextToken().type == TokenType::NO ) {
		tokens_->AdvanceToNextToken();
		CheckTokenAndAdvance( TokenType::WAI );
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			delete oRlyBlock;
			return nullptr;
		}

		stmtBlock = ParseStatementBlock();
		if( stmtBlock == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
	}

	CheckTokenAndAdvance( TokenType::OIC );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete oRlyBlock;
		return nullptr;
	}

	return oRlyBlock;
}

AST::WtfBlock *Parser::ParseWtfBlock() {
	CheckTokenAndAdvance( TokenType::WTF );
	CheckTokenAndAdvance( TokenType::QUESTION );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		return nullptr;
	}

	Token curToken = tokens_->GetNextToken();
	if( curToken.type != TokenType::OMG ) {
		return nullptr;
	}
	AST::WtfBlock *wtfBlock = new AST::WtfBlock();

	do {
		tokens_->AdvanceToNextToken();
		AST::Literal *literal = ParseLiteral();
		if( literal == nullptr ) {
			delete wtfBlock;
			return nullptr;
		}
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			delete wtfBlock;
			return nullptr;
		}

		AST::StatementBlock *stmtBlock = ParseStatementBlock();
		if( stmtBlock == nullptr ) {
			delete wtfBlock;
			return nullptr;
		}

		tokens_->AdvanceToNextToken();
		curToken = tokens_->GetNextToken();
	} while( curToken.type == TokenType::OMG );

	if( tokens_->GetNextToken().type == TokenType::OMGWTF ) {
		tokens_->AdvanceToNextToken();
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			delete wtfBlock;
			return nullptr;
		}

		AST::StatementBlock *stmtBlock = ParseStatementBlock();
		if( stmtBlock == nullptr ) {
			delete wtfBlock;
			return nullptr;
		}
	}
	
	CheckTokenAndAdvance( TokenType::OIC );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete wtfBlock;
		return nullptr;
	}

	return wtfBlock;
}

AST::LoopBlock *Parser::ParseLoopBlock() {
	CheckTokenAndAdvance( TokenType::IM );
	CheckTokenAndAdvance( TokenType::IN );
	if( !CheckTokenAndAdvance( TokenType::YR ) ) {
		return nullptr;
	}
	
	AST::LiteralIdentifier *id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	
	AST::LoopBlock *loopBlock = nullptr;
	switch( tokens_->GetNextToken().type ) {
		case TokenType::LINE_DELIMITER:
			loopBlock = new AST::ForLoopBlock();
			break;
		case TokenType::WATCHIN:
			loopBlock = new AST::RangeLoopBlock();
			
			if( !CheckTokenAndAdvance( TokenType::YR ) ) {
				delete loopBlock;
				return nullptr;
			}
			
			id = ParseLiteralIdentifier();
			if( id == nullptr ) {
				delete loopBlock;
				return nullptr;
			}
			
			break;
		default:
		{
			loopBlock = new AST::ForLoopBlock();
			
			AST::UnaryExpression *incExpr = nullptr;
			AST::FunkshunCall *funkExpr = nullptr;
			switch( tokens_->GetNextToken().type ) {
				case TokenType::UPPIN:
					incExpr = new AST::UppinExpression();
					tokens_->AdvanceToNextToken();
					break;
				case TokenType::NERFIN:
					incExpr = new AST::NerfinExpression();
					tokens_->AdvanceToNextToken();
					break;
				default:
				{
					incExpr = ParseUnaryOperator();
					if( incExpr != nullptr ) {
						break;
					}
					
					Token token;
					if( !CheckTokenAndAdvance( TokenType::IDENTIFIER, &token ) ) {
						delete loopBlock;
						return nullptr;
					}
					funkExpr = new AST::FunkshunCall();
					break;
				}
			}
			assert( ( incExpr != nullptr ) ^ ( funkExpr != nullptr ) );
			
			if( !CheckTokenAndAdvance( TokenType::YR ) ) {
				delete loopBlock;
				return nullptr;
			}
			AST::Identifier *inductVar = ParseInductionIdentifier();
			if( inductVar == nullptr ) {
				delete loopBlock;
				return nullptr;
			}
			
			if( tokens_->GetNextToken().type == TokenType::FROM ) {
				tokens_->AdvanceToNextToken();
				AST::Expression *expr = ParseExpression();
				if( expr == nullptr ) {
					delete loopBlock;
					return nullptr;
				}
			}
			
			AST::UnaryExpression *testExpr = nullptr;
			switch( tokens_->GetNextToken().type ) {
				case TokenType::TIL:
					testExpr = new AST::TilExpression();
					break;
				case TokenType::WILE:
					testExpr = new AST::WileExpression();
					break;
				default:
					break;
			}
			if( testExpr != nullptr ) {
				tokens_->AdvanceToNextToken();
				AST::Expression *expr = ParseExpression();
				if( expr == nullptr ) {
					delete loopBlock;
					return nullptr;
				}
			}
			
			break;
		}
	}
	
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete loopBlock;
		return nullptr;
	}
	
	AST::StatementBlock *stmtBlock = ParseStatementBlock();
	if( stmtBlock == nullptr ) {
		delete loopBlock;
		return nullptr;
	}
	
	CheckTokenAndAdvance( TokenType::IM );
	CheckTokenAndAdvance( TokenType::OUTTA );
	if( !CheckTokenAndAdvance( TokenType::YR ) ) {
		delete loopBlock;
		return nullptr;
	}
	
	id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		// TODO: Should also check for match with loop begin identifier.
		delete loopBlock;
		return nullptr;
	}
	
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete loopBlock;
		return nullptr;
	}

	return loopBlock;
}

AST::FunkshunBlock *Parser::ParseFunkshunBlock() {
	CheckTokenAndAdvance( TokenType::HOW );
	CheckTokenAndAdvance( TokenType::IZ );
	if( !CheckTokenAndAdvance( TokenType::I ) ) {
		return nullptr;
	}

	AST::LiteralIdentifier *id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	AST::FunkshunBlock *funkshun = new AST::FunkshunBlock();

	if( tokens_->GetNextToken().type == TokenType::YR ) {
		tokens_->AdvanceToNextToken();
		id = ParseLiteralIdentifier();
		if( id == nullptr ) {
			delete funkshun;
			return nullptr;
		}
		while( tokens_->GetNextToken().type == TokenType::AN ) {
			tokens_->AdvanceToNextToken();
			if( !CheckTokenAndAdvance( TokenType::YR ) ) {
				delete funkshun;
				return nullptr;
			}
			id = ParseLiteralIdentifier();
			if( id == nullptr ) {
				delete funkshun;
				return nullptr;
			}
		}
	}

	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete funkshun;
		return nullptr;
	}

	AST::StatementBlock *stmtBlock = ParseStatementBlock();
	if( stmtBlock == nullptr ) {
		delete funkshun;
		return nullptr;
	}

	CheckTokenAndAdvance( TokenType::IF );
	CheckTokenAndAdvance( TokenType::U );
	CheckTokenAndAdvance( TokenType::SAY );
	CheckTokenAndAdvance( TokenType::SO );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete funkshun;
		return nullptr;
	}

	return funkshun;
}

AST::PlzBlock *Parser::ParsePlzBlock() {
	CheckTokenAndAdvance( TokenType::PLZ );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		return nullptr;
	}

	AST::StatementBlock *stmtBlock = ParseStatementBlock();
	if( stmtBlock == nullptr ) {
		return nullptr;
	}
	AST::PlzBlock *plzBlock = new AST::PlzBlock();

	while( tokens_->GetNextToken().type == TokenType::O && 
		   tokens_->GetNextToken( 1 ).type == TokenType::NOES ) {
		tokens_->AdvanceToNextToken();
		tokens_->AdvanceToNextToken();

		AST::Expression *expr = ParseExpression();
		if( expr == nullptr ) {
			delete plzBlock;
			return nullptr;
		}

		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			delete plzBlock;
			return nullptr;
		}

		stmtBlock = ParseStatementBlock();
		if( stmtBlock == nullptr ) {
			delete plzBlock;
			return nullptr;
		}
	}

	if( tokens_->GetNextToken().type == TokenType::O ) {
		CheckTokenAndAdvance( TokenType::WELL );
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			delete plzBlock;
			return nullptr;
		}

		stmtBlock = ParseStatementBlock();
		if( stmtBlock == nullptr ) {
			delete plzBlock;
			return nullptr;
		}
	}

	CheckTokenAndAdvance( TokenType::KTHX );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete plzBlock;
		return nullptr;
	}

	return plzBlock;
}

AST::Statement *Parser::ParseStatement() {
	AST::Statement *stmt = nullptr;

	switch( tokens_->GetNextToken().type ) {
		case TokenType::I:
			stmt = ParseVarDeclare();
			break;
		case TokenType::FOUND:
			stmt = ParseFunkshunReturn();
			break;
		case TokenType::VISIBLE:
			stmt = ParseVisibleStatement();
			break;
		case TokenType::GIMMEH:
			stmt = ParseGimmehStatement();
			break;
		case TokenType::GTFO:
			tokens_->AdvanceToNextToken();
			stmt = new AST::GtfoStatement();
			break;
		case TokenType::IDENTIFIER:
		case TokenType::SRS:
		case TokenType::IT:
		case TokenType::SLOT:
		{
			AST::Identifier *id = ParseIdentifier();
			if( id == nullptr ) {
				return nullptr;
			}
			
			switch( tokens_->GetNextToken().type ) {
				case TokenType::R:
					stmt = ParseVarAssign(id);
					break;
				case TokenType::IS:
					stmt = ParseVarCast(id);
					break;
				default:
					stmt = id;
					break;
			}
			break;
		}
		default:
			stmt = ParseExpression();
			break;
	}
	if( stmt == nullptr ) {
		return nullptr;
	}

	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete stmt;
		return nullptr;
	}

	return stmt;
}

AST::VarDeclare *Parser::ParseVarDeclare() {
	CheckTokenAndAdvance( TokenType::I );
	CheckTokenAndAdvance( TokenType::HAS );
	if( !CheckTokenAndAdvance( TokenType::A ) ) {
		// TODO: Fix similar issue elsewhere.
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Identifier *id = ParseExplicitIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	AST::VarDeclare *varDecl = new AST::VarDeclare();

	if( tokens_->GetNextToken().type == TokenType::ITZ ) {
		tokens_->AdvanceToNextToken();
		if( tokens_->GetNextToken().type == TokenType::A ) {
			tokens_->AdvanceToNextToken();
			AST::TypeIdentifier *type = ParseTypeIdentifier();
			if( type == nullptr ) {
				delete varDecl;
				return nullptr;
			}
		} else {
			AST::Expression *expr = ParseExpression();
			if( expr == nullptr ) {
				delete varDecl;
				return nullptr;
			}
		}
	}

	return varDecl;
}

AST::VarAssign *Parser::ParseVarAssign( AST::Identifier *id ) {
	AST::VarAssign *varAssign = new AST::VarAssign();
	
	if( !CheckTokenAndAdvance( TokenType::R ) ) {
		delete varAssign;
		return nullptr;
	}

	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		delete varAssign;
		return nullptr;
	}
	
	return varAssign;
}

AST::VarCast *Parser::ParseVarCast( AST::Identifier *id ) {
	AST::VarCast *varCast = new AST::VarCast();
	
	CheckTokenAndAdvance( TokenType::IS );
	CheckTokenAndAdvance( TokenType::NOW );
	if( !CheckTokenAndAdvance( TokenType::A ) ) {
		delete varCast;
		return nullptr;
	}
	
	AST::TypeIdentifier *type = ParseTypeIdentifier();
	if( type == nullptr ) {
		delete varCast;
		return nullptr;
	}
	
	return varCast;
}

AST::VisibleStatement *Parser::ParseVisibleStatement() {
	if( !CheckTokenAndAdvance( TokenType::VISIBLE ) ) {
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	AST::VisibleStatement *visibleStmt = new AST::VisibleStatement();
	
	while( true ) {
		expr = ParseExpression();
		if( expr == nullptr ) {
			break;
		}
		// TODO: Add expr to visibleStmt.
	}
	
	return visibleStmt;
}

AST::GimmehStatement *Parser::ParseGimmehStatement() {
	if( !CheckTokenAndAdvance( TokenType::GIMMEH ) ) {
		return nullptr;
	}
	
	AST::Identifier *id = ParseIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	
	return new AST::GimmehStatement();
}

AST::FunkshunReturn *Parser::ParseFunkshunReturn() {
	CheckTokenAndAdvance( TokenType::FOUND );
	if( !CheckTokenAndAdvance( TokenType::YR ) ) {
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	
	return new AST::FunkshunReturn();
}

AST::Expression *Parser::ParseExpression() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::IDENTIFIER:
		case TokenType::SRS:
		case TokenType::IT:
		case TokenType::SLOT:
			return ParseIdentifier();
		case TokenType::WIN:
		case TokenType::FAIL:
		case TokenType::YARN_LITERAL:
		case TokenType::NUMBR_LITERAL:
		case TokenType::NUMBAR_LITERAL:
			return ParseLiteral();
		case TokenType::I:
			return ParseFunkshunCall();
		case TokenType::MAEK:
			return ParseCastExpression();
		case TokenType::SMOOSH:
			return ParseSmooshExpression();
		case TokenType::NOOB:
			tokens_->AdvanceToNextToken();
			return new AST::NoobExpression();
		default:
			break;
	}
	
	AST::Expression *expr = ParseUnaryOperator();
	if( expr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete expr;
			return nullptr;
		}
		// TODO: Add operand to expr.
		return expr;
	}
	
	expr = ParseBinaryOperator();
	if( expr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete expr;
			return nullptr;
		}
		// TODO: Add operand to expr.
		
		if( !CheckTokenAndAdvance( TokenType::AN ) ) {
			delete expr;
			return nullptr;
		}
		
		operand = ParseExpression();
		if( operand == nullptr ) {
			delete expr;
			return nullptr;
		}
		// TODO: Add operand to expr.
		return expr;
	}
	
	expr = ParseNaryOperator();
	if( expr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete expr;
			return nullptr;
		}
		// TODO: Add operand to expr.
		
		while( true ) {
			if( tokens_->GetNextToken().type == TokenType::MKAY ) {
				tokens_->AdvanceToNextToken();
				break;
			} else if( tokens_->GetNextToken().type == TokenType::LINE_DELIMITER ) {
				break;
			}
			
			if( !CheckTokenAndAdvance( TokenType::AN ) ) {
				delete expr;
				return nullptr;
			}
			
			operand = ParseExpression();
			if( operand == nullptr ) {
				delete expr;
				return nullptr;
			}
			// TODO: Add operand to expr.
		}
		
		return expr;
	}
	
	// TODO: Output error message.
	tokens_->SkipToEOL();
	return nullptr;
}

AST::UnaryExpression *Parser::ParseUnaryOperator() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::NOT:
			tokens_->AdvanceToNextToken();
			return new AST::NotExpression();
		default:
			break;
	}
	
	tokens_->SkipToEOL();
	return nullptr;
}

AST::BinaryExpression *Parser::ParseBinaryOperator() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::SUM:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::SumExpression();
			}
			break;
		case TokenType::DIFF:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::DiffExpression();
			}
			break;
		case TokenType::PRODUKT:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::ProduktExpression();
			}
			break;
		case TokenType::QUOSHUNT:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::QuoshuntExpression();
			}
			break;
		case TokenType::MOD:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::ModExpression();
			}
			break;
		case TokenType::BIGGR:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::BiggrExpression();
			}
			break;
		case TokenType::SMALLR:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::SmallrExpression();
			}
			break;
		case TokenType::BOTH:
			switch( tokens_->GetNextToken( 1 ).type ) {
				case TokenType::OF:
					tokens_->AdvanceToNextToken( 2 );
					return new AST::BothExpression();
				case TokenType::SAEM:
					tokens_->AdvanceToNextToken( 2 );
					return new AST::SaemExpression();
				default:
					break;
			}
			break;
		case TokenType::EITHER:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::EitherExpression();
			}
			break;
		case TokenType::WON:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::WonExpression();
			}
			break;
		case TokenType::DIFFRINT:
			tokens_->AdvanceToNextToken();
			return new AST::DiffrintExpression();
		default:
			break;
	}
	
	tokens_->SkipToEOL();
	return nullptr;
}

AST::NaryExpression *Parser::ParseNaryOperator() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::ALL:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::AllExpression();
			}
			break;
		case TokenType::ANY:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::AnyExpression();
			}
			break;
		default:
			break;
	}
	
	tokens_->SkipToEOL();
	return nullptr;
}

AST::FunkshunCall *Parser::ParseFunkshunCall() {
	CheckTokenAndAdvance( TokenType::I );
	if( !CheckTokenAndAdvance( TokenType::IZ ) ) {
		return nullptr;
	}
	
	AST::LiteralIdentifier *funkshunIdentifier = ParseLiteralIdentifier();
	if( funkshunIdentifier == nullptr ) {
		return nullptr;
	}
	
	AST::FunkshunCall *funkshunCall = new AST::FunkshunCall();
	
	if( tokens_->GetNextToken().type == TokenType::YR ) {
		AST::Identifier *id = ParseIdentifier();
		if( id == nullptr ) {
			delete funkshunCall;
			return nullptr;
		}
		
		while( tokens_->GetNextToken().type == TokenType::AN ) {
			if( !CheckTokenAndAdvance( TokenType::YR ) ) {
				delete funkshunCall;
				return nullptr;
			}
			
			id = ParseIdentifier();
			if( id == nullptr ) {
				delete funkshunCall;
				return nullptr;
			}
		}
	}
	
	switch( tokens_->GetNextToken().type ) {
		case TokenType::MKAY:
			tokens_->AdvanceToNextToken();
		case TokenType::LINE_DELIMITER:
			return funkshunCall;
		default:
			delete funkshunCall;
			tokens_->SkipToEOL();
			return nullptr;
	}
}

AST::CastExpression *Parser::ParseCastExpression() {
	if( !CheckTokenAndAdvance( TokenType::MAEK ) ) {
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	
	AST::CastExpression *castExpr = new AST::CastExpression();
	
	if( !CheckTokenAndAdvance( TokenType::A ) ) {
		delete castExpr;
		return nullptr;
	}
	
	AST::TypeIdentifier *type = ParseTypeIdentifier();
	if( type == nullptr ) {
		delete castExpr;
		return nullptr;
	}
	
	return castExpr;
}

AST::SmooshExpression *Parser::ParseSmooshExpression() {
	if( !CheckTokenAndAdvance( TokenType::SMOOSH ) ) {
		return nullptr;
	}
	
	AST::SmooshExpression *smooshExpr = new AST::SmooshExpression();
	
	switch( tokens_->GetNextToken().type ) {
		case TokenType::MKAY:
			tokens_->AdvanceToNextToken();
		case TokenType::LINE_DELIMITER:
			return smooshExpr;
		default:
			break;
	}
	
	AST::Expression *operand = ParseExpression();
	if( operand == nullptr ) {
		delete smooshExpr;
		return nullptr;
	}
	// TODO: Add operand to smooshExpr.
	
	while( true ) {
		if( tokens_->GetNextToken().type == TokenType::MKAY ) {
			tokens_->AdvanceToNextToken();
			break;
		} else if( tokens_->GetNextToken().type == TokenType::LINE_DELIMITER ) {
			break;
		}
		
		if( !CheckTokenAndAdvance( TokenType::AN ) ) {
			delete smooshExpr;
			return nullptr;
		}
		
		operand = ParseExpression();
		if( operand == nullptr ) {
			delete smooshExpr;
			return nullptr;
		}
		// TODO: Add operand to smooshExpr.
	}
	
	return smooshExpr;
}

AST::Identifier *Parser::ParseIdentifier() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::IDENTIFIER:
			return ParseLiteralIdentifier();
		case TokenType::SRS:
			return ParseSrsIdentifier();
		case TokenType::IT:
			return ParseItIdentifier();
		case TokenType::SLOT:
			return ParseSlotIdentifier();
		default:
			tokens_->SkipToEOL();
			return nullptr;
	}
}

AST::Identifier *Parser::ParseExplicitIdentifier() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::IDENTIFIER:
			return ParseLiteralIdentifier();
		case TokenType::SRS:
			return ParseSrsIdentifier();
		case TokenType::SLOT:
			return ParseSlotIdentifier();
		default:
			tokens_->SkipToEOL();
			return nullptr;
	}
}

AST::Identifier *Parser::ParseInductionIdentifier() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::IDENTIFIER:
			return ParseLiteralIdentifier();
		case TokenType::SRS:
			return ParseSrsIdentifier();
		default:
			tokens_->SkipToEOL();
			return nullptr;
	}
}

AST::LiteralIdentifier *Parser::ParseLiteralIdentifier() {
	Token curToken;
	
	if( !CheckTokenAndAdvance( TokenType::IDENTIFIER, &curToken ) ) {
		return nullptr;
	}
	
	return new AST::LiteralIdentifier();
}

AST::SrsIdentifier *Parser::ParseSrsIdentifier() {
	if( !CheckTokenAndAdvance( TokenType::SRS ) ) {
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	
	return new AST::SrsIdentifier();
}

AST::ItIdentifier *Parser::ParseItIdentifier() {
	if( !CheckTokenAndAdvance( TokenType::IT ) ) {
		return nullptr;
	}
	
	return new AST::ItIdentifier();
}

AST::SlotIdentifier *Parser::ParseSlotIdentifier() {
	if( !CheckTokenAndAdvance( TokenType::SLOT ) ) {
		return nullptr;
	}
	
	AST::Expression *key = ParseExpression();
	if( key == nullptr ) {
		return nullptr;
	}
	
	if( !CheckTokenAndAdvance( TokenType::IN ) ) {
		return nullptr;
	}
	
	AST::Identifier *bukkitId = ParseIdentifier();
	if( bukkitId == nullptr ) {
		delete key;
		return nullptr;
	}
	
	return new AST::SlotIdentifier();
}

AST::TypeIdentifier *Parser::ParseTypeIdentifier() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::NOOB:
		case TokenType::TROOF:
		case TokenType::NUMBR:
		case TokenType::NUMBAR:
		case TokenType::YARN:
		case TokenType::BUKKIT:
			break;
		default:
			tokens_->SkipToEOL();
			return nullptr;
	}
	tokens_->AdvanceToNextToken();
	
	return new AST::TypeIdentifier();
}

AST::Literal *Parser::ParseLiteral() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::WIN:
		case TokenType::FAIL:
			return ParseTroofLiteral();
		case TokenType::NUMBR_LITERAL:
			return ParseNumbrLiteral();
		case TokenType::NUMBAR_LITERAL:
			return ParseNumbarLiteral();
		case TokenType::YARN_LITERAL:
			return ParseYarnLiteral();
		default:
			return nullptr;
	}
}

AST::TroofLiteral *Parser::ParseTroofLiteral() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::WIN:
		case TokenType::FAIL:
			break;
		default:
			tokens_->SkipToEOL();
			return nullptr;
	}
	tokens_->AdvanceToNextToken();
	
	return new AST::TroofLiteral();
}

AST::NumbrLiteral *Parser::ParseNumbrLiteral() {
	Token curToken;
	
	if( !CheckTokenAndAdvance( TokenType::NUMBR_LITERAL, &curToken ) ) {
		return nullptr;
	}
	
	return new AST::NumbrLiteral();
}

AST::NumbarLiteral *Parser::ParseNumbarLiteral() {
	Token curToken;
	
	if( !CheckTokenAndAdvance( TokenType::NUMBAR_LITERAL, &curToken ) ) {
		return nullptr;
	}
	
	return new AST::NumbarLiteral();
}

AST::YarnLiteral *Parser::ParseYarnLiteral() {
	Token curToken;
	
	if( !CheckTokenAndAdvance( TokenType::YARN_LITERAL, &curToken ) ) {
		return nullptr;
	}
	
	return new AST::YarnLiteral();
}

bool Parser::CheckTokenAndAdvance( TokenType expected, Token *token ) {
	Token curToken = tokens_->GetNextToken();

	if( !lineGood_ ) {
		if( ( curToken.type == TokenType::LINE_DELIMITER ||  
			curToken.type == TokenType::END_OF_FILE ) && 
			curToken.type == expected ) {
			lineGood_ = true;
		}

		return false;
	}

	if( curToken.type != expected ) {
		lineGood_ = false;
		tokens_->SkipToEOL();

		// TODO: Output some error message.

		return false;
	}
	
	tokens_->AdvanceToNextToken();
	if( token != nullptr ) {
		token->type = curToken.type;
		token->string = curToken.string;
	}

	return true;
}
