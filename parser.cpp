#include <cassert>
#include <stdexcept>

#include "parser.h"
#include "tokenlist.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"

AST::Program *Parser::Parse( TokenList *tokens ) {
	tokens_ = tokens;
	funkshuns_.clear();
	lineGood_ = true;
	tokens_->StartIterating();

	AcceptToken( TokenType::HAI );
	
	// Not really used right now; will possibly be useful in future when 
	// grammar is altered in future (e.g. possibly range-loop construct).
	if( tokens_->PeekToken().type == TokenType::NUMBAR_LITERAL ) {
		const std::string lolcodeVersion = tokens_->PeekToken().string;
		if( lolcodeVersion != "1.2" ) {
			return nullptr;
		}
		tokens_->AdvanceToNextToken();
	}
	lolcodeVersion_ = LolcodeVersion::V12;
	
	AcceptToken( TokenType::LINE_DELIMITER );

	AST::StatementBlock *body = ParseProgramBody();
	if( body == nullptr ) {
		while( !funkshuns_.empty() ) {
			delete funkshuns_.back();
			funkshuns_.pop_back();
		}
		return nullptr;
	}
	AST::Program *program = new AST::Program( funkshuns_, body );

	AcceptToken( TokenType::KTHXBYE );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ||
		!AcceptToken( TokenType::END_OF_FILE ) ) {
		tokens_->AdvanceToNextToken();
		delete program;
		return nullptr;
	}

	return program;
}

AST::StatementBlock *Parser::ParseStatementBlock( 
		const StatementBlockType bodyType ) {
	AST::StatementBlock *stmtBlock = new AST::StatementBlock();
	
	while( true ) {
		AST::Statement *stmt = nullptr;
		switch( tokens_->PeekToken().type ) {
			case TokenType::KTHXBYE:
				if( bodyType == StatementBlockType::PROGRAM_BODY ) {
					return stmtBlock;
				}
				break;
			case TokenType::IF:
				if( bodyType == StatementBlockType::FUNKSHUN_BODY ) {
					return stmtBlock;
				}
				break;
			case TokenType::MEBBE:
			case TokenType::NO:
				switch( bodyType ) {
					case StatementBlockType::ORLY_YA:
					case StatementBlockType::ORLY_MEBBE:
						return stmtBlock;
					default:
						break;
				}
				break;
			case TokenType::OIC:
				switch( bodyType ) {
					case StatementBlockType::ORLY_YA:
					case StatementBlockType::ORLY_MEBBE:
					case StatementBlockType::ORLY_NO:
					case StatementBlockType::WTF_OMG:
					case StatementBlockType::WTF_OMGWTF:
						return stmtBlock;
					default:
						break;
				}
				break;
			case TokenType::OMG:
			case TokenType::OMGWTF:
				if( bodyType == StatementBlockType::WTF_OMG ) {
					return stmtBlock;
				}
				break;
			case TokenType::KTHX:
				switch( bodyType ) {
					case StatementBlockType::PLZ_BODY:
					case StatementBlockType::PLZ_ONOES:
					case StatementBlockType::PLZ_OWEL:
						return stmtBlock;
					default:
						break;
				}
				break;
			case TokenType::O:
				switch( tokens_->PeekToken( 1 ).type ) {
					case TokenType::RLY:
						stmt = ParseORlyBlock();
						break;
					case TokenType::NOES:
					case TokenType::WEL:
						switch( bodyType ) {
							case StatementBlockType::PLZ_BODY:
							case StatementBlockType::PLZ_ONOES:
								return stmtBlock;
							default:
								break;
						}
						break;
					default:
						break;
				}
				break;
			case TokenType::WTF:
				stmt = ParseWtfBlock();
				break;
			case TokenType::IM:
				switch( tokens_->PeekToken( 1 ).type ) {
					case TokenType::IN:
						stmt = ParseLoopBlock();
						break;
					case TokenType::OUTTA:
						if( bodyType == StatementBlockType::LOOP_BODY ) {
							return stmtBlock;
						}
						break;
					default:
						break;
				}
				break;
			case TokenType::HOW:
				if( bodyType == StatementBlockType::PROGRAM_BODY ) {
					AST::FunkshunBlock *funkshun = ParseFunkshunBlock();
					if( funkshun != nullptr ) {
						funkshuns_.push_back( funkshun );
						stmt = new AST::FunkshunDeclare( 
							funkshun->GetName()->Clone() );
					}
				}
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
			return nullptr;
		}
		stmtBlock->AddStatement( stmt );
	}
}

