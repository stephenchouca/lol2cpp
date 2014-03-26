#ifndef AST_H
#define AST_H

namespace AST {
	class ASTVisitor;

	class ASTNode {
		public:
			ASTNode() : parent_( nullptr ) {}
			virtual ~ASTNode() {}

			inline void SetParent( ASTNode *parent ) { parent_ = parent; }
			
			//virtual void Accept( ASTVisitor & ) = 0;
			
			//friend std::ostream &operator <<( std::ostream &, ASTNode & ) {}

		protected:
			inline unsigned int GetLevel() {
				return ( parent_ == nullptr ? 0 : parent_->GetLevel() + 1 );
			}

		private:
			ASTNode *parent_;
	};
}

#endif
