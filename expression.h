#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "ast.h"
#include "statement.h"
#include "visitor.h"

namespace AST {
	typedef bool troof_t;
	typedef long int numbr_t;
	typedef double numbar_t;
	typedef std::string yarn_t;
	
	class Literal : public Expression {
	};

	class TroofLiteral : public Literal {
		public:
			TroofLiteral( troof_t val ) : val_( val ) {}
			
			void Print( std::ostream &out ) {
				out << DebugIndent() << "TROOF: " << ( val_ ? "WIN" : "FAIL" );
			}
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			TroofLiteral *Clone() { return new TroofLiteral( val_ ); }
			
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
			
		private:
			Type type_;
	};

	class Identifier : public Expression {
	};

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
			
			void SetBukkitRef( Expression * );
			
		private:
			Expression *key_;
			Expression *bukkitRef_;
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
			
			Expression *GetOperand() { return operand_; }
			
			void SetOperand( Expression * );
			
		protected:
			Expression *operand_;
	};
	
	class UnaryBooleanExpression : public UnaryExpression {
	};
	
	class UnaryMathExpression : public UnaryExpression {
	};

	class NotExpression : public UnaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "NOT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NotExpression *Clone();
	};
	
	class UppinExpression : public UnaryMathExpression {
		public:
			std::string GetOperatorName() { return "UPPIN"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			UppinExpression *Clone();
	};
	
	class NerfinExpression : public UnaryMathExpression {
		public:
			std::string GetOperatorName() { return "NERFIN"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			NerfinExpression *Clone();
	};
	
	class TilExpression : public UnaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "TIL"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			TilExpression *Clone();
	};
	
	class WileExpression : public UnaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "WILE"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			WileExpression *Clone();
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
			
			Expression *GetLeftOperand() { return leftOperand_; }
			Expression *GetRightOperand() { return rightOperand_; }
			
			void SetLeftOperand( Expression * );
			void SetRightOperand( Expression * );
			
		protected:
			Expression *leftOperand_;
			Expression *rightOperand_;
	};
	
	class BinaryMathExpression : public BinaryExpression {
	};
	
	class BinaryBooleanExpression : public BinaryExpression {
	};
	
	class BinaryCompareExpression : public BinaryExpression {
	};

	class SumExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "SUM"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SumExpression *Clone();
	};

	class DiffExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "DIFF"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			DiffExpression *Clone();
	};

	class ProduktExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "PRODUKT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ProduktExpression *Clone();
	};

	class QuoshuntExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "QUOSHUNT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			QuoshuntExpression *Clone();
	};

	class ModExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "MOD"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			ModExpression *Clone();
	};

	class BiggrExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "BIGGR"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			BiggrExpression *Clone();
	};

	class SmallrExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "SMALLR"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SmallrExpression *Clone();
	};

	class BothExpression : public BinaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "BOTH"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			BothExpression *Clone();
	};

	class EitherExpression : public BinaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "EITHER"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			EitherExpression *Clone();
	};

	class WonExpression : public BinaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "WON"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			WonExpression *Clone();
	};

	class SaemExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "SAEM"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SaemExpression *Clone();
	};

	class DiffrintExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "DIFFRINT"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			DiffrintExpression *Clone();
	};

	class NaryExpression : public Expression {
		public:
			~NaryExpression();
			
			void Print( std::ostream & );
			virtual std::string GetOperatorName() = 0;
			
			void CloneOperands( NaryExpression * );
			
			ExpressionList &GetOperands() { return operands_; }
			
			void AddOperand( Expression * );
			
		protected:
			ExpressionList operands_;
	};
	
	class NaryBooleanExpression : public NaryExpression {
	};

	class AllExpression : public NaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "ALL"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			AllExpression *Clone();
	};

	class AnyExpression : public NaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "ANY"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			AnyExpression *Clone();
	};
	
	class SmooshExpression : public NaryExpression {
		public:
			std::string GetOperatorName() { return "SMOOSH"; }
			
			void Accept( ASTVisitor *visitor ) { visitor->Visit( this ); }
			
			SmooshExpression *Clone();
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