AST::ORlyBlock *Parser::ParseORlyBlock() {
	AcceptToken( TokenType::O );
	AcceptToken( TokenType::RLY );
	AcceptToken( TokenType::QUESTION );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AcceptToken( TokenType::YA );
	AcceptToken( TokenType::RLY );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::StatementBlock *yaBody = ParseORlyYaBody();
	if( yaBody == nullptr ) {
		return nullptr;
	}
	AST::ORlyBlock *oRlyBlock = new AST::ORlyBlock( yaBody );

	while( tokens_->PeekToken().type == TokenType::MEBBE ) {
		tokens_->AdvanceToNextToken();
		
		AST::Expression *cond = ParseExpression();
		if( cond == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
		if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete cond;
			delete oRlyBlock;
			return nullptr;
		}
		
		AST::StatementBlock *mebbeBody = ParseORlyMebbeBody();
		if( mebbeBody == nullptr ) {
			delete cond;
			delete oRlyBlock;
			return nullptr;
		}
		oRlyBlock->AddMebbeBlock( cond, mebbeBody );
	}

	if( tokens_->PeekToken().type == TokenType::NO ) {
		tokens_->AdvanceToNextToken();
		
		AcceptToken( TokenType::WAI );
		if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete oRlyBlock;
			return nullptr;
		}

		AST::StatementBlock *noBody = ParseORlyNoBody();
		if( noBody == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
		oRlyBlock->SetNoWaiBody( noBody );
	}

	AcceptToken( TokenType::OIC );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete oRlyBlock;
		return nullptr;
	}

	return oRlyBlock;
}

AST::WtfBlock *Parser::ParseWtfBlock() {
	AcceptToken( TokenType::WTF );
	AcceptToken( TokenType::QUESTION );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}

	if( tokens_->PeekToken().type != TokenType::OMG ) {
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
		if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete literal;
			delete wtfBlock;
			return nullptr;
		}

		AST::StatementBlock *omgBody = ParseWtfOmgBody();
		if( omgBody == nullptr ) {
			delete literal;
			delete wtfBlock;
			return nullptr;
		}

		wtfBlock->AddOmgBlock( literal, omgBody );
	} while( tokens_->PeekToken().type == TokenType::OMG );

	if( tokens_->PeekToken().type == TokenType::OMGWTF ) {
		tokens_->AdvanceToNextToken();
		if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete wtfBlock;
			return nullptr;
		}
		
		AST::StatementBlock *omgwtfBody = ParseWtfOmgwtfBody();
		if( omgwtfBody == nullptr ) {
			delete wtfBlock;
			return nullptr;
		}
		wtfBlock->SetOmgwtfBody( omgwtfBody );
	}
	
	AcceptToken( TokenType::OIC );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete wtfBlock;
		return nullptr;
	}
	
	return wtfBlock;
}

