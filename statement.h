#ifndef STATEMENT_H
#define STATEMENT_H

#include <list>
#include <utility>
#include <iostream>

#include "ast.h"
#include "visitor.h"

namespace AST {
	class ORlyMebbeBody;
	class WtfOmgBody;
	class PlzONoesBody;
	class Statement;
	class Expression;
	class UnaryExpression;
	class FunkshunCall;
	class Identifier;
	class LiteralIdentifier;
	class TypeIdentifier;
	class Literal;
	
	typedef std::pair<Expression *, ORlyMebbeBody *> MebbeBlock;
	typedef std::pair<Literal *, WtfOmgBody *> OmgBlock;
	typedef std::pair<Expression *, PlzONoesBody *> NoesBlock;
	typedef std::pair<LiteralIdentifier *, bool> FunkshunParam;
	
	typedef std::list<Statement *> StatementList;
	typedef std::list<Expression *> ExpressionList;
	typedef std::list<Literal *> LiteralList;
	typedef std::list<MebbeBlock> MebbeBlockList;
	typedef std::list<OmgBlock> OmgBlockList;
	typedef std::list<NoesBlock> NoesBlockList;
	typedef std::list<FunkshunParam> FunkshunParamList;
	
	typedef std::list<Statement *>::iterator StatementListIterator;
	typedef std::list<Expression *>::iterator ExpressionListIterator;
	typedef std::list<Literal *>::iterator LiteralListIterator;
	typedef std::list<MebbeBlock>::iterator MebbeBlockListIterator;
	typedef std::list<OmgBlock>::iterator OmgBlockListIterator;
	typedef std::list<NoesBlock>::iterator NoesBlockListIterator;
	typedef std::list<FunkshunParam>::iterator FunkshunParamListIterator;
	
	class Statement : public ASTNode {
	};
	
	class StatementBlock : public ASTNode {
		public:
			~StatementBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 0; }
			
			StatementList &GetStatements() { return stmts_; }
			
			void AddStatement( Statement * );
			void SwapStatement( StatementListIterator &, Statement * );
			
		private:
			StatementList stmts_;
	};
	
	class ProgramBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class ProgramGlobals : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class FunkshunBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class ORlyYaBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class ORlyMebbeBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class ORlyNoBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class WtfOmgBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class WtfOmgwtfBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class LoopBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class PlzBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class PlzONoesBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};
	
	class PlzOWelBody : public StatementBlock {
		public:
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
	};

	class Expression : public Statement {
		public:
			virtual Expression *Clone() = 0;
	};

	class ORlyBlock : public Statement {
		public:
			ORlyBlock( ORlyYaBody * );
			~ORlyBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ORlyYaBody *GetYaRlyBody() { return yaRlyBody_; }
			MebbeBlockList &GetMebbeBlocks() { return mebbeBlocks_; }
			ORlyNoBody *GetNoWaiBody() { return noWaiBody_; }
			
			void AddMebbeBlock( Expression *, ORlyMebbeBody * );
			void SetNoWaiBody( ORlyNoBody * );
			
		private:
			ORlyYaBody *yaRlyBody_;
			MebbeBlockList mebbeBlocks_;
			ORlyNoBody *noWaiBody_;
	};

	class WtfBlock : public Statement {
		public:
			WtfBlock() : omgwtfBody_( nullptr ) {}
			~WtfBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			OmgBlockList &GetOmgBlocks() { return omgBlocks_; }
			WtfOmgwtfBody *GetOmgwtfBody() { return omgwtfBody_; }
			
			void AddOmgBlock( Literal *, WtfOmgBody * );
			void SetOmgwtfBody( WtfOmgwtfBody * );
			
		private:
			OmgBlockList omgBlocks_;
			WtfOmgwtfBody *omgwtfBody_;
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
			LoopBody *GetBody() { return body_; }
			Identifier *GetLoopVariable() { return loopVar_; }
			bool GetLoopVariableIsLocal() { return loopVarIsLocal_; }
			
			void SetBody( LoopBody * );
			virtual void SetLoopVariable( Identifier *, bool );
			
		protected:
			LiteralIdentifier *label_;
			LoopBody *body_;
			
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
			
			void SetLoopVariable( Identifier *, bool );
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
			FunkshunBody *GetBody() { return body_; }
			
			void AddParameter( LiteralIdentifier *, bool );
			void SetBody( FunkshunBody * );
			
		private:
			LiteralIdentifier *name_;
			FunkshunParamList params_;
			FunkshunBody *body_;
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
			PlzBlock( PlzBody * );
			~PlzBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			void AddONoesBlock( Expression *, PlzONoesBody * );
			void SetOWelBody( PlzOWelBody * );
			
		private:
			PlzBody *tryBody_;
			NoesBlockList oNoesBlocks_;
			PlzOWelBody *oWelBody_;
	};

	class VarDeclare : public Statement {
		public:
			VarDeclare( Identifier * );
			~VarDeclare();
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Identifier *GetVariable() { return varId_; }
			Expression *GetInitValue() { return initVal_; }
			TypeIdentifier *GetInitType() { return initType_; }
			
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
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Identifier *GetVariable() { return varId_; }
			Expression *GetAssignValue() { return assignVal_; }
			
			void SetAssignValue( Expression * );
			
		private:
			Identifier *varId_;
			Expression *assignVal_;
	};

#if 0
	class VarCast : public Statement {
		public:
			VarCast( Identifier * );
			~VarCast();
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Identifier *GetVariable() { return varId_; }
			TypeIdentifier *GetCastTargetType() { return targetType_; }
			
			void SetCastTargetType( TypeIdentifier * );
			
		private:
			Identifier *varId_;
			TypeIdentifier *targetType_;
	};
#endif

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
			Program( ProgramBody * );
			~Program() {
				delete globals_;
				delete body_;
			}
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ProgramGlobals *GetGlobals() { return globals_; }
			ProgramBody *GetBody() { return body_; }
		
		private:
			ProgramGlobals *globals_;
			ProgramBody *body_;
	};
}

#endif
