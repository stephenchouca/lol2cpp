#ifndef STATEMENT_H
#define STATEMENT_H

#include <list>
#include <utility>
#include <iostream>

#include "ast.h"
#include "expression.h"
#include "visitor.h"

namespace AST {
	class Statement;
	class FunkshunBlock;
	
	typedef std::pair<Expression *, StatementBlock *> MebbeBlock;
	typedef std::pair<Literal *, StatementBlock *> OmgBlock;
	typedef std::pair<Expression *, StatementBlock *> NoesBlock;
	typedef std::pair<LiteralIdentifier *, bool> FunkshunParam;
	
	typedef std::list<Statement *> StatementList;
	typedef std::list<FunkshunBlock *> FunkshunList;
	typedef std::list<MebbeBlock> MebbeBlockList;
	typedef std::list<OmgBlock> OmgBlockList;
	typedef std::list<NoesBlock> NoesBlockList;
	typedef std::list<FunkshunParam> FunkshunParamList;
	
	typedef std::list<Statement *>::iterator StatementListIterator;
	typedef std::list<FunkshunBlock *>::iterator FunkshunListIterator;
	typedef std::list<MebbeBlock>::iterator MebbeBlockListIterator;
	typedef std::list<OmgBlock>::iterator OmgBlockListIterator;
	typedef std::list<NoesBlock>::iterator NoesBlockListIterator;
	typedef std::list<FunkshunParam>::iterator FunkshunParamListIterator;
	
	class Statement : public ASTNode {};
	
	class StatementBlock : public ASTNode {
		public:
			~StatementBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 0; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			StatementList &GetStatements() { return stmts_; }
			
			void AddStatement( Statement * );
			void SwapStatement( StatementListIterator &, Statement * );
			
		private:
			StatementList stmts_;
	};

	class ORlyBlock : public Statement {
		public:
			ORlyBlock( StatementBlock * );
			~ORlyBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			StatementBlock *GetYaRlyBody() { return yaRlyBody_; }
			MebbeBlockList &GetMebbeBlocks() { return mebbeBlocks_; }
			StatementBlock *GetNoWaiBody() { return noWaiBody_; }
			
			void AddMebbeBlock( Expression *, StatementBlock * );
			void SetNoWaiBody( StatementBlock * );
			
		private:
			StatementBlock *yaRlyBody_;
			MebbeBlockList mebbeBlocks_;
			StatementBlock *noWaiBody_;
	};

	class WtfBlock : public Statement {
		public:
			WtfBlock() : omgwtfBody_( nullptr ) {}
			~WtfBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			OmgBlockList &GetOmgBlocks() { return omgBlocks_; }
			StatementBlock *GetOmgwtfBody() { return omgwtfBody_; }
			
			void AddOmgBlock( Literal *, StatementBlock * );
			void SetOmgwtfBody( StatementBlock * );
			
		private:
			OmgBlockList omgBlocks_;
			StatementBlock *omgwtfBody_;
	};

	class LoopBlock : public Statement {
		public:
			LoopBlock( LiteralIdentifier * );
			~LoopBlock();
			
			void Print( std::ostream & );
			virtual void PrintLoopSpecific( std::ostream & ) = 0;
			virtual std::string GetLoopType() = 0;
			unsigned int GetWidth() { return 2; }
			
			LiteralIdentifier *GetLabel() { return label_; }
			StatementBlock *GetBody() { return body_; }
			Identifier *GetLoopVariable() { return loopVar_; }
			bool GetLoopVariableIsLocal() { return loopVarIsLocal_; }
			
			void SetBody( StatementBlock * );
			virtual void SetLoopVariable( Identifier *, bool );
			
		protected:
			LiteralIdentifier *label_;
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
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Expression *GetLoopVariableIncExpr() { return loopVarIncExpr_; }
			Expression *GetLoopVariableInitExpr() { return loopVarInitExpr_; }
			UnaryExpression *GetLoopGuard() { return loopGuard_; }
			
			void SetLoopVariableIncExpr( Expression * );
			void SetLoopVariableInitExpr( Expression * );
			void SetLoopGuard( UnaryExpression * );
		
		private:
			Expression *loopVarIncExpr_;
			Expression *loopVarInitExpr_;
			UnaryExpression *loopGuard_;
	};
	
	class RangeLoopBlock : public LoopBlock {
		public:
			RangeLoopBlock( LiteralIdentifier * );
			~RangeLoopBlock();
			
