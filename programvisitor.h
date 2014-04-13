#ifndef PROGRAMVISITOR_H
#define PROGRAMVISITOR_H

#include "visitor.h"
#include "statement.h"
#include "expression.h"

namespace AST {
	class ASTProgramOrderVisitor : public ASTVisitor {
		public:
			void Visit( ProgramBody * );
			void Visit( ProgramGlobals * );
			void Visit( FunkshunBody * );
			void Visit( ORlyYaBody * );
			void Visit( ORlyMebbeBody * );
			void Visit( ORlyNoBody * );
			void Visit( WtfOmgBody * );
			void Visit( WtfOmgwtfBody * );
			void Visit( LoopBody * );
			void Visit( PlzBody * );
			void Visit( PlzONoesBody * );
			void Visit( PlzOWelBody * );
			
			void Visit( ORlyBlock * );
			void Visit( WtfBlock * );
			void Visit( ForLoopBlock * );
			void Visit( RangeLoopBlock * );
			void Visit( FunkshunBlock * );
			void Visit( FunkshunDeclare * );
			void Visit( PlzBlock * );
			void Visit( VarDeclare * );
			void Visit( VarAssign * );
			//void Visit( VarCast * );
			void Visit( FunkshunReturn * );
			void Visit( VisibleStatement * );
			void Visit( GimmehStatement * );
			void Visit( Program * );
			
			void Visit( SrsIdentifier * );
			void Visit( SlotIdentifier * );
			void Visit( CastExpression * );
			
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
			void VisitStatementBlock( StatementBlock * );
			void VisitUnaryExpression( UnaryExpression * );
			void VisitBinaryExpression( BinaryExpression * );
			void VisitNaryExpression( NaryExpression * );
	};
}

#endif