AST::LoopBlock *Parser::ParseLoopBlock() {
	AcceptToken( TokenType::IM );
	AcceptToken( TokenType::IN );
	if( !AcceptToken( TokenType::YR ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::LiteralIdentifier *id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	
	AST::LoopBlock *loopBlock = nullptr;
	switch( tokens_->PeekToken().type ) {
		case TokenType::LINE_DELIMITER:
			loopBlock = new AST::ForLoopBlock( id );
			break;
		case TokenType::WATCHIN:
		{
			AST::RangeLoopBlock *rangeLoopBlock = new AST::RangeLoopBlock( id );
			tokens_->AdvanceToNextToken();
			
			bool iteratorIsLocal = false;
			if( tokens_->PeekToken().type == TokenType::YR ) {
				tokens_->AdvanceToNextToken();
				iteratorIsLocal = true;
			}
			
			AST::Identifier *iterator = ParseLoopVarIdentifier();
			if( iterator == nullptr ) {
				delete rangeLoopBlock;
				return nullptr;
			}
			rangeLoopBlock->SetLoopVariable( iterator, iteratorIsLocal );
			
			if( !AcceptToken( TokenType::FROM ) ) {
				tokens_->AdvanceToNextToken();
				delete rangeLoopBlock;
				return nullptr;
			}
			
			AST::Expression *bukkitRef = ParseBukkitReference();
			if( bukkitRef == nullptr ) {
				delete rangeLoopBlock;
				return nullptr;
			}
			rangeLoopBlock->SetBukkitRef( bukkitRef );
			
			loopBlock = rangeLoopBlock;
			break;
		}
		default:
		{
			AST::ForLoopBlock *forLoopBlock = new AST::ForLoopBlock( id );
			AST::UnaryExpression *unaryIncExpr = nullptr;
			AST::FunkshunCall *funkshunIncExpr = nullptr;
			
			switch( tokens_->PeekToken().type ) {
				case TokenType::WITH:
					tokens_->AdvanceToNextToken();
					break;
				case TokenType::UPPIN:
					unaryIncExpr = new AST::UppinExpression();
					forLoopBlock->SetLoopVariableIncExpr( unaryIncExpr );
					tokens_->AdvanceToNextToken();
					break;
				case TokenType::NERFIN:
					unaryIncExpr = new AST::NerfinExpression();
					forLoopBlock->SetLoopVariableIncExpr( unaryIncExpr );
					tokens_->AdvanceToNextToken();
					break;
				default:
				{
					unaryIncExpr = ParseUnaryOperator();
					if( unaryIncExpr != nullptr ) {
						forLoopBlock->SetLoopVariableIncExpr( unaryIncExpr );
						break;
					}
					
					// TODO: Expand to support indirect function calls via SRS.
					AST::LiteralIdentifier *funkshunId = ParseLiteralIdentifier();
					if( funkshunId == nullptr ) {
						delete forLoopBlock;
						return nullptr;
					}
					
					funkshunIncExpr = new AST::FunkshunCall( funkshunId );
					forLoopBlock->SetLoopVariableIncExpr( funkshunIncExpr );
					break;
				}
			}
			
			bool loopVarIsLocal = false;
			if( tokens_->PeekToken().type == TokenType::YR ) {
				tokens_->AdvanceToNextToken();
				loopVarIsLocal = true;
			}
			
			AST::Identifier *loopVar = ParseLoopVarIdentifier();
			if( loopVar == nullptr ) {
				delete forLoopBlock;
				return nullptr;
			}
			forLoopBlock->SetLoopVariable( loopVar, loopVarIsLocal );
			if( unaryIncExpr != nullptr ) {
				unaryIncExpr->SetOperand( loopVar->Clone() );
			} else if( funkshunIncExpr != nullptr ) {
				funkshunIncExpr->AddOperand( loopVar->Clone() );
			}
			
			if( tokens_->PeekToken().type == TokenType::FROM ) {
				tokens_->AdvanceToNextToken();
				
				AST::Expression *initExpr = ParseExpression();
				if( initExpr == nullptr ) {
					delete forLoopBlock;
					return nullptr;
				}
				forLoopBlock->SetLoopVariableInitExpr( initExpr );
			}
			
			AST::UnaryExpression *testExpr = nullptr;
			switch( tokens_->PeekToken().type ) {
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
					delete testExpr;
					delete forLoopBlock;
					return nullptr;
				}
				testExpr->SetOperand( expr );
				forLoopBlock->SetLoopGuard( testExpr );
			}
			
			loopBlock = forLoopBlock;
			break;
		}
	}
	assert( loopBlock != nullptr );
	
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete loopBlock;
		return nullptr;
	}
	
	AST::StatementBlock *body = ParseLoopBody();
	if( body == nullptr ) {
		delete loopBlock;
		return nullptr;
	}
	loopBlock->SetBody( body );
	
	AcceptToken( TokenType::IM );
	AcceptToken( TokenType::OUTTA );
	if( !AcceptToken( TokenType::YR ) ) {
		tokens_->AdvanceToNextToken();
		delete loopBlock;
		return nullptr;
	}
	
	id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		// TODO: Should also check for match with loop begin identifier.
		delete loopBlock;
		return nullptr;
	}
	
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete loopBlock;
		return nullptr;
	}

	return loopBlock;
}

