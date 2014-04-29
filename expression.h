#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <list>

#include "ast.h"
#include "visitor.h"

namespace AST {
	typedef bool troof_t;
	typedef long long int numbr_t;
	typedef double numbar_t;
	typedef std::string yarn_t;
	
	class Expression;
	class Literal;
	
	typedef std::list<Expression *> ExpressionList;
	typedef std::list<Literal *> LiteralList;
	
	typedef std::list<Expression *>::iterator ExpressionListIterator;
	typedef std::list<Literal *>::iterator LiteralListIterator;
	
	enum class OperatorType {
		INVALID,
		NOT,
		UPPIN,
		NERFIN,
		TIL,
		WILE,
		SUM,
		DIFF,
		PRODUKT,
		QUOSHUNT,
		MOD,
		BIGGR,
		SMALLR,
		BOTH,
		EITHER,
		WON,
		SAEM,
		DIFFRINT,
		ALL,
		ANY,
		SMOOSH
	};
	
	class Expression : public ASTNode {
		public:
			virtual Expression *Clone() = 0;
	};
	
	class Literal : public Expression {};

	class TroofLiteral : public Literal {
		public:
			TroofLiteral( troof_t val ) : val_( val ) {}
			
			void Print( std::ostream &out ) {
				out << DebugIndent() << "TROOF: " << ( val_ ? "WIN" : "FAIL" );
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			TroofLiteral *Clone() { return new TroofLiteral( val_ ); }
			
			troof_t GetValue() { return val_; }
			
		private:
			troof_t val_;
	};

	class NumbrLiteral : public Literal {
		public:
			NumbrLiteral( std::string & );
			
			void Print( std::ostream &out ) {
				out << DebugIndent() << "NUMBR: " << val_;
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NumbrLiteral *Clone() { return new NumbrLiteral( val_ ); }
			
			numbr_t GetValue() { return val_; }
		
		private:
			NumbrLiteral( numbr_t val ) : val_( val ) {}
			
		private:
			numbr_t val_;
	};

	class NumbarLiteral : public Literal {
		public:
			NumbarLiteral( std::string & );
			
			void Print( std::ostream &out ) {
				out << DebugIndent() << "NUMBAR: " << val_;
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NumbarLiteral *Clone() { return new NumbarLiteral( val_ ); }
			
			numbar_t GetValue() { return val_; }
		
		private:
			NumbarLiteral( numbar_t val ) : val_( val ) {}
		
		private:
			numbar_t val_;
	};

	class YarnLiteral : public Literal {
		public:
			YarnLiteral( yarn_t &val ) : val_( val ) {}
			
			void Print( std::ostream &out ) {
				out << DebugIndent() << "YARN: \"" << val_ << "\"";
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			YarnLiteral *Clone() { return new YarnLiteral( val_ ); }
			
			yarn_t GetValue() { return val_; }
			
		private:
			yarn_t val_;
	};
	
	class NoobLiteral : public Literal {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "NOOB";
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NoobLiteral *Clone() { return new NoobLiteral(); }
	};

	class TypeIdentifier : public ASTNode {
		public:
			enum class Type {
				NUMBR, 
				NUMBAR, 
				TROOF, 
				YARN, 
				BUKKIT, 
				NOOB 
			};
		
		public:
			TypeIdentifier( Type type ) : type_( type ) {}
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			TypeIdentifier *Clone() { return new TypeIdentifier( type_ ); }
			
			Type GetType() { return type_; }
			
		private:
			Type type_;
	};

	class Identifier : public Expression {};

	class LiteralIdentifier : public Identifier {
		public:
			LiteralIdentifier( std::string id ) : id_( id ) {}
			
			void Print( std::ostream &out ) {
				out << DebugIndent() << "IDENTIFIER: " << GetIdentifier();
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			LiteralIdentifier *Clone() { return new LiteralIdentifier( id_ ); }
			
			std::string GetIdentifier() { return id_; }
			
		private:
			std::string id_;
	};

	class SrsIdentifier : public Identifier {
		public:
			SrsIdentifier( Expression * );
			~SrsIdentifier() {
				delete ref_;
			}
			
			void Print( std::ostream & );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SrsIdentifier *Clone() { return new SrsIdentifier( ref_->Clone() ); }
			
		private:
			Expression *ref_;
	};

	class ItIdentifier : public Identifier {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "IT";
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ItIdentifier *Clone() { return new ItIdentifier(); }
	};

	class SlotIdentifier : public Identifier {
		public:
			SlotIdentifier( Expression * );
			~SlotIdentifier() {
				delete key_;
				delete bukkitRef_;
			}
			
			void Print( std::ostream & );
			inline unsigned int GetWidth() { return 2; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SlotIdentifier *Clone();
			
			Expression *GetKey() { return key_; }
			Expression *GetBukkitRef() { return bukkitRef_; }
			bool GetSafety() { return isSafe_; }
			
			void SetBukkitRef( Expression * );
			void SetSafety( bool isSafe ) { isSafe_ = isSafe; }
			
		private:
			Expression *key_;
			Expression *bukkitRef_;
			bool isSafe_;
	};

