#ifndef STATEMENT_H
#define STATEMENT_H

#include <list>
#include <utility>
#include <iostream>

#include "ast.h"
#include "visitor.h"

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
	
	typedef std::pair<Expression *, StatementBlock *> MebbeBlock;
	typedef std::pair<Literal *, StatementBlock *> OmgBlock;
	typedef std::pair<Expression *, StatementBlock *> NoesBlock;
	typedef std::pair<LiteralIdentifier *, bool> FunkshunParam;
	
	typedef std::list<Statement *> StatementList;
	typedef std::list<Expression *> ExpressionList;
	typedef std::list<Literal *> LiteralList;
	typedef std::list<StatementBlock *> StatementBlockList;
	typedef std::list<MebbeBlock> MebbeBlockList;
	typedef std::list<OmgBlock> OmgBlockList;
	typedef std::list<NoesBlock> NoesBlockList;
	typedef std::list<FunkshunParam> FunkshunParamList;
	
	typedef std::list<Statement *>::iterator StatementListIterator;
	typedef std::list<Expression *>::iterator ExpressionListIterator;
	typedef std::list<Literal *>::iterator LiteralListIterator;
	typedef std::list<StatementBlock *>::iterator StatementBlockListIterator;
	typedef std::list<MebbeBlock>::iterator MebbeBlockListIterator;
	typedef std::list<OmgBlock>::iterator OmgBlockListIterator;
	typedef std::list<NoesBlock>::iterator NoesBlockListIterator;
	typedef std::list<FunkshunParam>::iterator FunkshunParamListIterator;
	
	class Statement : public ASTNode {
	};
	
	class StatementBlock : public ASTNode {
		public:
			enum class Type { 
				PROGRAM_BODY,
				PROGRAM_GLOBALS,
				FUNKSHUN_BODY,
				ORLY_YA,
				ORLY_MEBBE,
				ORLY_NO,
				WTF_OMG,
				WTF_OMGWTF,
				LOOP_BODY,
				PLZ_BODY,
				PLZ_ONOES,
				PLZ_OWEL
			};
			
		public:
			StatementBlock( Type type ) : type_( type ) {}
			~StatementBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 0; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			Type GetType() { return type_; }
			StatementList &GetStatements() { return stmts_; }
			
			void AddStatement( Statement * );
			void SwapStatement( StatementListIterator &, Statement * );
			
		private:
			Type type_;
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
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			StatementBlock *GetYaRlyBlock() { return yaRlyBlock_; }
			MebbeBlockList &GetMebbeBlocks() { return mebbeBlocks_; }
			StatementBlock *GetNoWaiBlock() { return noWaiBlock_; }
			
			void AddMebbeBlock( Expression *, StatementBlock * );
			void SetNoWaiBlock( StatementBlock * );
			
		private:
			StatementBlock *yaRlyBlock_;
			MebbeBlockList mebbeBlocks_;
			StatementBlock *noWaiBlock_;
	};

	class WtfBlock : public Statement {
		public:
			WtfBlock() : omgwtfBlock_( nullptr ) {}
			~WtfBlock();
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 3; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			OmgBlockList &GetOmgBlocks() { return omgBlocks_; }
			StatementBlock *GetOmgwtfBlock() { return omgwtfBlock_; }
			
			void AddOmgBlock( Literal *, StatementBlock * );
			void SetOmgwtfBlock( StatementBlock * );
			
		private:
			OmgBlockList omgBlocks_;
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
			
			LiteralIdentifier *GetLabel() { return label_; }
			StatementBlock *GetBody() { return body_; }
			Identifier *GetLoopVariable() { return loopVar_; }
			
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
			UnaryBooleanExpression *GetLoopGuard() { return loopGuard_; }
			
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
			
			void AddNoesBlock( Expression *, StatementBlock * );
			void SetWelBlock( StatementBlock * );
			
		private:
			StatementBlock *tryBlock_;
			NoesBlockList noesBlocks_;
			StatementBlock *welBlock_;
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
			
		private:
			Identifier *targetVar_;
			bool isLong_;
	};

	class Program : public ASTNode {
		public:
			Program( StatementBlock * );
			~Program() {
				delete globals_;
				delete body_;
			}
			
			void Print( std::ostream & );
			unsigned int GetWidth() { return 2; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			StatementBlock *GetGlobals() { return globals_; }
			StatementBlock *GetBody() { return body_; }
		
		private:
			StatementBlock *globals_;
			StatementBlock *body_;
	};
}

#endif
