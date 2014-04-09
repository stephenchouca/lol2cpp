#ifndef GLOBALSHOISTER_H
#define GLOBALSHOISTER_H

#include "programvisitor.h"

class GlobalsHoister : public AST::ASTProgramOrderVisitor {
	public:
		void ProcessEnd( AST::ProgramBody * );
		void ProcessBegin( AST::FunkshunBlock * );
};

#endif