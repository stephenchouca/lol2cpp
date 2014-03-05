#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ast.h"
#include "statement.h"

namespace AST {
	class Literal : public ASTNode {
	};

	class TroofLiteral : public Literal {
	};

	class NumbrLiteral : public Literal {
	};

	class NumbarLiteral : public Literal {
	};

	class YarnLiteral : public Literal {
	};

	class TypeIdentifier : public ASTNode {
	};

	class Identifier : public ASTNode {
	};

	class LiteralIdentifier : public Identifier {
	};

	class SrsIdentifier : public Identifier {
	};

	class ItIdentifier : public Identifier {
	};

	class SlotIdentifier : public Identifier {
	};

	class UnaryExpression : public Expression {
	};

	class NotExpression : public UnaryExpression {
	};

	class BinaryExpression : public Expression {
	};

	class SumExpression : public BinaryExpression {
	};

	class DiffExpression : public BinaryExpression {
	};

	class ProduktExpression : public BinaryExpression {
	};

	class QuoshuntExpression : public BinaryExpression {
	};

	class ModExpression : public BinaryExpression {
	};

	class BiggrExpression : public BinaryExpression {
	};

	class SmallrExpression : public BinaryExpression {
	};

	class ModExpression : public BinaryExpression {
	};

	class BiggrExpression : public BinaryExpression {
	};

	class SmallrExpression : public BinaryExpression {
	};

	class BothExpression : public BinaryExpression {
	};

	class EitherExpression : public BinaryExpression {
	};

	class WonExpression : public BinaryExpression {
	};

	class SaemExpression : public BinaryExpression {
	};

	class DiffrintExpression : public BinaryExpression {
	};

	class NaryExpression : public Expression {
	};

	class AllExpression : public NaryExpression {
	};

	class AnyExpression : public NaryExpression {
	};

	class CastExpression : public Expression {
	};

	class SmooshExpression : public Expression {
	};

	class FunkshunCall : public Expression {
	};
}

#endif
