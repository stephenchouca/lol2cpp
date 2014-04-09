#include <cassert>

#include "globalshoister.h"
#include "statement.h"

void GlobalsHoister::ProcessEnd( AST::ProgramBody *programBody ) {
	for( AST::StatementListIterator it = programBody->GetStatements().begin(); 
		 it != programBody->GetStatements().end(); ++it ) {
		AST::FunkshunBlock *funkshun = dynamic_cast<AST::FunkshunBlock *>( *it );
		
		if( funkshun == nullptr ) {
			continue;
		}
		
		programBody->SwapStatement( it, 
			new AST::FunkshunDeclare( funkshun->GetName()->Clone() ) );
	}
}

void GlobalsHoister::ProcessBegin( AST::FunkshunBlock *funkshunBlock ) {
	AST::ASTNode *node = funkshunBlock->GetParent();
	while( node != nullptr && dynamic_cast<AST::Program *>( node ) == nullptr ) {
		node = node->GetParent();
	}
	assert( node != nullptr );
	
	AST::Program *program = dynamic_cast<AST::Program *>( node );
	program->GetGlobals()->AddStatement( funkshunBlock );
	funkshunBlock->SetParent( program );
}