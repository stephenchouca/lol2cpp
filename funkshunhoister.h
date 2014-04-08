#ifndef FUNKSHUNHOISTER_H
#define FUNKSHUNHOISTER_H

#include "programvisitor.h"

class FunkshunHoister : public AST::ASTProgramOrderVisitor {
	public:
		void ProcessEnd( AST::StatementBlock * );
		void ProcessBegin( AST::FunkshunBlock * );
};

#endif