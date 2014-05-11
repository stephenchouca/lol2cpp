#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

namespace AST {
	class StatementBlock;
	class ORlyBlock;
	class WtfBlock;
	class ForLoopBlock;
	class RangeLoopBlock;
	class FunkshunBlock;
	class FunkshunDeclare;
	class PlzBlock;
	class LiteralVarDeclare;
	class SrsVarDeclare;
	class SlotVarDeclare;
	class VarAssign;
	class FunkshunReturn;
	class GtfoStatement;
	class WhatevrStatement;
	class VisibleStatement;
	class GimmehStatement;
	class Program;
	
	class TroofLiteral;
	class NumbrLiteral;
	class NumbarLiteral;
	class YarnLiteral;
	class NoobLiteral;
	class TypeIdentifier;
	class LiteralIdentifier;
	class SrsIdentifier;
	class ItIdentifier;
	class SlotIdentifier;
	class NotExpression;
	class UppinExpression;
	class NerfinExpression;
	class TilExpression;
	class WileExpression;
	class SumExpression;
	class DiffExpression;
	class ProduktExpression;
	class QuoshuntExpression;
	class ModExpression;
	class BiggrExpression;
	class SmallrExpression;
	class BothExpression;
	class EitherExpression;
	class WonExpression;
	class SaemExpression;
	class DiffrintExpression;
	class AllExpression;
	class AnyExpression;
	class SmooshExpression;
	class FunkshunCall;
	class MaekExpression;
	
	class ASTVisitor {
		public:
			virtual void ProcessBegin( StatementBlock *node ) {}
			virtual void ProcessEnd( StatementBlock *node ) {}
			virtual void ProcessBegin( ORlyBlock *node ) {}
			virtual void ProcessEnd( ORlyBlock *node ) {}
			virtual void ProcessBegin( WtfBlock *node ) {}
			virtual void ProcessEnd( WtfBlock *node ) {}
			virtual void ProcessBegin( ForLoopBlock *node ) {}
			virtual void ProcessEnd( ForLoopBlock *node ) {}
			virtual void ProcessBegin( RangeLoopBlock *node ) {}
			virtual void ProcessEnd( RangeLoopBlock *node ) {}
			virtual void ProcessBegin( FunkshunBlock *node ) {}
			virtual void ProcessEnd( FunkshunBlock *node ) {}
			virtual void ProcessBegin( FunkshunDeclare *node ) {}
			virtual void ProcessEnd( FunkshunDeclare *node ) {}
			virtual void ProcessBegin( PlzBlock *node ) {}
			virtual void ProcessEnd( PlzBlock *node ) {}
			virtual void ProcessBegin( LiteralVarDeclare *node ) {}
			virtual void ProcessEnd( LiteralVarDeclare *node ) {}
			virtual void ProcessBegin( SrsVarDeclare *node ) {}
			virtual void ProcessEnd( SrsVarDeclare *node ) {}
			virtual void ProcessBegin( SlotVarDeclare *node ) {}
			virtual void ProcessEnd( SlotVarDeclare *node ) {}
			virtual void ProcessBegin( VarAssign *node ) {}
			virtual void ProcessEnd( VarAssign *node ) {}
			virtual void ProcessBegin( FunkshunReturn *node ) {}
			virtual void ProcessEnd( FunkshunReturn *node ) {}
			virtual void Process( GtfoStatement *node ) {}
			virtual void Process( WhatevrStatement *node ) {}
			virtual void ProcessBegin( VisibleStatement *node ) {}
			virtual void ProcessEnd( VisibleStatement *node ) {}
			virtual void ProcessBegin( GimmehStatement *node ) {}
			virtual void ProcessEnd( GimmehStatement *node ) {}
			virtual void ProcessBegin( Program *node ) {}
			virtual void ProcessEnd( Program *node ) {}
			
			virtual void Process( TroofLiteral *node ) {}
			virtual void Process( NumbrLiteral *node ) {}
			virtual void Process( NumbarLiteral *node ) {}
			virtual void Process( YarnLiteral *node ) {}
			virtual void Process( NoobLiteral *node ) {}
			virtual void Process( TypeIdentifier *node ) {}
			virtual void Process( LiteralIdentifier *node ) {}
			virtual void ProcessBegin( SrsIdentifier *node ) {}
			virtual void ProcessEnd( SrsIdentifier *node ) {}
			virtual void Process( ItIdentifier *node ) {}
			virtual void ProcessBegin( SlotIdentifier *node ) {}
			virtual void ProcessEnd( SlotIdentifier *node ) {}
			virtual void ProcessBegin( MaekExpression *node ) {}
			virtual void ProcessEnd( MaekExpression *node ) {}
			
			virtual void ProcessBegin( NotExpression *node ) {}
			virtual void ProcessEnd( NotExpression *node ) {}
			virtual void ProcessBegin( UppinExpression *node ) {}
			virtual void ProcessEnd( UppinExpression *node ) {}
			virtual void ProcessBegin( NerfinExpression *node ) {}
			virtual void ProcessEnd( NerfinExpression *node ) {}
			virtual void ProcessBegin( TilExpression *node ) {}
			virtual void ProcessEnd( TilExpression *node ) {}
			virtual void ProcessBegin( WileExpression *node ) {}
			virtual void ProcessEnd( WileExpression *node ) {}
			
