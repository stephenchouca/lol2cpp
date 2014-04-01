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

			inline void SetParent( ASTNode *parent ) { parent_ = parent; }
			
			//virtual void Accept( ASTVisitor & ) = 0;
			
			virtual void Print( std::ostream & ) = 0;
			inline std::string DebugIndent( unsigned int tabs = 0 ) {
				return std::string( ( GetLevel() + tabs ) * 4, ' ' );
			}
			friend std::ostream &operator <<( std::ostream &, ASTNode & );

		protected:
			virtual unsigned int GetWidth() { return 1; }
			inline unsigned int GetLevel() {
				return ( parent_ == nullptr ? 0 : parent_->GetLevel() + GetWidth() );
			}

		private:
			ASTNode *parent_;
	};
}

#endif
