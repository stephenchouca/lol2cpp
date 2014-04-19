#include <cassert>

#include "program_visitor.h"
#include "statement.h"
#include "expression.h"

namespace AST {
	void ASTProgramOrderVisitor::Visit( StatementBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		for( StatementListIterator it = node->GetStatements().begin(); 
			 it != node->GetStatements().end(); ++it ) {
			Accept( *it );
		}
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( ORlyBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetYaRlyBody() );
		for( MebbeBlockListIterator it = node->GetMebbeBlocks().begin(); 
			 it != node->GetMebbeBlocks().end(); ++it ) {
			Accept( it->first );
			Accept( it->second );
		}
		Accept( node->GetNoWaiBody() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( WtfBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		for( OmgBlockListIterator it = node->GetOmgBlocks().begin(); 
			 it != node->GetOmgBlocks().end(); ++it ) {
			Accept( it->first );
			Accept( it->second );
		}
		Accept( node->GetOmgwtfBody() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( ForLoopBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetLoopVariable() );
		Accept( node->GetLoopVariableInitExpr() );
		Accept( node->GetLoopGuard() );
		Accept( node->GetLoopVariableIncExpr() );
		Accept( node->GetBody() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( RangeLoopBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetLoopVariable() );
		Accept( node->GetBukkitRef() );
		Accept( node->GetBody() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( FunkshunBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetName() );
		for( FunkshunParamListIterator it = node->GetParameters().begin(); 
			 it != node->GetParameters().end(); ++it ) {
			Accept( it->first );
		}
		Accept( node->GetBody() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( FunkshunDeclare *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetFunkshunName() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( PlzBlock *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		// TODO: Implement PLZ traversal.
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::VisitVarDeclare( VarDeclare *node ) {
		Accept( node->GetVariable() );
		Accept( node->GetInitValue() );
		Accept( node->GetInitType() );
	}
	
	void ASTProgramOrderVisitor::Visit( LiteralVarDeclare *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitVarDeclare( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SrsVarDeclare *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitVarDeclare( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SlotVarDeclare *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitVarDeclare( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( VarAssign *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetVariable() );
		Accept( node->GetAssignValue() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( FunkshunReturn *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetReturnValue() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( VisibleStatement *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		for( ExpressionListIterator it = node->GetExpressions().begin();
			 it != node->GetExpressions().end(); ++it ) {
			Accept( *it );
		}
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( GimmehStatement *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetTargetVariable() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( Program *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		for( FunkshunListIterator it = node->GetFunkshuns().begin();
			 it != node->GetFunkshuns().end(); ++it ) {
			Accept( *it );
		}
		Accept( node->GetBody() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SrsIdentifier *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		// TODO: Implement SRS traversal.
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SlotIdentifier *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetBukkitRef() );
		Accept( node->GetKey() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( CastExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetSourceExpr() );
		Accept( node->GetCastTargetType() );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::VisitUnaryExpression( UnaryExpression *node ) {
		assert( node != nullptr );
		Accept( node->GetOperand() );
	}
	
	void ASTProgramOrderVisitor::Visit( NotExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitUnaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( UppinExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitUnaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( NerfinExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitUnaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( TilExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitUnaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( WileExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitUnaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::VisitBinaryExpression( BinaryExpression *node ) {
		assert( node != nullptr );
		Accept( node->GetLeftOperand() );
		Accept( node->GetRightOperand() );
	}
	
	void ASTProgramOrderVisitor::Visit( SumExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( DiffExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( ProduktExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( QuoshuntExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( ModExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( BiggrExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SmallrExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( BothExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( EitherExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( WonExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SaemExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( DiffrintExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitBinaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::VisitNaryExpression( NaryExpression *node ) {
		assert( node != nullptr );
		for( ExpressionListIterator it = node->GetOperands().begin();
			 it != node->GetOperands().end(); ++it ) {
			Accept( *it ); 
		}
	}
	
	void ASTProgramOrderVisitor::Visit( AllExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitNaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( AnyExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitNaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( SmooshExpression *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		VisitNaryExpression( node );
		
		ProcessEnd( node );
	}
	
	void ASTProgramOrderVisitor::Visit( FunkshunCall *node ) {
		assert( node != nullptr );
		ProcessBegin( node );
		
		Accept( node->GetFunkshunName() );
		VisitNaryExpression( node );
		
		ProcessEnd( node );
	}
}