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

	AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::PROGRAM );
	if( stmtBlock == nullptr ) {
		return nullptr;
	}
	AST::Program *program = new AST::Program( stmtBlock );

	CheckTokenAndAdvance( TokenType::KTHXBYE );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ||
		!CheckTokenAndAdvance( TokenType::END_OF_FILE ) ) {
		tokens_->AdvanceToNextToken();
		delete program;
		return nullptr;
	}

	return program;
}

AST::StatementBlock *Parser::ParseStatementBlock( const BodyType bodyType ) {
	AST::StatementBlock *stmtBlock = new AST::StatementBlock();
	
	while( true ) {
		AST::Statement *stmt = nullptr;
		std::cout << "next token: " << tokens_->GetNextToken() << std::endl;
		switch( tokens_->GetNextToken().type ) {
			case TokenType::KTHXBYE:
				if( bodyType == BodyType::PROGRAM ) {
					return stmtBlock;
				}
				break;
			case TokenType::IF:
				if( bodyType == BodyType::FUNKSHUN_BODY ) {
					return stmtBlock;
				}
				break;
			case TokenType::MEBBE:
			case TokenType::NO:
				switch( bodyType ) {
					case BodyType::ORLY_YA:
					case BodyType::ORLY_MEBBE:
						return stmtBlock;
					default:
						break;
				}
				break;
			case TokenType::OIC:
				switch( bodyType ) {
					case BodyType::ORLY_YA:
					case BodyType::ORLY_MEBBE:
					case BodyType::ORLY_NO:
					case BodyType::WTF_OMG:
					case BodyType::WTF_OMGWTF:
						std::cout << "return from oic" << std::endl;
						std::cout << tokens_->GetNextToken() << std::endl;
						return stmtBlock;
					default:
						break;
				}
				break;
			case TokenType::OMG:
			case TokenType::OMGWTF:
				if( bodyType == BodyType::WTF_OMG ) {
					return stmtBlock;
				}
				break;
			case TokenType::KTHX:
				switch( bodyType ) {
					case BodyType::PLZ_BODY:
					case BodyType::PLZ_ONOES:
					case BodyType::PLZ_OWELL:
						return stmtBlock;
					default:
						break;
				}
				break;
			case TokenType::O:
				switch( tokens_->GetNextToken( 1 ).type ) {
					case TokenType::RLY:
						stmt = ParseORlyBlock();
						break;
					case TokenType::NOES:
					case TokenType::WELL:
						switch( bodyType ) {
							case BodyType::PLZ_BODY:
							case BodyType::PLZ_ONOES:
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
				switch( tokens_->GetNextToken( 1 ).type ) {
					case TokenType::IN:
						stmt = ParseLoopBlock();
						break;
					case TokenType::OUTTA:
						if( bodyType == BodyType::LOOP_BODY ) {
							return stmtBlock;
						}
						break;
					default:
						break;
				}
				break;
			case TokenType::HOW:
				if( bodyType == BodyType::PROGRAM ) {
					stmt = ParseFunkshunBlock();
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
		std::cout << *stmt << std::endl;
		stmtBlock->AddStatement( stmt );
	}
}

AST::ORlyBlock *Parser::ParseORlyBlock() {
	std::cout << "here0" << std::endl;
	CheckTokenAndAdvance( TokenType::O );
	CheckTokenAndAdvance( TokenType::RLY );
	CheckTokenAndAdvance( TokenType::QUESTION );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	std::cout << "here" << std::endl;
	CheckTokenAndAdvance( TokenType::YA );
	CheckTokenAndAdvance( TokenType::RLY );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	std::cout << "here24" << std::endl;
	AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::ORLY_YA );
	if( stmtBlock == nullptr ) {
		return nullptr;
	}
	AST::ORlyBlock *oRlyBlock = new AST::ORlyBlock( stmtBlock );

	while( tokens_->GetNextToken().type == TokenType::MEBBE ) {
		std::cout << "here3" << std::endl;
		tokens_->AdvanceToNextToken();
		
		AST::Expression *cond = ParseExpression();
		if( cond == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete cond;
			delete oRlyBlock;
			return nullptr;
		}
		
		stmtBlock = ParseStatementBlock( BodyType::ORLY_MEBBE );
		if( stmtBlock == nullptr ) {
			delete cond;
			delete oRlyBlock;
			return nullptr;
		}
		
		oRlyBlock->AddMebbeBlock( cond, stmtBlock );
	}

	if( tokens_->GetNextToken().type == TokenType::NO ) {
		tokens_->AdvanceToNextToken();
		
		CheckTokenAndAdvance( TokenType::WAI );
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete oRlyBlock;
			return nullptr;
		}

		stmtBlock = ParseStatementBlock( BodyType::ORLY_NO );
		if( stmtBlock == nullptr ) {
			delete oRlyBlock;
			return nullptr;
		}
		
		oRlyBlock->SetNoWaiBlock( stmtBlock );
	}
	std::cout << "here4" << std::endl;

	CheckTokenAndAdvance( TokenType::OIC );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete oRlyBlock;
		return nullptr;
	}

	return oRlyBlock;
}

AST::WtfBlock *Parser::ParseWtfBlock() {
	CheckTokenAndAdvance( TokenType::WTF );
	CheckTokenAndAdvance( TokenType::QUESTION );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}

	if( tokens_->GetNextToken().type != TokenType::OMG ) {
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
			tokens_->AdvanceToNextToken();
			delete literal;
			delete wtfBlock;
			return nullptr;
		}

		AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::WTF_OMG );
		if( stmtBlock == nullptr ) {
			delete literal;
			delete wtfBlock;
			return nullptr;
		}
		std::cout << "got omg" << std::endl;

		wtfBlock->AddOmgBlock( literal, stmtBlock );
	} while( tokens_->GetNextToken().type == TokenType::OMG );

	if( tokens_->GetNextToken().type == TokenType::OMGWTF ) {
		tokens_->AdvanceToNextToken();
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete wtfBlock;
			return nullptr;
		}
		std::cout << "before parse omgwtf" << std::endl;
		AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::WTF_OMGWTF );
		if( stmtBlock == nullptr ) {
			delete wtfBlock;
			return nullptr;
		}
		
		wtfBlock->SetOmgwtfBlock( stmtBlock );
	}
	std::cout << "at end of parsewtf: " << tokens_->GetNextToken() << std::endl;
	CheckTokenAndAdvance( TokenType::OIC );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete wtfBlock;
		return nullptr;
	}
	std::cout << "done parsewtf" << std::endl;
	return wtfBlock;
}

