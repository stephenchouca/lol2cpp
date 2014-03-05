#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

namespace AST {
	class ASTVisitor {
		public:
			virtual void Visit( Program * ) = 0;
	};
}

#endif