			void PrintLoopSpecific( std::ostream & );
			std::string GetLoopType() { return "RANGE LOOP"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Expression *GetBukkitRef() { return bukkitRef_; }
			
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
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			LiteralIdentifier *GetName() { return name_; }
			FunkshunParamList &GetParameters() { return params_; }
			StatementBlock *GetBody() { return body_; }
			
			void AddParameter( LiteralIdentifier *, bool );
			void SetBody( StatementBlock * );
			
		private:
			LiteralIdentifier *name_;
			FunkshunParamList params_;
			StatementBlock *body_;
	};
	
	class FunkshunDeclare : public Statement {
		public:
			FunkshunDeclare( LiteralIdentifier * );
			~FunkshunDeclare();
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			LiteralIdentifier *GetFunkshunName() { return funkshunName_; }
		
		private:
			LiteralIdentifier *funkshunName_;
	};

	class PlzBlock : public Statement {
		public:
			PlzBlock( StatementBlock * );
			~PlzBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			void AddONoesBlock( Expression *, StatementBlock * );
			void SetOWelBody( StatementBlock * );
			
		private:
			StatementBlock *tryBody_;
			NoesBlockList oNoesBlocks_;
			StatementBlock *oWelBody_;
	};

	class VarDeclare : public Statement {
		public:
			VarDeclare() : 
				initVal_( nullptr ), 
				initType_( nullptr ) {}
			~VarDeclare();
			
			void Print( std::ostream & );
			
			virtual Identifier *GetVariable() = 0;
			Expression *GetInitValue() { return initVal_; }
			TypeIdentifier *GetInitType() { return initType_; }
			
			void SetInitValue( Expression * );
			void SetInitType( TypeIdentifier * );
			
		protected:
			Expression *initVal_;
			TypeIdentifier *initType_;
	};
	
	class LiteralVarDeclare : public VarDeclare {
		public:
			LiteralVarDeclare( LiteralIdentifier * );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			LiteralIdentifier *GetVariable() { return varId_; }
			
		private:
			LiteralIdentifier *varId_;
	};
	
	class SrsVarDeclare : public VarDeclare {
		public:
			SrsVarDeclare( SrsIdentifier * );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SrsIdentifier *GetVariable() { return varId_; }
		
		private:
			SrsIdentifier *varId_;
	};
	
	class SlotVarDeclare : public VarDeclare {
		public:
			SlotVarDeclare( SlotIdentifier * );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SlotIdentifier *GetVariable() { return varId_; }
			
		private:
			SlotIdentifier *varId_;
	};

	class VarAssign : public Statement {
		public:
			VarAssign( Identifier * );
			~VarAssign();
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			static VarAssign *CreateImplicitAssign() {
				return new VarAssign( new ItIdentifier() );
			}
			
			Identifier *GetVariable() { return varId_; }
			Expression *GetAssignValue() { return assignVal_; }
			
			void SetAssignValue( Expression * );
			
		private:
			Identifier *varId_;
			Expression *assignVal_;
	};

	class FunkshunReturn : public Statement {
		public:
			FunkshunReturn( Expression * );
			~FunkshunReturn();
			
			void Print( std::ostream & );
			
			Expression *GetReturnValue() { return retVal_; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
		private:
			Expression *retVal_;
	};

	class GtfoStatement : public Statement {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "GTFO";
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class WhatevrStatement : public Statement {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "WHATEVR";
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};

	class VisibleStatement : public Statement {
		public:
			VisibleStatement( Expression * );
			~VisibleStatement();
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ExpressionList &GetExpressions() { return exprs_; }
			bool GetSuppressNewLine() { return suppressNewline_; }
			
			void AddExpression( Expression * );
			void SetSuppressNewline() { suppressNewline_ = true; }
			
		private:
			ExpressionList exprs_;
			bool suppressNewline_;
	};

	class GimmehStatement : public Statement {
		public:
			GimmehStatement( Identifier *, bool );
			~GimmehStatement();
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Identifier *GetTargetVariable() { return targetVar_; }
			bool IsLong() { return isLong_; }
			
		private:
			Identifier *targetVar_;
			bool isLong_;
	};

	class Program : public ASTNode {
		public:
			Program( FunkshunList &, StatementBlock * );
			~Program();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			FunkshunList &GetFunkshuns() { return funkshuns_; }
			StatementBlock *GetBody() { return body_; }
		
		private:
			FunkshunList funkshuns_;
			StatementBlock *body_;
	};
}

#endif