AST::FunkshunBlock *Parser::ParseFunkshunBlock() {
	AcceptToken( TokenType::HOW );
	AcceptToken( TokenType::IZ );
	if( !AcceptToken( TokenType::I ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}

	AST::LiteralIdentifier *id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	AST::FunkshunBlock *funkshun = new AST::FunkshunBlock( id );

	bool isPassByRef = false;
	switch( tokens_->PeekToken().type ) {
		case TokenType::DAT:
			isPassByRef = true;
		case TokenType::YR:
			tokens_->AdvanceToNextToken();
			
			id = ParseLiteralIdentifier();
			if( id == nullptr ) {
				delete funkshun;
				return nullptr;
			}
			funkshun->AddParameter( id, isPassByRef );
			
			while( tokens_->PeekToken().type == TokenType::AN ) {
				tokens_->AdvanceToNextToken();
				
				switch( tokens_->PeekToken().type ) {
					case TokenType::DAT:
						isPassByRef = true;
						break;
					case TokenType::YR:
						isPassByRef = false;
						break;
					default:
						tokens_->SkipToNextLine();
						delete funkshun;
						return nullptr;
				}
				tokens_->AdvanceToNextToken();
				
				id = ParseLiteralIdentifier();
				if( id == nullptr ) {
					delete funkshun;
					return nullptr;
				}
				funkshun->AddParameter( id, isPassByRef );
			}
			
			break;
		default:
			break;
	}

	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete funkshun;
		return nullptr;
	}

	AST::StatementBlock *body = ParseFunkshunBody();
	if( body == nullptr ) {
		delete funkshun;
		return nullptr;
	}
	funkshun->SetBody( body );

	AcceptToken( TokenType::IF );
	AcceptToken( TokenType::U );
	AcceptToken( TokenType::SAY );
	AcceptToken( TokenType::SO );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete funkshun;
		return nullptr;
	}

	return funkshun;
}

AST::PlzBlock *Parser::ParsePlzBlock() {
	AcceptToken( TokenType::PLZ );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	// TODO: Implement support for PLZ construct.
	return nullptr;

	AST::StatementBlock *tryBody = ParsePlzBody();
	if( tryBody == nullptr ) {
		return nullptr;
	}
	AST::PlzBlock *plzBlock = new AST::PlzBlock( tryBody );

	while( tokens_->PeekToken().type == TokenType::O && 
		   tokens_->PeekToken( 1 ).type == TokenType::NOES ) {
		tokens_->AdvanceToNextToken( 2 );

		AST::Expression *expr = ParseExpression();
		if( expr == nullptr ) {
			delete plzBlock;
			return nullptr;
		}

		if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete expr;
			delete plzBlock;
			return nullptr;
		}

		AST::StatementBlock *oNoesBody = ParsePlzONoesBody();
		if( oNoesBody == nullptr ) {
			delete expr;
			delete plzBlock;
			return nullptr;
		}
		plzBlock->AddONoesBlock( expr, oNoesBody );
	}

	if( tokens_->PeekToken().type == TokenType::O ) {
		AcceptToken( TokenType::WEL );
		if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete plzBlock;
			return nullptr;
		}

		AST::StatementBlock *oWelBody = ParsePlzOWelBody();
		if( oWelBody == nullptr ) {
			delete plzBlock;
			return nullptr;
		}
		plzBlock->SetOWelBody( oWelBody );
	}

	AcceptToken( TokenType::KTHX );
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete plzBlock;
		return nullptr;
	}

	return plzBlock;
}

