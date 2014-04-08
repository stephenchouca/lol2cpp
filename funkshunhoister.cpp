#include <cassert>
#include <iostream>

#include "funkshunhoister.h"
#include "statement.h"

void FunkshunHoister::ProcessEnd( AST::StatementBlock *stmtBlock ) {
	if( stmtBlock->GetType() == AST::StatementBlock::Type::PROGRAM_GLOBALS ) {
		return;
	}
	
	for( AST::StatementListIterator it = stmtBlock->GetStatements().begin(); 
		 it != stmtBlock->GetStatements().end(); ++it ) {
		AST::FunkshunBlock *funkshun = dynamic_cast<AST::FunkshunBlock *>( *it );
		
		if( funkshun == nullptr ) {
			continue;
		}
		
		stmtBlock->SwapStatement( it, 
			new AST::FunkshunDeclare( funkshun->GetName()->Clone() ) );
	}
}

void FunkshunHoister::ProcessBegin( AST::FunkshunBlock *funkshunBlock ) {
	AST::ASTNode *node = funkshunBlock->GetParent();
	while( node != nullptr && dynamic_cast<AST::Program *>( node ) == nullptr ) {
		node = node->GetParent();
	}
	assert( node != nullptr );
	
	AST::Program *program = dynamic_cast<AST::Program *>( node );
	program->GetGlobals()->AddStatement( funkshunBlock );
	funkshunBlock->SetParent( program );
}