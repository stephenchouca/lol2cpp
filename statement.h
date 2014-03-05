#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"

namespace AST {
	class Statement : public ASTNode {
	};

	class Expression : public Statement {
	};

	class ORlyBlock : public Statement {
	};

	class WtfBlock : public Statement {
	};

	class LoopBlock : public Statement {
	};

	class FunkshunBlock : public Statement {
	};

	class PlzBlock : public Statement {
	};

	class VarDeclare : public Statement {
	};

	class VarAssign : public Statement {
	};

	class VarCast : public Statement {
	};

	class FunkshunReturn : public Statement {
	};

	class GTFOStatement : public Statement {
	};

	class VisibleStatement : public Statement {
	};

	class GimmehStatement : public Statement {
	};

	class StatementBlock : public ASTNode {
	};

	class Program : public ASTNode {
		public:
			inline void Accept( ASTVisitor &v ) { v.Visit( this ); }
	};
}

#endif
