#ifndef CODEGEN_H
#define CODEGEN_H

#include "programvisitor.h"

class CodeGenerator : public AST::ASTProgramOrderVisitor {
	public:
		void ProcessBegin( StatementBlock * );
		void ProcessEnd( StatementBlock * );
		void ProcessBegin( ORlyBlock * );
		void ProcessEnd( ORlyBlock * );
		void ProcessBegin( WtfBlock * );
		void ProcessEnd( WtfBlock * );
		void ProcessBegin( ForLoopBlock * );
		void ProcessEnd( ForLoopBlock * );
		void ProcessBegin( RangeLoopBlock * );
		void ProcessEnd( RangeLoopBlock * );
		void ProcessBegin( FunkshunBlock * );
		void ProcessEnd( FunkshunBlock * );
		void ProcessBegin( FunkshunDeclare * );
		void ProcessEnd( FunkshunDeclare * );
		void ProcessBegin( PlzBlock * );
		void ProcessEnd( PlzBlock * );
		void ProcessBegin( VarDeclare * );
		void ProcessEnd( VarDeclare * );
		void ProcessBegin( VarAssign * );
		void ProcessEnd( VarAssign * );
		void ProcessBegin( VarCast * );
		void ProcessEnd( VarCast * );
		void ProcessBegin( FunkshunReturn * );
		void ProcessEnd( FunkshunReturn * );
		void Process( GtfoStatement * );
		void ProcessBegin( VisibleStatement * );
		void ProcessEnd( VisibleStatement * );
		void ProcessBegin( GimmehStatement * );
		void ProcessEnd( GimmehStatement * );
		void ProcessBegin( Program * );
		void ProcessEnd( Program * );
		
		void Process( TroofLiteral * );
		void Process( NumbrLiteral * );
		void Process( NumbarLiteral * );
		void Process( YarnLiteral * );
		void Process( NoobLiteral * );
		void Process( TypeIdentifier * );
		void Process( LiteralIdentifier * );
		void ProcessBegin( SrsIdentifier * );
		void ProcessEnd( SrsIdentifier * );
		void Process( ItIdentifier * );
		void ProcessBegin( SlotIdentifier * );
		void ProcessEnd( SlotIdentifier * );
		void ProcessBegin( CastExpression * );
		void ProcessEnd( CastExpression * );
		
		void ProcessBegin( NotExpression * );
		void ProcessEnd( NotExpression * );
		void ProcessBegin( UppinExpression * );
		void ProcessEnd( UppinExpression * );
		void ProcessBegin( NerfinExpression * );
		void ProcessEnd( NerfinExpression * );
		void ProcessBegin( TilExpression * );
		void ProcessEnd( TilExpression * );
		void ProcessBegin( WileExpression * );
		void ProcessEnd( WileExpression * );
		
		void ProcessBegin( SumExpression * );
		void ProcessEnd( SumExpression * );
		void ProcessBegin( DiffExpression * );
		void ProcessEnd( DiffExpression * );
		void ProcessBegin( ProduktExpression * );
		void ProcessEnd( ProduktExpression * );
		void ProcessBegin( QuoshuntExpression * );
		void ProcessEnd( QuoshuntExpression * );
		void ProcessBegin( ModExpression * );
		void ProcessEnd( ModExpression * );
		void ProcessBegin( BiggrExpression * );
		void ProcessEnd( BiggrExpression * );
		void ProcessBegin( SmallrExpression * );
		void ProcessEnd( SmallrExpression * );
		void ProcessBegin( BothExpression * );
		void ProcessEnd( BothExpression * );
		void ProcessBegin( EitherExpression * );
		void ProcessEnd( EitherExpression * );
		void ProcessBegin( WonExpression * );
		void ProcessEnd( WonExpression * );
		void ProcessBegin( SaemExpression * );
		void ProcessEnd( SaemExpression * );
		void ProcessBegin( DiffrintExpression * );
		void ProcessEnd( DiffrintExpression * );
		
		void ProcessBegin( AllExpression * );
		void ProcessEnd( AllExpression * );
		void ProcessBegin( AnyExpression * );
		void ProcessEnd( AnyExpression * );
		void ProcessBegin( SmooshExpression * );
		void ProcessEnd( SmooshExpression * );
		void ProcessBegin( FunkshunCall * );
		void ProcessEnd( FunkshunCall * );
};

#endif