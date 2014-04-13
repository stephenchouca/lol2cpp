#include <cassert>
#include <string>
#include <list>
#include <stack>
#include <sstream>

#include "codegen.h"

#if 0
const std::string CodeGenerator::SOURCE_PREFIX = "Source_";
const std::string CodeGenerator::SOURCE_DEFINED_PREFIX = "Defined_";
const std::string CodeGenerator::CAST_TO_PREFIX = "CastTo";
const std::string CodeGenerator::CAST_TO_TROOF = CAST_TO_PREFIX + TROOF_TYPE;
const std::string CodeGenerator::CAST_TO_NUMBR = CAST_TO_PREFIX + NUMBR_TYPE;
const std::string CodeGenerator::CAST_TO_NUMBAR = CAST_TO_PREFIX + NUMBAR_TYPE;
const std::string CodeGenerator::CAST_TO_YARN = CAST_TO_PREFIX + YARN_TYPE;
const std::string CodeGenerator::CAST_TO_BUKKIT = CAST_TO_PREFIX + BUKKIT_TYPE;
const std::string CodeGenerator::CAST_TO_NOOB = CAST_TO_PREFIX + NOOB_TYPE;
const std::string CodeGenerator::VARIABLE_STORAGE = "Variable";
const std::string CodeGenerator::VARIABLE_TYPE_PREFIX = "Variable::Type::";
const std::string CodeGenerator::TROOF_TYPE = "TROOF";
const std::string CodeGenerator::NUMBR_TYPE = "NUMBR";
const std::string CodeGenerator::NUMBAR_TYPE = "NUMBAR";
const std::string CodeGenerator::YARN_TYPE = "YARN";
const std::string CodeGenerator::BUKKIT_TYPE = "BUKKIT";
const std::string CodeGenerator::NOOB_TYPE = "NOOB";
const std::string CodeGenerator::IT_VARIABLE = "ItVariable";
#endif

void CodeGenerator::ProcessStatementBlockEnd( 
		AST::StatementBlock *node, bool emitBraces ) {
	std::ostringstream code;
	const std::list<std::string> &stmts = codeSegments_.top();
	
	if( emitBraces ) {
		code << "{" << std::endl;
	}
	for( std::list<std::string>::const_iterator it = stmts.cbegin(); 
		 it != stmts.cend(); ++it ) {
		code << *it << std::endl;
	}
	if( emitBraces ) {
		code << "}" << std::endl;
	}
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::ORlyBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << "if(" << CAST_TO_TROOF << "(" << IT_VARIABLE << "))" << std::endl << *it;
	++it;
	
	for( unsigned int i = 0; i < node->GetMebbeBlocks().size(); ++i ) {
		code << "else if(" << CAST_TO_TROOF << "(" << *it << "))";
		++it;
		code << *it;
		++it;
	}
	
	if( node->GetNoWaiBody() != nullptr ) {
		code << "else" << *it;
	}
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::WtfBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	

	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::ForLoopBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << "for(";
	if( node->GetLoopVariable() != nullptr ) {
		if( !node->GetLoopVariableIsLocal() && 
			node->GetLoopVariableInitExpr() == nullptr ) {
			++it;
		} else {
			if( node->GetLoopVariableIsLocal() ) {
				code << VARIABLE_STORAGE << " ";
			}
			
			code << *it;
			++it;
			
			if( node->GetLoopVariableInitExpr() != nullptr ) {
				code << " = " << *it;
			}
		}
	}
	code << "; ";
	if( node->GetLoopGuard() != nullptr ) {
		code << CAST_TO_TROOF << "(" << *it << ")";
		++it;
	}
	code << "; ";
	if( node->GetLoopVariableIncExpr() != nullptr ) {
		code << *it;
		++it;
	}
	code << ")" << std::endl << *it;
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::RangeLoopBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	

	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::FunkshunBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << "bool " << SOURCE_DEFINED_PREFIX << *it << " = false;" << std::endl;
	
	code << VARIABLE_STORAGE << " " << *it << "(";
	++it;
	
	for( AST::FunkshunParamListIterator paramIt = node->GetParameters().begin();
		 paramIt != node->GetParameters().end(); ++paramIt ) {
		if( paramIt != node->GetParameters().cbegin() ) {
			code << ", ";
		}
		code << VARIABLE_STORAGE << ( paramIt->second ? "&" : "" ) << " " << *it;
		++it;
	}
	
	code << std::endl << *it;
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::FunkshunDeclare *node ) {
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	std::string code = SOURCE_DEFINED_PREFIX + *it + " = true;";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code );
}

