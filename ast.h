#ifndef AST_H
#define AST_H

#include <string>
#include <iostream>

namespace AST {
	class ASTVisitor;
	
	class ASTNode {
		public:
			ASTNode() : parent_( nullptr ) {}
			virtual ~ASTNode() {}
			
			virtual void Print( std::ostream & ) = 0;
			inline std::string DebugIndent( unsigned int tabs = 0 ) {
				return std::string( ( GetLevel() + tabs ) * 3, ' ' );
			}
			friend std::ostream & operator<<( std::ostream &out, ASTNode &node ) {
				node.Print( out );
				return out;
			}
			
			virtual void Accept( ASTVisitor * ) = 0;
			
			inline ASTNode *GetParent() { return parent_; }
			
			inline void SetParent( ASTNode *parent ) { parent_ = parent; }

		protected:
			virtual unsigned int GetWidth() { return 1; }
			inline unsigned int GetLevel() {
				return ( GetParent() == nullptr ? 0 : 
						 GetParent()->GetLevel() + GetParent()->GetWidth() );
			}

		private:
			ASTNode *parent_;
	};
}

#endif
