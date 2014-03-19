#ifndef AST_H
#define AST_H

namespace AST {
	class ASTVisitor;

	class ASTNode {
		public:
			ASTNode() : parent_( nullptr ) {}

			//virtual void Accept( ASTVisitor & ) = 0;

			void SetParent( ASTNode * );
			inline ASTNode *GetParent() { return parent_; }
			inline unsigned int GetLevel() {
				return ( parent_ == nullptr ? 0 : parent_->GetLevel() );
			}

		private:
			ASTNode *parent_;
	};
}

#endif
