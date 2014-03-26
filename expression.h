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
			
			TroofLiteral *Clone() { return new TroofLiteral( val_ ); }
			
		private:
			troof_t val_;
	};

	class NumbrLiteral : public Literal {
		public:
			NumbrLiteral( std::string & );
			
			NumbrLiteral *Clone() { return new NumbrLiteral( val_ ); }
		
		private:
			NumbrLiteral( numbr_t val ) : val_( val ) {}
			
		private:
			numbr_t val_;
	};

	class NumbarLiteral : public Literal {
		public:
			NumbarLiteral( std::string & );
			
			NumbarLiteral *Clone() { return new NumbarLiteral( val_ ); }
		
		private:
			NumbarLiteral( numbar_t val ) : val_( val ) {}
		
		private:
			numbar_t val_;
	};

	class YarnLiteral : public Literal {
		public:
			YarnLiteral( yarn_t &val ) : val_( val ) {}
			
			YarnLiteral *Clone() { return new YarnLiteral( val_ ); }
			
		private:
			yarn_t val_;
	};
	
	class NoobLiteral : public Literal {
		public:
			NoobLiteral *Clone() { return new NoobLiteral(); }
	};

	class TypeIdentifier : public ASTNode {
		public:
			enum class Type { NUMBR, NUMBAR, TROOF, YARN, BUKKIT, NOOB };
			
			TypeIdentifier( Type type ) : type_( type ) {}
			
			TypeIdentifier *Clone() { return new TypeIdentifier( type_ ); }
			
		private:
			Type type_;
	};

	class Identifier : public Expression {
	};

	class LiteralIdentifier : public Identifier {
		public:
			LiteralIdentifier( std::string id ) : id_( id ) {}
			
			LiteralIdentifier *Clone() { return new LiteralIdentifier( id_ ); }
			
		private:
			std::string id_;
	};

	class SrsIdentifier : public Identifier {
		public:
			SrsIdentifier( Expression * );
			
			SrsIdentifier *Clone() { return new SrsIdentifier( ref_ ); }
			
		private:
			Expression *ref_;
	};

	class ItIdentifier : public Identifier {
		public:
			ItIdentifier *Clone() { return new ItIdentifier(); }
	};

	class SlotIdentifier : public Identifier {
		public:
			SlotIdentifier( Expression * );
			~SlotIdentifier() {
				delete key_;
				delete bukkitRef_;
			}
			
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
			NotExpression *Clone();
	};
	
	class UppinExpression : public UnaryMathExpression {
		public:
			UppinExpression *Clone();
	};
	
	class NerfinExpression : public UnaryMathExpression {
		public:
			NerfinExpression *Clone();
	};
	
	class TilExpression : public UnaryBooleanExpression {
		public:
			TilExpression *Clone();
	};
	
	class WileExpression : public UnaryBooleanExpression {
		public:
			WileExpression *Clone();
	};

	class BinaryExpression : public Expression {
		public:
			BinaryExpression() : leftOperand_( nullptr ), rightOperand_( nullptr ) {}
			~BinaryExpression() {
				delete leftOperand_;
				delete rightOperand_;
			}
			
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
			SumExpression *Clone();
	};

	class DiffExpression : public BinaryMathExpression {
		public:
			DiffExpression *Clone();
	};

	class ProduktExpression : public BinaryMathExpression {
		public:
			ProduktExpression *Clone();
	};

	class QuoshuntExpression : public BinaryMathExpression {
		public:
			QuoshuntExpression *Clone();
	};

	class ModExpression : public BinaryMathExpression {
		public:
			ModExpression *Clone();
	};

	class BiggrExpression : public BinaryMathExpression {
		public:
			BiggrExpression *Clone();
	};

	class SmallrExpression : public BinaryMathExpression {
		public:
			SmallrExpression *Clone();
	};

	class BothExpression : public BinaryBoolExpression {
		public:
			BothExpression *Clone();
	};

	class EitherExpression : public BinaryBoolExpression {
		public:
			EitherExpression *Clone();
	};

	class WonExpression : public BinaryBoolExpression {
		public:
			WonExpression *Clone();
	};

	class SaemExpression : public BinaryExpression {
		public:
			SaemExpression *Clone();
	};

	class DiffrintExpression : public BinaryExpression {
		public:
			DiffrintExpression *Clone();
	};

	class NaryExpression : public Expression {
		public:
			~NaryExpression();
			
			void CloneOperands( NaryExpression * );
			
			void AddOperand( Expression * );
			
		protected:
			ExpressionList operands_;
	};
	
	class NaryBooleanExpression : public NaryExpression {
	};

	class AllExpression : public NaryBooleanExpression {
		public:
			AllExpression *Clone();
	};

	class AnyExpression : public NaryBooleanExpression {
		public:
			AnyExpression *Clone();
	};
	
	class SmooshExpression : public NaryExpression {
		public:
			SmooshExpression *Clone();
	};

	class FunkshunCall : public NaryExpression {
		public:
			FunkshunCall( LiteralIdentifier * );
			~FunkshunCall() {
				delete funkshunName_;
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
			
			CastExpression *Clone();
			
			void SetCastTargetType( TypeIdentifier * );
			
		private:
			Expression *srcExpr_;
			TypeIdentifier *targetType_;
	};
}

#endif
