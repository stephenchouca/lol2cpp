#include "visitor.h"

namespace AST {
	void Accept( ASTNode *node ) {
		if( node == nullptr ) {
			return;
		}
		
		node->Accept( this );
	}
}