AST::Statement *Parser::ParseStatement() {
	AST::Statement *stmt = nullptr;
	bool isSlotIdentifier = true;

	switch( tokens_->PeekToken().type ) {
		case TokenType::I:
			switch( tokens_->PeekToken( 1 ).type ) {
				case TokenType::HAS:
					stmt = ParseVarDeclare();
					break;
				case TokenType::IZ:
				{
					AST::FunkshunCall *funkshunCall = ParseFunkshunCall();
					if( funkshunCall == nullptr ) {
						break;
					}
					
					AST::VarAssign *varAssign = 
						AST::VarAssign::CreateImplicitAssign();
					varAssign->SetAssignValue( funkshunCall );
					stmt = varAssign;
					break;
				}
			}
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
			// TODO: To add support for breaking out of outer loop, probably want 
			//		 to refactor this out to another function.
			tokens_->AdvanceToNextToken();
			stmt = new AST::GtfoStatement();
			break;
		case TokenType::WHATEVR:
			tokens_->AdvanceToNextToken();
			stmt = new AST::WhatevrStatement();
			break;
		case TokenType::IDENTIFIER:
		case TokenType::SRS:
			isSlotIdentifier = false;
		case TokenType::SLOT:
		{
			AST::Identifier *id = ParseExplicitIdentifier();
			if( id == nullptr ) {
				return nullptr;
			}
			
			switch( tokens_->PeekToken().type ) {
				case TokenType::R:
					if( isSlotIdentifier ) {
						assert( dynamic_cast<AST::SlotIdentifier *>( id ) != nullptr );
						static_cast<AST::SlotIdentifier *>( id )->SetSafety( false );
					}
					stmt = ParseVarAssign( id );
					break;
				case TokenType::IS:
					stmt = ParseVarCast( id );
					break;
				default:
				{
					AST::VarAssign *varAssign = 
						AST::VarAssign::CreateImplicitAssign();
					varAssign->SetAssignValue( id );
					stmt = varAssign;
					break;
				}
			}
			break;
		}
		default:
		{
			AST::Expression *expr = ParseExpression();
			if( expr == nullptr ) {
				break;
			}
			
			AST::VarAssign *varAssign = AST::VarAssign::CreateImplicitAssign();
			varAssign->SetAssignValue( expr );
			stmt = varAssign;
			break;
		}
	}
	if( stmt == nullptr ) {
		return nullptr;
	}
	
	if( !AcceptToken( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete stmt;
		return nullptr;
	}

	return stmt;
}

AST::VarDeclare *Parser::ParseVarDeclare() {
	AcceptToken( TokenType::I );
	AcceptToken( TokenType::HAS );
	if( !AcceptToken( TokenType::A ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	TokenType varDeclType = tokens_->PeekToken().type;
	AST::Identifier *id = ParseExplicitIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	
	AST::VarDeclare *varDecl;
	switch( varDeclType ) {
		case TokenType::IDENTIFIER:
			assert( dynamic_cast<AST::LiteralIdentifier *>( id ) != nullptr );
			varDecl = new AST::LiteralVarDeclare( 
				static_cast<AST::LiteralIdentifier *>( id ) );
			break;
		case TokenType::SRS:
			assert( dynamic_cast<AST::SrsIdentifier *>( id ) != nullptr );
			varDecl = new AST::SrsVarDeclare( 
				static_cast<AST::SrsIdentifier *>( id ) );
			break;
		case TokenType::SLOT:
		{
			assert( dynamic_cast<AST::SlotIdentifier *>( id ) != nullptr );
			AST::SlotIdentifier *slotId = static_cast<AST::SlotIdentifier *>( id );
			varDecl = new AST::SlotVarDeclare( slotId );
			slotId->SetSafety( false );
			break;
		}
		default:
			return nullptr;
	}

	if( tokens_->PeekToken().type == TokenType::ITZ ) {
		tokens_->AdvanceToNextToken();
		
		if( tokens_->PeekToken().type == TokenType::A ) {
			tokens_->AdvanceToNextToken();
			
			AST::TypeIdentifier *type = ParseTypeIdentifier();
			if( type == nullptr ) {
				delete varDecl;
				return nullptr;
			}
			varDecl->SetInitType( type );
		} else {
			AST::Expression *expr = ParseExpression();
			if( expr == nullptr ) {
				delete varDecl;
				return nullptr;
			}
			varDecl->SetInitValue( expr );
		}
	}

	return varDecl;
}

AST::VarAssign *Parser::ParseVarAssign( AST::Identifier *id ) {
	AST::VarAssign *varAssign = new AST::VarAssign( id );
	
	if( !AcceptToken( TokenType::R ) ) {
		tokens_->AdvanceToNextToken();
		delete varAssign;
		return nullptr;
	}

	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		delete varAssign;
		return nullptr;
	}
	
	varAssign->SetAssignValue( expr );
	
	return varAssign;
}

AST::VarAssign *Parser::ParseVarCast( AST::Identifier *id ) {
	AST::VarAssign *varCast = new AST::VarAssign( id );
	
	AcceptToken( TokenType::IS );
	AcceptToken( TokenType::NOW );
	if( !AcceptToken( TokenType::A ) ) {
		tokens_->AdvanceToNextToken();
		delete varCast;
		return nullptr;
	}
	
	AST::TypeIdentifier *type = ParseTypeIdentifier();
	if( type == nullptr ) {
		delete varCast;
		return nullptr;
	}
	
	AST::MaekExpression *castExpr = new AST::MaekExpression( id->Clone() );
	castExpr->SetCastTargetType( type );
	varCast->SetAssignValue( castExpr );
	
	return varCast;
}

AST::VisibleStatement *Parser::ParseVisibleStatement() {
	if( !AcceptToken( TokenType::VISIBLE ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	AST::VisibleStatement *visibleStmt = new AST::VisibleStatement( expr );
	
	while( tokens_->PeekToken().type != TokenType::LINE_DELIMITER &&
		   tokens_->PeekToken().type != TokenType::EXCLAMATION ) {
		expr = ParseExpression();
		if( expr == nullptr ) {
			delete visibleStmt;
			return nullptr;
		}
		visibleStmt->AddExpression( expr );
	}
	
	if( tokens_->PeekToken().type == TokenType::EXCLAMATION ) {
		tokens_->AdvanceToNextToken();
		if( tokens_->PeekToken().type != TokenType::LINE_DELIMITER ) {
			delete visibleStmt;
			return nullptr;
		}
		
		visibleStmt->SetSuppressNewline();
	}
	
	return visibleStmt;
}

AST::GimmehStatement *Parser::ParseGimmehStatement() {
	if( !AcceptToken( TokenType::GIMMEH ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	bool isLong = false;
	if( tokens_->PeekToken().type == TokenType::LONG ) {
		isLong = true;
		tokens_->AdvanceToNextToken();
	}
	
	AST::Identifier *id = ParseExplicitIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	
	return new AST::GimmehStatement( id, isLong );
}

AST::FunkshunReturn *Parser::ParseFunkshunReturn() {
	AcceptToken( TokenType::FOUND );
	if( !AcceptToken( TokenType::YR ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *retExpr = ParseExpression();
	if( retExpr == nullptr ) {
		return nullptr;
	}
	
	return new AST::FunkshunReturn( retExpr );
}

AST::Expression *Parser::ParseExpression() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::IDENTIFIER:
		case TokenType::SRS:
		case TokenType::IT:
		case TokenType::SLOT:
			return ParseIdentifier();
		case TokenType::NOOB:
		case TokenType::WIN:
		case TokenType::FAIL:
		case TokenType::YARN_LITERAL:
		case TokenType::NUMBR_LITERAL:
		case TokenType::NUMBAR_LITERAL:
		case TokenType::MINUS_SIGN:
			return ParseLiteral();
		case TokenType::I:
			return ParseFunkshunCall();
		case TokenType::MAEK:
			return ParseMaekExpression();
		default:
			break;
	}
	
	AST::UnaryExpression *unaryExpr = ParseUnaryOperator();
	if( unaryExpr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete unaryExpr;
			return nullptr;
		}
		unaryExpr->SetOperand( operand );
		
		return unaryExpr;
	}
	
	AST::BinaryExpression *binaryExpr = ParseBinaryOperator();
	if( binaryExpr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete binaryExpr;
			return nullptr;
		}
		binaryExpr->SetLeftOperand( operand );
		
		if( tokens_->PeekToken().type == TokenType::AN ) {
			tokens_->AdvanceToNextToken();
		}
		
		operand = ParseExpression();
		if( operand == nullptr ) {
			delete binaryExpr;
			return nullptr;
		}
		binaryExpr->SetRightOperand( operand );
		
		return binaryExpr;
	}
	
	AST::NaryExpression *naryExpr = ParseNaryOperator();
	if( naryExpr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete naryExpr;
			return nullptr;
		}
		naryExpr->AddOperand( operand );
		
		while( true ) {
			if( tokens_->PeekToken().type == TokenType::MKAY ) {
				tokens_->AdvanceToNextToken();
				break;
			} else if( tokens_->PeekToken().type == TokenType::LINE_DELIMITER ) {
				break;
			}
			
			if( tokens_->PeekToken().type == TokenType::AN ) {
				tokens_->AdvanceToNextToken();
			}
			
			operand = ParseExpression();
			if( operand == nullptr ) {
				delete naryExpr;
				return nullptr;
			}
			naryExpr->AddOperand( operand );
		}
		
		return naryExpr;
	}
	
	// TODO: Output error message.
	tokens_->SkipToNextLine();
	return nullptr;
}

AST::UnaryExpression *Parser::ParseUnaryOperator() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::NOT:
			tokens_->AdvanceToNextToken();
			return new AST::NotExpression();
		default:
			break;
	}
	
	return nullptr;
}

AST::BinaryExpression *Parser::ParseBinaryOperator() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::SUM:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::SumExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::DIFF:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::DiffExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::PRODUKT:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::ProduktExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::QUOSHUNT:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::QuoshuntExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::MOD:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::ModExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::BIGGR:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::BiggrExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::SMALLR:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::SmallrExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::BOTH:
			switch( tokens_->PeekToken( 1 ).type ) {
				case TokenType::OF:
					tokens_->AdvanceToNextToken( 2 );
					return new AST::BothExpression();
				case TokenType::SAEM:
					tokens_->AdvanceToNextToken( 2 );
					return new AST::SaemExpression();
				default:
					break;
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::EITHER:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::EitherExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::WON:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::WonExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::DIFFRINT:
			tokens_->AdvanceToNextToken();
			return new AST::DiffrintExpression();
		default:
			break;
	}
	
	return nullptr;
}

