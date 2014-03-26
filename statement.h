#ifndef STATEMENT_H
#define STATEMENT_H

#include <list>

#include "ast.h"

namespace AST {
	class Statement;
	class Expression;
	class UnaryExpression;
	class UnaryBooleanExpression;
	class FunkshunCall;
	class StatementBlock;
	class Identifier;
	class LiteralIdentifier;
	class TypeIdentifier;
	class Literal;
	
	typedef std::list<Statement *> StatementList;
	typedef std::list<Expression *> ExpressionList;
	typedef std::list<Literal *> LiteralList;
	typedef std::list<LiteralIdentifier *> LiteralIdentifierList;
	typedef std::list<StatementBlock *> StatementBlockList;
	
	class Statement : public ASTNode {
	};
	
	class StatementBlock : public ASTNode {
		public:
			~StatementBlock();
			
			void AddStatement( Statement * );
			
		private:
			StatementList stmts_;
	};

	class Expression : public Statement {
		public:
			virtual Expression *Clone() = 0;
	};

	class ORlyBlock : public Statement {
		public:
			ORlyBlock( StatementBlock * );
			~ORlyBlock();
			
			void AddMeebeBlock( Expression *, StatementBlock * );
			void SetNoWaiBlock( StatementBlock * );
			
		private:
			StatementBlock *yaRlyBlock_;
			StatementBlock *noWaiBlock_;
			ExpressionList meebeConditions_;
			StatementBlockList meebeBlocks_;
	};

	class WtfBlock : public Statement {
		public:
			WtfBlock() : omgwtfBlock_( nullptr ) {}
			~WtfBlock();
			
			void AddWtfStmtBlock( Literal *, StatementBlock * );
			void SetOmgwtfBlock( StatementBlock * );
			
		private:
			LiteralList wtfConditions_;
			StatementBlockList wtfStmtBlocks_;
			StatementBlock *omgwtfBlock_;
	};

	class LoopBlock : public Statement {
		public:
			LoopBlock( LiteralIdentifier * );
			~LoopBlock();
			
			void SetBody( StatementBlock * );
			virtual void SetLoopVariable( Identifier *, bool );
			
		protected:
			LiteralIdentifier *loopId_;
			StatementBlock *body_;
			
			Identifier *loopVar_;
			bool loopVarIsLocal_;
	};
	
	class ForLoopBlock : public LoopBlock {
		public:
			ForLoopBlock( LiteralIdentifier * );
			~ForLoopBlock();
			
			void SetLoopVariable( Identifier *, bool );
			void SetLoopVariableIncExpr( Expression * );
			void SetLoopVariableInitExpr( Expression * );
			void SetLoopGuard( UnaryBooleanExpression * );
		
		private:
			Expression *loopVarIncExpr_;
			Expression *loopVarInitExpr_;
			UnaryBooleanExpression *loopGuard_;
	};
	
	class RangeLoopBlock : public LoopBlock {
		public:
			RangeLoopBlock( LiteralIdentifier * );
			~RangeLoopBlock();
			
			void SetBukkitRef( Expression * );
			
		private:
			Expression *bukkitRef_;
	};

	class FunkshunBlock : public Statement {
		public:
			FunkshunBlock( LiteralIdentifier * );
			~FunkshunBlock();
			
			void AddParameter( LiteralIdentifier * );
			void SetBody( StatementBlock * );
			
		private:
			LiteralIdentifier *funkshunId_;
			LiteralIdentifierList params_;
			StatementBlock *funkshunBody_;
	};

	class PlzBlock : public Statement {
		public:
			PlzBlock( StatementBlock * );
			~PlzBlock();
			
			void AddNoesBlock( Expression *, StatementBlock * );
			void SetWellBlock( StatementBlock * );
			
		private:
			StatementBlock *tryBlock_;
			ExpressionList exceptions_;
			StatementBlockList noesBlocks_;
			StatementBlock *wellBlock_;
	};

	class VarDeclare : public Statement {
		public:
			VarDeclare( Identifier * );
			~VarDeclare();
			
			void SetInitValue( Expression * );
			void SetInitType( TypeIdentifier * );
			
		private:
			Identifier *varId_;
			Expression *initVal_;
			TypeIdentifier *initType_;
	};

	class VarAssign : public Statement {
		public:
			VarAssign( Identifier * );
			~VarAssign();
			
			void SetAssignValue( Expression * );
			
		private:
			Identifier *varId_;
			Expression *assignVal_;
	};

	class VarCast : public Statement {
		public:
			VarCast( Identifier * );
			~VarCast();
			
			void SetCastTargetType( TypeIdentifier * );
			
		private:
			Identifier *varId_;
			TypeIdentifier *targetType_;
	};

	class FunkshunReturn : public Statement {
		public:
			FunkshunReturn( Expression * );
			~FunkshunReturn();
			
		private:
			Expression *retVal_;
	};

	class GtfoStatement : public Statement {
	};

	class VisibleStatement : public Statement {
		public:
			VisibleStatement( Expression * );
			~VisibleStatement();
			
			void AddExpression( Expression * );
			
		private:
			ExpressionList exprs_;
	};

	class GimmehStatement : public Statement {
		public:
			GimmehStatement( Identifier * );
			~GimmehStatement();
			
		private:
			Identifier *targetId_;
	};

	class Program : public ASTNode {
		public:
			Program( StatementBlock * );
			~Program() {
				delete stmtBlock_;
			}
			
			//inline void Accept( ASTVisitor &v ) { v.Visit( this ); }
		
		private:
			StatementBlock *stmtBlock_;
	};
}

#endif