	class UnaryExpression : public Expression {
		public:
			UnaryExpression() : operand_( nullptr ) {}
			~UnaryExpression() {
				delete operand_;
			}
			
			void Print( std::ostream & );
			virtual std::string GetOperatorName() = 0;
			
			void CloneOperand( UnaryExpression * );
			
			virtual OperatorType GetOperator() { return OperatorType::INVALID; }
			Expression *GetOperand() { return operand_; }
			
			void SetOperand( Expression * );
			
		protected:
			Expression *operand_;
	};

	class NotExpression : public UnaryExpression {
		public:
			std::string GetOperatorName() { return "NOT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NotExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::NOT; }
	};
	
	class UppinExpression : public UnaryExpression {
		public:
			std::string GetOperatorName() { return "UPPIN"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			UppinExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::UPPIN; }
	};
	
	class NerfinExpression : public UnaryExpression {
		public:
			std::string GetOperatorName() { return "NERFIN"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NerfinExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::NERFIN; }
	};
	
	class TilExpression : public UnaryExpression {
		public:
			std::string GetOperatorName() { return "TIL"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			TilExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::TIL; }
	};
	
	class WileExpression : public UnaryExpression {
		public:
			std::string GetOperatorName() { return "WILE"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			WileExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::WILE; }
	};

	class BinaryExpression : public Expression {
		public:
			BinaryExpression() : leftOperand_( nullptr ), rightOperand_( nullptr ) {}
			~BinaryExpression() {
				delete leftOperand_;
				delete rightOperand_;
			}
			
			void Print( std::ostream & );
			virtual std::string GetOperatorName() = 0;
			
			void CloneOperands( BinaryExpression * );
			
			virtual OperatorType GetOperator() { return OperatorType::INVALID; }
			Expression *GetLeftOperand() { return leftOperand_; }
			Expression *GetRightOperand() { return rightOperand_; }
			
			void SetLeftOperand( Expression * );
			void SetRightOperand( Expression * );
			
		protected:
			Expression *leftOperand_;
			Expression *rightOperand_;
	};

	class SumExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "SUM"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SumExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::SUM; }
	};

	class DiffExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "DIFF"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			DiffExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::DIFF; }
	};

	class ProduktExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "PRODUKT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ProduktExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::PRODUKT; }
	};

	class QuoshuntExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "QUOSHUNT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			QuoshuntExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::QUOSHUNT; }
	};

	class ModExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "MOD"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ModExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::MOD; }
	};

	class BiggrExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "BIGGR"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			BiggrExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::BIGGR; }
	};

	class SmallrExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "SMALLR"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SmallrExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::SMALLR; }
	};

	class BothExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "BOTH"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			BothExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::BOTH; }
	};

	class EitherExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "EITHER"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			EitherExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::EITHER; }
	};

	class WonExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "WON"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			WonExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::WON; }
	};

	class SaemExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "SAEM"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SaemExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::SAEM; }
	};

	class DiffrintExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "DIFFRINT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			DiffrintExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::DIFFRINT; }
	};

	class NaryExpression : public Expression {
		public:
			~NaryExpression();
			
			void Print( std::ostream & );
			virtual std::string GetOperatorName() = 0;
			
			void CloneOperands( NaryExpression * );
			
			virtual OperatorType GetOperator() { return OperatorType::INVALID; }
			ExpressionList &GetOperands() { return operands_; }
			
			void AddOperand( Expression * );
			
		protected:
			ExpressionList operands_;
	};

	class AllExpression : public NaryExpression {
		public:
			std::string GetOperatorName() { return "ALL"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			AllExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::ALL; }
	};

	class AnyExpression : public NaryExpression {
		public:
			std::string GetOperatorName() { return "ANY"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			AnyExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::ANY; }
	};
	
	class SmooshExpression : public NaryExpression {
		public:
			std::string GetOperatorName() { return "SMOOSH"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SmooshExpression *Clone();
			
			OperatorType GetOperator() { return OperatorType::SMOOSH; }
	};

	class FunkshunCall : public NaryExpression {
		public:
			FunkshunCall( LiteralIdentifier * );
			~FunkshunCall() {
				delete funkshunName_;
			}
			
			std::string GetOperatorName() { 
				return "CALL " + funkshunName_->GetIdentifier();
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			LiteralIdentifier *GetFunkshunName() { return funkshunName_; }
			
			FunkshunCall *Clone();
				
		private:
			LiteralIdentifier *funkshunName_;
	};
	
	class CastExpression : public Expression {
		public:
			CastExpression( Expression * );
			~CastExpression() {
				delete srcExpr_;
				delete targetType_;
			}
			
			void Print( std::ostream &out );
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			CastExpression *Clone();
			
			Expression *GetSourceExpr() { return srcExpr_; }
			TypeIdentifier *GetCastTargetType() { return targetType_; }
			
			void SetCastTargetType( TypeIdentifier * );
			
		private:
			Expression *srcExpr_;
			TypeIdentifier *targetType_;
	};
}

#endif
