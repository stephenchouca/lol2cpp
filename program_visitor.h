#ifndef PROGRAM_VISITOR_H
#define PROGRAM_VISITOR_H

#include "visitor.h"
#include "statement.h"
#include "expression.h"

namespace AST {
	class ASTProgramOrderVisitor : public ASTVisitor {
		public:
			void Visit( StatementBlock * );
			void Visit( ORlyBlock * );
			void Visit( WtfBlock * );
			void Visit( ForLoopBlock * );
			void Visit( RangeLoopBlock * );
			void Visit( FunkshunBlock * );
			void Visit( FunkshunDeclare * );
			void Visit( PlzBlock * );
			void Visit( LiteralVarDeclare * );
			void Visit( SrsVarDeclare * );
			void Visit( SlotVarDeclare * );
			void Visit( VarAssign * );
			void Visit( FunkshunReturn * );
			void Visit( VisibleStatement * );
			void Visit( GimmehStatement * );
			void Visit( Program * );
			
			void Visit( SrsIdentifier * );
			void Visit( SlotIdentifier * );
			void Visit( MaekExpression * );
			
			void Visit( NotExpression * );
			void Visit( UppinExpression * );
			void Visit( NerfinExpression * );
			void Visit( TilExpression * );
			void Visit( WileExpression * );
			
			void Visit( SumExpression * );
			void Visit( DiffExpression * );
			void Visit( ProduktExpression * );
			void Visit( QuoshuntExpression * );
			void Visit( ModExpression * );
			void Visit( BiggrExpression * );
			void Visit( SmallrExpression * );
			void Visit( BothExpression * );
			void Visit( EitherExpression * );
			void Visit( WonExpression * );
			void Visit( SaemExpression * );
			void Visit( DiffrintExpression * );
			
			void Visit( AllExpression * );
			void Visit( AnyExpression * );
			void Visit( SmooshExpression * );
			void Visit( FunkshunCall * );
			
		private:
			void VisitVarDeclare( VarDeclare * );
			void VisitUnaryExpression( UnaryExpression * );
			void VisitBinaryExpression( BinaryExpression * );
			void VisitNaryExpression( NaryExpression * );
	};
}

#endif