void CodeGenerator::ProcessEnd( AST::VarDeclare *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << VARIABLE_STORAGE << " " << *it;
	++it;
	
	if( node->GetInitValue() != nullptr ) {
		code << " = " << *it;
	} else if( node->GetInitType() != nullptr ) {
		code << "(" << *it << ")";
	}
	code << ";";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::VarAssign *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << *it << " = ";
	++it;
	
	code << *it << ";";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::FunkshunReturn *node ) {
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	std::string code = "return " + *it + ";";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code );
}

void CodeGenerator::Process( AST::GtfoStatement *node ) {
	codeSegments_.top().push_back( "break;" );
}

void CodeGenerator::ProcessEnd( AST::VisibleStatement *node ) {
	std::ostringstream code;
	const std::list<std::string> &exprs = codeSegments_.top();
	
	code << "std::cout";
	for( std::list<std::string>::const_iterator it = exprs.cbegin(); 
		 it != exprs.cend(); ++it ) {
		code << " << " << *it;
	}
	if( !node->GetSuppressNewLine() ) {
		code << " << std::endl";
	}
	code << ";";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::GimmehStatement *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	if( node->IsLong() ) {
		code << "std::getline( std::cin, " << *it << " );";
	} else {
		code << "std::cin >> " << *it << ";";
	}
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::Program *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	// Dump globals.
	code << *it << std::endl;
	++it;
	
	// Dump program body.
	code << "int main()" << std::endl;
	code << *it << std::endl;
	
	codeSegments_.pop();
	codeOutput_ = code.str();
}

void CodeGenerator::Process( AST::TroofLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE << "(";
	code << ( node->GetValue() ? "true" : "false" ) << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NumbrLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE << "((numbr_t)" << node->GetValue() << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NumbarLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE << "((numbar_t)" << node->GetValue() << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::YarnLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE << "(\"" << node->GetValue() << "\")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NoobLiteral *node ) {
	codeSegments_.top().push_back( VARIABLE_STORAGE + "()" );
}

void CodeGenerator::Process( AST::TypeIdentifier *node ) {
	std::ostringstream code;
	code << VARIABLE_TYPE_PREFIX;
	switch( node->GetType() ) {
		case AST::TypeIdentifier::Type::NUMBR:
			code << NUMBR_TYPE;
			break;
		case AST::TypeIdentifier::Type::NUMBAR:
			code << NUMBAR_TYPE;
			break;
		case AST::TypeIdentifier::Type::TROOF:
			code << TROOF_TYPE;
			break;
		case AST::TypeIdentifier::Type::YARN:
			code << YARN_TYPE;
			break;
		case AST::TypeIdentifier::Type::BUKKIT:
			code << BUKKIT_TYPE;
			break;
		case AST::TypeIdentifier::Type::NOOB:
			code << NOOB_TYPE;
			break;
		default:
			break;
	}
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::LiteralIdentifier *node ) {
	codeSegments_.top().push_back( SOURCE_PREFIX + node->GetIdentifier() );
}

void CodeGenerator::Process( AST::ItIdentifier *node ) {
	codeSegments_.top().push_back( IT_VARIABLE );
}

void CodeGenerator::ProcessUnaryExpressionEnd( AST::UnaryExpression *node ) {
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	std::string code = node->GetOperatorName() + "(" + *it + ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code );
}

void CodeGenerator::ProcessBinaryExpressionEnd( AST::BinaryExpression *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << node->GetOperatorName() << "(";
	code << *it << ", ";
	++it;
	code << *it << ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessNaryExpressionEnd( AST::NaryExpression *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << node->GetOperatorName() << "(";
	for( unsigned int i = 0; i < node->GetOperands().size(); ++i ) {
		if( i != 0 ) {
			code << ", ";
		}
		code << *it;
		++it;
	}
	code << ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::FunkshunCall *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << *it << "(";
	++it;
	for( unsigned int i = 0; i < node->GetOperands().size(); ++i ) {
		if( i != 0 ) {
			code << ", ";
		}
		code << *it;
		++it;
	}
	code << ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}