AST::NaryExpression *Parser::ParseNaryOperator() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::ALL:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::AllExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::ANY:
			if( tokens_->PeekToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::AnyExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::SMOOSH:
			tokens_->AdvanceToNextToken();
			return new AST::SmooshExpression();
		default:
			break;
	}
	
	return nullptr;
}

AST::FunkshunCall *Parser::ParseFunkshunCall() {
	AcceptToken( TokenType::I );
	if( !AcceptToken( TokenType::IZ ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::LiteralIdentifier *funkshunId = ParseLiteralIdentifier();
	if( funkshunId == nullptr ) {
		return nullptr;
	}
	AST::FunkshunCall *funkshunCall = new AST::FunkshunCall( funkshunId );
	
	if( tokens_->PeekToken().type == TokenType::YR ) {
		tokens_->AdvanceToNextToken();
		
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete funkshunCall;
			return nullptr;
		}
		funkshunCall->AddOperand( operand );
		
		while( tokens_->PeekToken().type == TokenType::AN ) {
			tokens_->AdvanceToNextToken();
			if( !AcceptToken( TokenType::YR ) ) {
				tokens_->AdvanceToNextToken();
				delete funkshunCall;
				return nullptr;
			}
			
			operand = ParseExpression();
			if( operand == nullptr ) {
				delete funkshunCall;
				return nullptr;
			}
			funkshunCall->AddOperand( operand );
		}
	}
	
	switch( tokens_->PeekToken().type ) {
		case TokenType::MKAY:
			tokens_->AdvanceToNextToken();
		case TokenType::LINE_DELIMITER:
			return funkshunCall;
		default:
			delete funkshunCall;
			tokens_->SkipToNextLine();
			return nullptr;
	}
}

AST::MaekExpression *Parser::ParseMaekExpression() {
	if( !AcceptToken( TokenType::MAEK ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	AST::MaekExpression *castExpr = new AST::MaekExpression( expr );
	
	if( !AcceptToken( TokenType::A ) ) {
		tokens_->AdvanceToNextToken();
		delete castExpr;
		return nullptr;
	}
	
	AST::TypeIdentifier *type = ParseTypeIdentifier();
	if( type == nullptr ) {
		delete castExpr;
		return nullptr;
	}
	castExpr->SetCastTargetType( type );
	
	return castExpr;
}

AST::Expression *Parser::ParseBukkitReference() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::I:
			return ParseFunkshunCall();
		case TokenType::MAEK:
			return ParseMaekExpression();
		default:
			return ParseIdentifier();
	}
}

AST::Identifier *Parser::ParseIdentifier() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::IDENTIFIER:
			return ParseLiteralIdentifier();
		case TokenType::SRS:
			return ParseSrsIdentifier();
		case TokenType::IT:
			return ParseItIdentifier();
		case TokenType::SLOT:
			return ParseSlotIdentifier();
		default:
			tokens_->SkipToNextLine();
			return nullptr;
	}
}