AST::LoopBlock *Parser::ParseLoopBlock() {
	CheckTokenAndAdvance( TokenType::IM );
	CheckTokenAndAdvance( TokenType::IN );
	if( !CheckTokenAndAdvance( TokenType::YR ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::LiteralIdentifier *id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	
	AST::LoopBlock *loopBlock = nullptr;
	switch( tokens_->GetNextToken().type ) {
		case TokenType::LINE_DELIMITER:
			loopBlock = new AST::ForLoopBlock( id );
			break;
		case TokenType::WATCHIN:
		{
			AST::RangeLoopBlock *rangeLoopBlock = new AST::RangeLoopBlock( id );
			
			bool iteratorIsLocal = false;
			if( tokens_->GetNextToken().type == TokenType::YR ) {
				tokens_->AdvanceToNextToken();
				iteratorIsLocal = true;
			}
			
			AST::Identifier *iterator = ParseLoopVarIdentifier();
			if( iterator == nullptr ) {
				delete rangeLoopBlock;
				return nullptr;
			}
			rangeLoopBlock->SetLoopVariable( iterator, iteratorIsLocal );
			
			if( !CheckTokenAndAdvance( TokenType::FROM ) ) {
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
			
			AST::UnaryExpression *unaryIncExpr;
			switch( tokens_->GetNextToken().type ) {
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
					
					AST::LiteralIdentifier *funkshunId = ParseLiteralIdentifier();
					if( funkshunId != nullptr ) {
						AST::FunkshunCall *funkshunIncExpr = 
							new AST::FunkshunCall( funkshunId );
						forLoopBlock->SetLoopVariableIncExpr( funkshunIncExpr );
						break;
					}
					
					delete forLoopBlock;
					return nullptr;
				}
			}
			
			bool counterIsLocal = false;
			if( tokens_->GetNextToken().type == TokenType::YR ) {
				tokens_->AdvanceToNextToken();
				counterIsLocal = true;
			}
			
			AST::Identifier *counter = ParseLoopVarIdentifier();
			if( counter == nullptr ) {
				delete forLoopBlock;
				return nullptr;
			}
			forLoopBlock->SetLoopVariable( counter, counterIsLocal );
			
			if( tokens_->GetNextToken().type == TokenType::FROM ) {
				tokens_->AdvanceToNextToken();
				
				AST::Expression *initExpr = ParseExpression();
				if( initExpr == nullptr ) {
					delete forLoopBlock;
					return nullptr;
				}
				forLoopBlock->SetLoopVariableInitExpr( initExpr );
			}
			
			AST::UnaryBooleanExpression *testExpr = nullptr;
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
	
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete loopBlock;
		return nullptr;
	}
	
	AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::LOOP_BODY );
	if( stmtBlock == nullptr ) {
		delete loopBlock;
		return nullptr;
	}
	loopBlock->SetBody( stmtBlock );
	
	CheckTokenAndAdvance( TokenType::IM );
	CheckTokenAndAdvance( TokenType::OUTTA );
	if( !CheckTokenAndAdvance( TokenType::YR ) ) {
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
	
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete loopBlock;
		return nullptr;
	}

	return loopBlock;
}

AST::FunkshunBlock *Parser::ParseFunkshunBlock() {
	CheckTokenAndAdvance( TokenType::HOW );
	CheckTokenAndAdvance( TokenType::IZ );
	if( !CheckTokenAndAdvance( TokenType::I ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}

	AST::LiteralIdentifier *id = ParseLiteralIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	AST::FunkshunBlock *funkshun = new AST::FunkshunBlock( id );

	bool isPassByRef = false;
	switch( tokens_->GetNextToken().type ) {
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
			
			while( tokens_->GetNextToken().type == TokenType::AN ) {
				tokens_->AdvanceToNextToken();
				
				switch( tokens_->GetNextToken().type ) {
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

	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete funkshun;
		return nullptr;
	}

	AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::FUNKSHUN_BODY );
	if( stmtBlock == nullptr ) {
		delete funkshun;
		return nullptr;
	}
	funkshun->SetBody( stmtBlock );

	CheckTokenAndAdvance( TokenType::IF );
	CheckTokenAndAdvance( TokenType::U );
	CheckTokenAndAdvance( TokenType::SAY );
	CheckTokenAndAdvance( TokenType::SO );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete funkshun;
		return nullptr;
	}

	return funkshun;
}

AST::PlzBlock *Parser::ParsePlzBlock() {
	CheckTokenAndAdvance( TokenType::PLZ );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}

	AST::StatementBlock *stmtBlock = ParseStatementBlock( BodyType::PLZ_BODY );
	if( stmtBlock == nullptr ) {
		return nullptr;
	}
	AST::PlzBlock *plzBlock = new AST::PlzBlock( stmtBlock );

	while( tokens_->GetNextToken().type == TokenType::O && 
		   tokens_->GetNextToken( 1 ).type == TokenType::NOES ) {
		tokens_->AdvanceToNextToken( 2 );

		AST::Expression *expr = ParseExpression();
		if( expr == nullptr ) {
			delete plzBlock;
			return nullptr;
		}

		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete expr;
			delete plzBlock;
			return nullptr;
		}

		stmtBlock = ParseStatementBlock( BodyType::PLZ_ONOES );
		if( stmtBlock == nullptr ) {
			delete expr;
			delete plzBlock;
			return nullptr;
		}
		
		plzBlock->AddNoesBlock( expr, stmtBlock );
	}

	if( tokens_->GetNextToken().type == TokenType::O ) {
		CheckTokenAndAdvance( TokenType::WELL );
		if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
			tokens_->AdvanceToNextToken();
			delete plzBlock;
			return nullptr;
		}

		stmtBlock = ParseStatementBlock( BodyType::PLZ_OWELL );
		if( stmtBlock == nullptr ) {
			delete plzBlock;
			return nullptr;
		}
		plzBlock->SetWellBlock( stmtBlock );
	}

	CheckTokenAndAdvance( TokenType::KTHX );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
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
			AST::Identifier *id = ParseExplicitIdentifier();
			if( id == nullptr ) {
				return nullptr;
			}
			
			switch( tokens_->GetNextToken().type ) {
				case TokenType::R:
					stmt = ParseVarAssign( id );
					break;
				case TokenType::IS:
					stmt = ParseVarCast( id );
					break;
				default:
					stmt = id;
					break;
			}
			break;
		}
		default:
			std::cout << "here" << std::endl;
			stmt = ParseExpression();
			break;
	}
	if( stmt == nullptr ) {
		return nullptr;
	}
	std::cout << "got stmt" << std::endl;
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		tokens_->AdvanceToNextToken();
		delete stmt;
		return nullptr;
	}

	return stmt;
}

