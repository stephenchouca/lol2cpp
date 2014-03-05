#include "parser.h"
#include "tokenlist.h"
#include "ast.h"

Parser::Parser() : 
	tokens_( nullptr ),
	program_( nullptr ),
	lineGood_( true ) {}

AST::Program *Parser::Parse( TokenList *tokens ) {
	tokens_ = tokens;
	lineGood_ = true;
	tokens_->StartIterating();

	CheckTokenAndAdvance( TokenType::HAI );
	CheckTokenAndAdvance( TokenType::LINE_DELIMITER );

	AST::StatementBlock *stmtBlock = ParseStatementBlock();

	CheckTokenAndAdvance( TokenType::KTHXBYE );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ||
		!CheckTokenAndAdvance( TokenType::END_OF_FILE ) ) {
		delete stmtBlock;
		return nullptr;
	}

	return nullptr;
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

	while( tokens_->GetNextToken() == TokenType::MEEBE ) {
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

	if( tokens_->GetNextToken() == TokenType::NO ) {
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

AST::WtfBlock Parser::ParseWtfBlock() {
	CheckTokenAndAdvance( TokenType::WTF );
	CheckTokenAndAdvance( TokenType::QUESTION );
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		return nullptr;
	}

	Token curToken = tokens_->GetNextToken();
	if( !curToken.type != TokenType::OMG ) {
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
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) {
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
	switch( tokens_->GetNextToken().type ) {
		case TokenType::LINE_DELIMITER:
			break;
		case TokenType::WATCHIN:
		case TokenType::UPPIN:
		case TokenType::NERFIN:
	if( !CheckTokenAndAdvance( TokenType::LINE_DELIMITER ) ) {
		delete id;
		return nullptr;
	}

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
			if( !CheckTokenAndAdvance( TokenType::YR ) {
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
			stmt = ParseVisibleStmt();
			break;
		case TokenType::GIMMEH:
			stmt = ParseGimmehStmt();
			break;
		case TokenType::GTFO:
			tokens_->AdvanceToNextToken();
			break;
		case TokenType::IDENTIFIER:
		{
			bool parsed = true;
			switch( tokens_->GetNextToken( 1 ).type ) {
				case TokenType::R:
					stmt = ParseVarAssign();
					break;
				case TokenType::IS:
					stmt = ParseVarCast();
					break;
				default:
					parsed = false;
					break;
			}
			if( parsed ) {
				break;
			}
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
		return nullptr;
	}
	
	AST::Identifier *id = ParseIdentifier();
	if( id == nullptr ) {
		return nullptr;
	}
	AST::VarDeclare *varDecl = new AST::VarDeclare();

	if( tokens_->GetNextToken().type == TokenType::ITZ ) {
		tokens_->AdvanceToNextToken();
		if( tokens_->GetNextToken().type == TokenType::A ) {
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

AST::VarAssign *ParseVarAssign() {

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
		tokens_->SkipLine();

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
