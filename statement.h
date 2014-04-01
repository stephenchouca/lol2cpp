#ifndef STATEMENT_H
#define STATEMENT_H

#include <list>
#include <iostream>

#include "ast.h"

namespace AST {
	class StatementBlock;
	class Statement;
	class Expression;
	class UnaryExpression;
	class UnaryBooleanExpression;
	class FunkshunCall;
	class Identifier;
	class LiteralIdentifier;
	class TypeIdentifier;
	class Literal;
	
	typedef std::list<Statement *> StatementList;
	typedef std::list<Expression *> ExpressionList;
	typedef std::list<Literal *> LiteralList;
	typedef std::list<LiteralIdentifier *> LiteralIdentifierList;
	typedef std::list<StatementBlock *> StatementBlockList;
	
	typedef std::list<Statement *>::const_iterator StatementListIterator;
	typedef std::list<Expression *>::const_iterator ExpressionListIterator;
	typedef std::list<Literal *>::const_iterator LiteralListIterator;
	typedef std::list<LiteralIdentifier *>::const_iterator 
		LiteralIdentifierListIterator;
	typedef std::list<StatementBlock *>::const_iterator StatementBlockListIterator;
	
	class Statement : public ASTNode {
	};
	
	class StatementBlock : public ASTNode {
		public:
			~StatementBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 0; }
			
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
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void AddMeebeBlock( Expression *, StatementBlock * );
			void SetNoWaiBlock( StatementBlock * );
			
		private:
			StatementBlock *yaRlyBlock_;
			ExpressionList meebeConditions_;
			StatementBlockList meebeBlocks_;
			StatementBlock *noWaiBlock_;
	};

	class WtfBlock : public Statement {
		public:
			WtfBlock() : omgwtfBlock_( nullptr ) {}
			~WtfBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void AddOmgBlock( Literal *, StatementBlock * );
			void SetOmgwtfBlock( StatementBlock * );
			
		private:
			LiteralList omgLabels_;
			StatementBlockList omgStmtBlocks_;
			StatementBlock *omgwtfBlock_;
	};

	class LoopBlock : public Statement {
		public:
			LoopBlock( LiteralIdentifier * );
			~LoopBlock();
			
			void Print( std::ostream & );
			virtual void PrintLoopSpecific( std::ostream & ) = 0;
			virtual std::string GetLoopType() = 0;
			unsigned int GetWidth() { return 2; }
			
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
			
			void PrintLoopSpecific( std::ostream & );
			std::string GetLoopType() { return "FOR LOOP"; }
			
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
			
			void PrintLoopSpecific( std::ostream & );
			std::string GetLoopType() { return "RANGE LOOP"; }
			
			void SetBukkitRef( Expression * );
			
		private:
			Expression *bukkitRef_;
	};

	class FunkshunBlock : public Statement {
		public:
			FunkshunBlock( LiteralIdentifier * );
			~FunkshunBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void AddParameter( LiteralIdentifier * );
			void SetBody( StatementBlock * );
			
		private:
			LiteralIdentifier *funkshunId_;
			LiteralIdentifierList params_;
			StatementBlock *body_;
	};

	class PlzBlock : public Statement {
		public:
			PlzBlock( StatementBlock * );
			~PlzBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void AddNoesBlock( Expression *, StatementBlock * );
			void SetWellBlock( StatementBlock * );
			
		private:
			StatementBlock *tryBlock_;
			ExpressionList exceptions_;
			StatementBlockList handlers_;
			StatementBlock *wellBlock_;
	};

	class VarDeclare : public Statement {
		public:
			VarDeclare( Identifier * );
			~VarDeclare();
			
			void Print( std::ostream & );
			
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
			
			void Print( std::ostream & );
			
			void SetAssignValue( Expression * );
			
		private:
			Identifier *varId_;
			Expression *assignVal_;
	};

	class VarCast : public Statement {
		public:
			VarCast( Identifier * );
			~VarCast();
			
			void Print( std::ostream & );
			
			void SetCastTargetType( TypeIdentifier * );
			
		private:
			Identifier *varId_;
			TypeIdentifier *targetType_;
	};

	class FunkshunReturn : public Statement {
		public:
			FunkshunReturn( Expression * );
			~FunkshunReturn();
			
			void Print( std::ostream & );
			
		private:
			Expression *retVal_;
	};

	class GtfoStatement : public Statement {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "GTFO";
			}
	};

	class VisibleStatement : public Statement {
		public:
			VisibleStatement( Expression * );
			~VisibleStatement();
			
			void Print( std::ostream & );
			
			void AddExpression( Expression * );
			
		private:
			ExpressionList exprs_;
	};

	class GimmehStatement : public Statement {
		public:
			GimmehStatement( Identifier *, bool );
			~GimmehStatement();
			
			void Print( std::ostream & );
			
		private:
			Identifier *targetId_;
			bool isLong_;
	};

	class Program : public ASTNode {
		public:
			Program( StatementBlock * );
			~Program() {
				delete stmtBlock_;
			}
			
			void Print( std::ostream & );
			
			//inline void Accept( ASTVisitor &v ) { v.Visit( this ); }
		
		private:
			StatementBlock *stmtBlock_;
	};
}

#endif