AST::VarDeclare *Parser::ParseVarDeclare() {
	CheckTokenAndAdvance( TokenType::I );
	CheckTokenAndAdvance( TokenType::HAS );
	if( !CheckTokenAndAdvance( TokenType::A ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Identifier *id = ParseExplicitIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	AST::VarDeclare *varDecl = new AST::VarDeclare( id );

	if( tokens_->GetNextToken().type == TokenType::ITZ ) {
		tokens_->AdvanceToNextToken();
		
		if( tokens_->GetNextToken().type == TokenType::A ) {
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
	
	if( !CheckTokenAndAdvance( TokenType::R ) ) {
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

AST::VarCast *Parser::ParseVarCast( AST::Identifier *id ) {
	AST::VarCast *varCast = new AST::VarCast( id );
	
	CheckTokenAndAdvance( TokenType::IS );
	CheckTokenAndAdvance( TokenType::NOW );
	if( !CheckTokenAndAdvance( TokenType::A ) ) {
		tokens_->AdvanceToNextToken();
		delete varCast;
		return nullptr;
	}
	
	AST::TypeIdentifier *type = ParseTypeIdentifier();
	if( type == nullptr ) {
		delete varCast;
		return nullptr;
	}
	varCast->SetCastTargetType( type );
	
	return varCast;
}

AST::VisibleStatement *Parser::ParseVisibleStatement() {
	if( !CheckTokenAndAdvance( TokenType::VISIBLE ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	std::cout << "parse visible" << std::endl;
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	AST::VisibleStatement *visibleStmt = new AST::VisibleStatement( expr );
	
	while( true ) {
		if( tokens_->GetNextToken().type == TokenType::LINE_DELIMITER ) {
			break;
		}
		
		expr = ParseExpression();
		if( expr == nullptr ) {
			delete visibleStmt;
			return nullptr;
		}
		visibleStmt->AddExpression( expr );
	}
	
	return visibleStmt;
}

AST::GimmehStatement *Parser::ParseGimmehStatement() {
	if( !CheckTokenAndAdvance( TokenType::GIMMEH ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	bool isLong = false;
	if( tokens_->GetNextToken().type == TokenType::LONG ) {
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
	CheckTokenAndAdvance( TokenType::FOUND );
	if( !CheckTokenAndAdvance( TokenType::YR ) ) {
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
	switch( tokens_->GetNextToken().type ) {
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
			return ParseCastExpression();
		case TokenType::SMOOSH:
			return ParseSmooshExpression();
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
	std::cout << "here2" << std::endl;
	AST::BinaryExpression *binaryExpr = ParseBinaryOperator();
	if( binaryExpr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete binaryExpr;
			return nullptr;
		}
		binaryExpr->SetLeftOperand( operand );
		
		if( !CheckTokenAndAdvance( TokenType::AN ) ) {
			tokens_->AdvanceToNextToken();
			delete binaryExpr;
			return nullptr;
		}
		
		operand = ParseExpression();
		if( operand == nullptr ) {
			delete binaryExpr;
			return nullptr;
		}
		binaryExpr->SetRightOperand( operand );
		
		return binaryExpr;
	}
	std::cout << "here4" << std::endl;
	AST::NaryExpression *naryExpr = ParseNaryOperator();
	if( naryExpr != nullptr ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete naryExpr;
			return nullptr;
		}
		naryExpr->AddOperand( operand );
		
		while( true ) {
			if( tokens_->GetNextToken().type == TokenType::MKAY ) {
				tokens_->AdvanceToNextToken();
				break;
			} else if( tokens_->GetNextToken().type == TokenType::LINE_DELIMITER ) {
				break;
			}
			
			if( !CheckTokenAndAdvance( TokenType::AN ) ) {
				tokens_->AdvanceToNextToken();
				delete naryExpr;
				return nullptr;
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
	switch( tokens_->GetNextToken().type ) {
		case TokenType::NOT:
			tokens_->AdvanceToNextToken();
			return new AST::NotExpression();
		default:
			break;
	}
	
	return nullptr;
}

AST::BinaryExpression *Parser::ParseBinaryOperator() {
	std::cout << tokens_->GetNextToken() << std::endl;
	switch( tokens_->GetNextToken().type ) {
		case TokenType::SUM:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::SumExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::DIFF:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::DiffExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::PRODUKT:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::ProduktExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::QUOSHUNT:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::QuoshuntExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::MOD:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::ModExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::BIGGR:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::BiggrExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::SMALLR:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::SmallrExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::BOTH:
			std::cout << "in both" << std::endl;
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
			tokens_->SkipToNextLine();
			break;
		case TokenType::EITHER:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::EitherExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::WON:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
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
	switch( tokens_->GetNextToken().type ) {
		case TokenType::ALL:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::AllExpression();
			}
			tokens_->SkipToNextLine();
			break;
		case TokenType::ANY:
			if( tokens_->GetNextToken( 1 ).type == TokenType::OF ) {
				tokens_->AdvanceToNextToken( 2 );
				return new AST::AnyExpression();
			}
			tokens_->SkipToNextLine();
			break;
		default:
			break;
	}
	
	return nullptr;
}

AST::FunkshunCall *Parser::ParseFunkshunCall() {
	CheckTokenAndAdvance( TokenType::I );
	if( !CheckTokenAndAdvance( TokenType::IZ ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::LiteralIdentifier *funkshunId = ParseLiteralIdentifier();
	if( funkshunId == nullptr ) {
		return nullptr;
	}
	AST::FunkshunCall *funkshunCall = new AST::FunkshunCall( funkshunId );
	
	if( tokens_->GetNextToken().type == TokenType::YR ) {
		AST::Expression *operand = ParseExpression();
		if( operand == nullptr ) {
			delete funkshunCall;
			return nullptr;
		}
		funkshunCall->AddOperand( operand );
		
		while( tokens_->GetNextToken().type == TokenType::AN ) {
			if( !CheckTokenAndAdvance( TokenType::YR ) ) {
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
	
	switch( tokens_->GetNextToken().type ) {
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

AST::CastExpression *Parser::ParseCastExpression() {
	if( !CheckTokenAndAdvance( TokenType::MAEK ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	AST::CastExpression *castExpr = new AST::CastExpression( expr );
	
	if( !CheckTokenAndAdvance( TokenType::A ) ) {
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

AST::SmooshExpression *Parser::ParseSmooshExpression() {
	if( !CheckTokenAndAdvance( TokenType::SMOOSH ) ) {
		tokens_->AdvanceToNextToken();
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
	smooshExpr->AddOperand( operand );
	
	while( true ) {
		if( tokens_->GetNextToken().type == TokenType::MKAY ) {
			tokens_->AdvanceToNextToken();
			break;
		} else if( tokens_->GetNextToken().type == TokenType::LINE_DELIMITER ) {
			break;
		}
		
		if( !CheckTokenAndAdvance( TokenType::AN ) ) {
			tokens_->AdvanceToNextToken();
			delete smooshExpr;
			return nullptr;
		}
		
		operand = ParseExpression();
		if( operand == nullptr ) {
			delete smooshExpr;
			return nullptr;
		}
		smooshExpr->AddOperand( operand );
	}
	
	return smooshExpr;
}

AST::Expression *Parser::ParseBukkitReference() {
	switch( tokens_->GetNextToken().type ) {
		case TokenType::I:
			return ParseFunkshunCall();
		case TokenType::MAEK:
			return ParseCastExpression();
		default:
			return ParseIdentifier();
	}
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
			tokens_->SkipToNextLine();
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
			tokens_->SkipToNextLine();
			return nullptr;
	}
}

AST::Identifier *Parser::ParseLoopVarIdentifier() {
	switch( tokens_->GetNextToken().type ) {
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
	
	if( !CheckTokenAndAdvance( TokenType::IDENTIFIER, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	return new AST::LiteralIdentifier( curToken.string );
}

AST::SrsIdentifier *Parser::ParseSrsIdentifier() {
	if( !CheckTokenAndAdvance( TokenType::SRS ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *expr = ParseExpression();
	if( expr == nullptr ) {
		return nullptr;
	}
	
	return new AST::SrsIdentifier( expr );
}

AST::ItIdentifier *Parser::ParseItIdentifier() {
	if( !CheckTokenAndAdvance( TokenType::IT ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	return new AST::ItIdentifier();
}

AST::SlotIdentifier *Parser::ParseSlotIdentifier() {
	if( !CheckTokenAndAdvance( TokenType::SLOT ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *key = ParseExpression();
	if( key == nullptr ) {
		return nullptr;
	}
	AST::SlotIdentifier *slotId = new AST::SlotIdentifier( key );
	
	if( !CheckTokenAndAdvance( TokenType::IN ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	AST::Expression *bukkitRef = ParseBukkitReference();
	if( bukkitRef == nullptr ) {
		delete slotId;
		return nullptr;
	}
	slotId->SetBukkitReference( bukkitRef );
	
	return slotId;
}

AST::TypeIdentifier *Parser::ParseTypeIdentifier() {
	AST::TypeIdentifier::Type type;
	
	switch( tokens_->GetNextToken().type ) {
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
	switch( tokens_->GetNextToken().type ) {
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
			switch( tokens_->GetNextToken( 1 ).type ) {
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
	
	switch( tokens_->GetNextToken().type ) {
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
	
	if( tokens_->GetNextToken().type == TokenType::MINUS_SIGN ) {
		tokens_->AdvanceToNextToken();
		numbrLiteral += "-";
	}
	
	if( !CheckTokenAndAdvance( TokenType::NUMBR_LITERAL, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	numbrLiteral += curToken.string;
	return new AST::NumbrLiteral( numbrLiteral );
}

AST::NumbarLiteral *Parser::ParseNumbarLiteral() {
	Token curToken;
	std::string numbarLiteral = "";
	
	if( tokens_->GetNextToken().type == TokenType::MINUS_SIGN ) {
		tokens_->AdvanceToNextToken();
		numbarLiteral += "-";
	}
	
	if( !CheckTokenAndAdvance( TokenType::NUMBAR_LITERAL, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	numbarLiteral += curToken.string;
	return new AST::NumbarLiteral( numbarLiteral );
}

AST::YarnLiteral *Parser::ParseYarnLiteral() {
	Token curToken;
	
	if( !CheckTokenAndAdvance( TokenType::YARN_LITERAL, &curToken ) ) {
		tokens_->AdvanceToNextToken();
		return nullptr;
	}
	
	return new AST::YarnLiteral( curToken.string );
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