AST::Identifier *Parser::ParseExplicitIdentifier() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::IDENTIFIER:
			return ParseLiteralIdentifier();
		case TokenType::SRS:
			return ParseSrsIdentifier();
		case TokenType::SLOT:
			return ParseSlotIdentifier();
		default:
			tokens_->SkipToNextLine();
			return nullptr;
	}
}

AST::Identifier *Parser::ParseLoopVarIdentifier() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::IDENTIFIER:
			return ParseLiteralIdentifier();
		case TokenType::SRS:
			return ParseSrsIdentifier();
		default:
			tokens_->SkipToNextLine();
			return nullptr;
	}
}

AST::LiteralIdentifier *Parser::ParseLiteralIdentifier() {
	Token curToken;
	
	if( !AcceptToken( TokenType::IDENTIFIER, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	return new AST::LiteralIdentifier( curToken.string );
}

AST::SrsIdentifier *Parser::ParseSrsIdentifier() {
	if( !AcceptToken( TokenType::SRS ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	// TODO: Implement support for SRS construct.
	return nullptr;
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	
	return new AST::SrsIdentifier( expr );
}

AST::ItIdentifier *Parser::ParseItIdentifier() {
	if( !AcceptToken( TokenType::IT ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	return new AST::ItIdentifier();
}

AST::SlotIdentifier *Parser::ParseSlotIdentifier() {
	if( !AcceptToken( TokenType::SLOT ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *key = ParseExpression();
	if( key == nullptr ) {
		return nullptr;
	}
	AST::SlotIdentifier *slotId = new AST::SlotIdentifier( key );
	
	if( !AcceptToken( TokenType::IN ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *bukkitRef = ParseBukkitReference();
	if( bukkitRef == nullptr ) {
		delete slotId;
		return nullptr;
	}
	slotId->SetBukkitRef( bukkitRef );
	
	return slotId;
}

AST::TypeIdentifier *Parser::ParseTypeIdentifier() {
	AST::TypeIdentifier::Type type;
	
	switch( tokens_->PeekToken().type ) {
		case TokenType::NOOB:
			type = AST::TypeIdentifier::Type::NOOB;
			break;
		case TokenType::TROOF:
			type = AST::TypeIdentifier::Type::TROOF;
			break;
		case TokenType::NUMBR:
			type = AST::TypeIdentifier::Type::NUMBR;
			break;
		case TokenType::NUMBAR:
			type = AST::TypeIdentifier::Type::NUMBAR;
			break;
		case TokenType::YARN:
			type = AST::TypeIdentifier::Type::YARN;
			break;
		case TokenType::BUKKIT:
			type = AST::TypeIdentifier::Type::BUKKIT;
			break;
		default:
			tokens_->SkipToNextLine();
			return nullptr;
	}
	tokens_->AdvanceToNextToken();
	
	return new AST::TypeIdentifier( type );
}

AST::Literal *Parser::ParseLiteral() {
	switch( tokens_->PeekToken().type ) {
		case TokenType::NOOB:
			tokens_->AdvanceToNextToken();
			return new AST::NoobLiteral();
		case TokenType::WIN:
		case TokenType::FAIL:
			return ParseTroofLiteral();
		case TokenType::NUMBR_LITERAL:
			return ParseNumbrLiteral();
		case TokenType::NUMBAR_LITERAL:
			return ParseNumbarLiteral();
		case TokenType::MINUS_SIGN:
			switch( tokens_->PeekToken( 1 ).type ) {
				case TokenType::NUMBR_LITERAL:
					return ParseNumbrLiteral();
				case TokenType::NUMBAR_LITERAL:
					return ParseNumbarLiteral();
				default:
					break;
			}
			break;
		case TokenType::YARN_LITERAL:
			return ParseYarnLiteral();
		default:
			break;
	}
	tokens_->SkipToNextLine();
	
	return nullptr;
}

AST::TroofLiteral *Parser::ParseTroofLiteral() {
	bool troofLit = false;
	
	switch( tokens_->PeekToken().type ) {
		case TokenType::WIN:
			troofLit = true;
		case TokenType::FAIL:
			break;
		default:
			tokens_->SkipToNextLine();
			return nullptr;
	}
	tokens_->AdvanceToNextToken();
	
	return new AST::TroofLiteral( troofLit );
}

AST::NumbrLiteral *Parser::ParseNumbrLiteral() {
	Token curToken;
	std::string numbrLiteral = "";
	
	if( tokens_->PeekToken().type == TokenType::MINUS_SIGN ) {
		tokens_->AdvanceToNextToken();
		numbrLiteral += "-";
	}
	
	if( !AcceptToken( TokenType::NUMBR_LITERAL, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	numbrLiteral += curToken.string;
	return new AST::NumbrLiteral( numbrLiteral );
}

AST::NumbarLiteral *Parser::ParseNumbarLiteral() {
	Token curToken;
	std::string numbarLiteral = "";
	
	if( tokens_->PeekToken().type == TokenType::MINUS_SIGN ) {
		tokens_->AdvanceToNextToken();
		numbarLiteral += "-";
	}
	
	if( !AcceptToken( TokenType::NUMBAR_LITERAL, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	numbarLiteral += curToken.string;
	return new AST::NumbarLiteral( numbarLiteral );
}

AST::YarnLiteral *Parser::ParseYarnLiteral() {
	Token curToken;
	
	if( !AcceptToken( TokenType::YARN_LITERAL, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	return new AST::YarnLiteral( curToken.string );
}

bool Parser::AcceptToken( TokenType expected, Token *token ) {
	Token curToken = tokens_->PeekToken();

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
