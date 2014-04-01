#include <iostream>

#include "ast.h"

namespace AST {
	std::ostream & operator<<( std::ostream &out, ASTNode &node ) {
		node.Print( out );
		return out;
	}
}