			virtual void ProcessBegin( SumExpression *node ) {}
			virtual void ProcessEnd( SumExpression *node ) {}
			virtual void ProcessBegin( DiffExpression *node ) {}
			virtual void ProcessEnd( DiffExpression *node ) {}
			virtual void ProcessBegin( ProduktExpression *node ) {}
			virtual void ProcessEnd( ProduktExpression *node ) {}
			virtual void ProcessBegin( QuoshuntExpression *node ) {}
			virtual void ProcessEnd( QuoshuntExpression *node ) {}
			virtual void ProcessBegin( ModExpression *node ) {}
			virtual void ProcessEnd( ModExpression *node ) {}
			virtual void ProcessBegin( BiggrExpression *node ) {}
			virtual void ProcessEnd( BiggrExpression *node ) {}
			virtual void ProcessBegin( SmallrExpression *node ) {}
			virtual void ProcessEnd( SmallrExpression *node ) {}
			virtual void ProcessBegin( BothExpression *node ) {}
			virtual void ProcessEnd( BothExpression *node ) {}
			virtual void ProcessBegin( EitherExpression *node ) {}
			virtual void ProcessEnd( EitherExpression *node ) {}
			virtual void ProcessBegin( WonExpression *node ) {}
			virtual void ProcessEnd( WonExpression *node ) {}
			virtual void ProcessBegin( SaemExpression *node ) {}
			virtual void ProcessEnd( SaemExpression *node ) {}
			virtual void ProcessBegin( DiffrintExpression *node ) {}
			virtual void ProcessEnd( DiffrintExpression *node ) {}
			
			virtual void ProcessBegin( AllExpression *node ) {}
			virtual void ProcessEnd( AllExpression *node ) {}
			virtual void ProcessBegin( AnyExpression *node ) {}
			virtual void ProcessEnd( AnyExpression *node ) {}
			virtual void ProcessBegin( SmooshExpression *node ) {}
			virtual void ProcessEnd( SmooshExpression *node ) {}
			virtual void ProcessBegin( FunkshunCall *node ) {}
			virtual void ProcessEnd( FunkshunCall *node ) {}
			
			virtual void Visit( StatementBlock * ) = 0;
			virtual void Visit( ORlyBlock * ) = 0;
			virtual void Visit( WtfBlock * ) = 0;
			virtual void Visit( ForLoopBlock * ) = 0;
			virtual void Visit( RangeLoopBlock * ) = 0;
			virtual void Visit( FunkshunBlock * ) = 0;
			virtual void Visit( FunkshunDeclare * ) = 0;
			virtual void Visit( PlzBlock * ) = 0;
			virtual void Visit( LiteralVarDeclare * ) = 0;
			virtual void Visit( SrsVarDeclare * ) = 0;
			virtual void Visit( SlotVarDeclare * ) = 0;
			virtual void Visit( VarAssign * ) = 0;
			virtual void Visit( FunkshunReturn * ) = 0;
			virtual void Visit( GtfoStatement *node ) { Process( node ); }
			virtual void Visit( WhatevrStatement *node ) { Process( node ); }
			virtual void Visit( VisibleStatement * ) = 0;
			virtual void Visit( GimmehStatement * ) = 0;
			virtual void Visit( Program * ) = 0;
			
			virtual void Visit( TroofLiteral *node ) { Process( node ); }
			virtual void Visit( NumbrLiteral *node ) { Process( node ); }
			virtual void Visit( NumbarLiteral *node ) { Process( node ); }
			virtual void Visit( YarnLiteral *node ) { Process( node ); }
			virtual void Visit( NoobLiteral *node ) { Process( node ); }
			virtual void Visit( TypeIdentifier *node ) { Process( node ); }
			virtual void Visit( LiteralIdentifier *node ) { Process( node ); }
			virtual void Visit( SrsIdentifier * ) = 0;
			virtual void Visit( ItIdentifier *node ) { Process( node ); }
			virtual void Visit( SlotIdentifier * ) = 0;
			virtual void Visit( MaekExpression * ) = 0;
			
			virtual void Visit( NotExpression * ) = 0;
			virtual void Visit( UppinExpression * ) = 0;
			virtual void Visit( NerfinExpression * ) = 0;
			virtual void Visit( TilExpression * ) = 0;
			virtual void Visit( WileExpression * ) = 0;
			
			virtual void Visit( SumExpression * ) = 0;
			virtual void Visit( DiffExpression * ) = 0;
			virtual void Visit( ProduktExpression * ) = 0;
			virtual void Visit( QuoshuntExpression * ) = 0;
			virtual void Visit( ModExpression * ) = 0;
			virtual void Visit( BiggrExpression * ) = 0;
			virtual void Visit( SmallrExpression * ) = 0;
			virtual void Visit( BothExpression * ) = 0;
			virtual void Visit( EitherExpression * ) = 0;
			virtual void Visit( WonExpression * ) = 0;
			virtual void Visit( SaemExpression * ) = 0;
			virtual void Visit( DiffrintExpression * ) = 0;
			
			virtual void Visit( AllExpression * ) = 0;
			virtual void Visit( AnyExpression * ) = 0;
			virtual void Visit( SmooshExpression * ) = 0;
			virtual void Visit( FunkshunCall * ) = 0;
		
		protected:
			void Accept( ASTNode *node ) {
				if( node != nullptr ) {
					node->Accept( this );
				}
			}
	};
}

#endif

