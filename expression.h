#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "ast.h"
#include "statement.h"

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
			
			YarnLiteral *Clone() { return new YarnLiteral( val_ ); }
			
		private:
			yarn_t val_;
	};
	
	class NoobLiteral : public Literal {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "NOOB";
			}
			
			NoobLiteral *Clone() { return new NoobLiteral(); }
	};

	class TypeIdentifier : public ASTNode {
		public:
			enum class Type { NUMBR, NUMBAR, TROOF, YARN, BUKKIT, NOOB };
			
			TypeIdentifier( Type type ) : type_( type ) {}
			
			void Print( std::ostream & );
			
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
			
			SrsIdentifier *Clone() { return new SrsIdentifier( ref_ ); }
			
		private:
			Expression *ref_;
	};

	class ItIdentifier : public Identifier {
		public:
			void Print( std::ostream &out ) {
				out << DebugIndent() << "IT";
			}
			
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
			
			SlotIdentifier *Clone();
			
			void SetBukkitReference( Expression * );
			
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
			
			NotExpression *Clone();
	};
	
	class UppinExpression : public UnaryMathExpression {
		public:
			std::string GetOperatorName() { return "UPPIN"; }
			
			UppinExpression *Clone();
	};
	
	class NerfinExpression : public UnaryMathExpression {
		public:
			std::string GetOperatorName() { return "NERFIN"; }
			
			NerfinExpression *Clone();
	};
	
	class TilExpression : public UnaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "TIL"; }
			
			TilExpression *Clone();
	};
	
	class WileExpression : public UnaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "WILE"; }
			
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
			
			void SetLeftOperand( Expression * );
			void SetRightOperand( Expression * );
			
		protected:
			Expression *leftOperand_;
			Expression *rightOperand_;
	};
	
	class BinaryMathExpression : public BinaryExpression {
	};
	
	class BinaryBoolExpression : public BinaryExpression {
	};
	
	class BinaryCompareExpression : public BinaryExpression {
	};

	class SumExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "SUM"; }
			
			SumExpression *Clone();
	};

	class DiffExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "DIFF"; }
			
			DiffExpression *Clone();
	};

	class ProduktExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "PRODUKT"; }
			
			ProduktExpression *Clone();
	};

	class QuoshuntExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "QUOSHUNT"; }
			
			QuoshuntExpression *Clone();
	};

	class ModExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "MOD"; }
			
			ModExpression *Clone();
	};

	class BiggrExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "BIGGR"; }
			
			BiggrExpression *Clone();
	};

	class SmallrExpression : public BinaryMathExpression {
		public:
			std::string GetOperatorName() { return "SMALLR"; }
			
			SmallrExpression *Clone();
	};

	class BothExpression : public BinaryBoolExpression {
		public:
			std::string GetOperatorName() { return "BOTH"; }
			
			BothExpression *Clone();
	};

	class EitherExpression : public BinaryBoolExpression {
		public:
			std::string GetOperatorName() { return "EITHER"; }
			
			EitherExpression *Clone();
	};

	class WonExpression : public BinaryBoolExpression {
		public:
			std::string GetOperatorName() { return "WON"; }
			
			WonExpression *Clone();
	};

	class SaemExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "SAEM"; }
			
			SaemExpression *Clone();
	};

	class DiffrintExpression : public BinaryExpression {
		public:
			std::string GetOperatorName() { return "DIFFRINT"; }
			
			DiffrintExpression *Clone();
	};

	class NaryExpression : public Expression {
		public:
			~NaryExpression();
			
			void Print( std::ostream & );
			virtual std::string GetOperatorName() = 0;
			
			void CloneOperands( NaryExpression * );
			
			void AddOperand( Expression * );
			
		protected:
			ExpressionList operands_;
	};
	
	class NaryBooleanExpression : public NaryExpression {
	};

	class AllExpression : public NaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "ALL"; }
			
			AllExpression *Clone();
	};

	class AnyExpression : public NaryBooleanExpression {
		public:
			std::string GetOperatorName() { return "ANY"; }
			
			AnyExpression *Clone();
	};
	
	class SmooshExpression : public NaryExpression {
		public:
			std::string GetOperatorName() { return "SMOOSH"; }
			
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
			
			CastExpression *Clone();
			
			void SetCastTargetType( TypeIdentifier * );
			
		private:
			Expression *srcExpr_;
			TypeIdentifier *targetType_;
	};
}

#endif
