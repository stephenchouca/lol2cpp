#include <cassert>
#include <string>
#include <list>
#include <stack>
#include <sstream>

#include "codegen.h"

#if 0
const std::string CodeGenerator::SOURCE_PREFIX = "Source_";
const std::string CodeGenerator::SOURCE_DEFINED_PREFIX = "Defined_";
const std::string CodeGenerator::EXTRACT_PREFIX = "Extract";
const std::string CodeGenerator::EXTRACT_TROOF = EXTRACT_PREFIX + TROOF_TYPE;
const std::string CodeGenerator::EXTRACT_NUMBR = EXTRACT_PREFIX + NUMBR_TYPE;
const std::string CodeGenerator::EXTRACT_NUMBAR = EXTRACT_PREFIX + NUMBAR_TYPE;
const std::string CodeGenerator::EXTRACT_YARN = EXTRACT_PREFIX + YARN_TYPE;
const std::string CodeGenerator::EXTRACT_BUKKIT = EXTRACT_PREFIX + BUKKIT_TYPE;
const std::string CodeGenerator::EXTRACT_NOOB = EXTRACT_PREFIX + NOOB_TYPE;
const std::string CodeGenerator::CAST_TO = "CastTo";
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

void CodeGenerator::ProcessStatementBlockEnd( AST::StatementBlock *node ) {
	std::ostringstream code;
	const std::list<std::string> &stmts = codeSegments_.top();
	
	code << "{" << std::endl 
		 << VARIABLE_STORAGE << " " << IT_VARIABLE << ";" << std::endl;
	for( std::list<std::string>::const_iterator it = stmts.cbegin(); 
		 it != stmts.cend(); ++it ) {
		code << *it << std::endl;
	}
	code << "}";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::ORlyBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << "if(" << IT_VARIABLE << "." << EXTRACT_TROOF << "())" << std::endl << *it;
	++it;
	
	for( unsigned int i = 0; i < node->GetMebbeBlocks().size(); ++i ) {
		code << "else if(" << *it << "." << EXTRACT_TROOF << "())";
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
	
	// TODO: Implement.
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::ForLoopBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	std::string loopVar;
	
	code << "for(";
	if( node->GetLoopVariable() != nullptr ) {
		if( !node->GetLoopVariableIsLocal() && 
			node->GetLoopVariableInitExpr() == nullptr ) {
			loopVar = *it;
			++it;
		} else {
			if( node->GetLoopVariableIsLocal() ) {
				code << VARIABLE_STORAGE << " ";
			}
			
			loopVar = *it;
			code << loopVar;
			++it;
			
			if( node->GetLoopVariableInitExpr() != nullptr ) {
				code << " = " << *it;
			}
		}
	}
	code << "; ";
	if( node->GetLoopGuard() != nullptr ) {
		code << *it << "." << EXTRACT_TROOF << "()";
		++it;
	}
	code << "; ";
	if( node->GetLoopVariableIncExpr() != nullptr ) {
		code << loopVar << " = " << *it;
		++it;
	}
	code << ")" << std::endl << *it;
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::RangeLoopBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	// TODO: Implement.
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::FunkshunBlock *node ) {
	std::ostringstream funkshunDecl, funkshunDef;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	funkshunDecl << "bool " << SOURCE_DEFINED_PREFIX << *it 
				 << " = false;" << std::endl;
	
	funkshunDecl << VARIABLE_STORAGE << " " << *it;
	funkshunDef << VARIABLE_STORAGE << " " << *it;
	++it;

	funkshunDecl << "(";
	funkshunDef << "(";
	for( AST::FunkshunParamListIterator paramIt = node->GetParameters().begin();
		 paramIt != node->GetParameters().end(); ++paramIt ) {
		if( paramIt != node->GetParameters().cbegin() ) {
			funkshunDecl << ", ";
			funkshunDef << ", ";
		}
		funkshunDecl << VARIABLE_STORAGE << ( paramIt->second ? "&" : "" );
		funkshunDef << VARIABLE_STORAGE << ( paramIt->second ? "&" : "" ) 
					<< " " << *it;
		++it;
	}
	funkshunDecl << ");";
	funkshunDef << ")";
	
	funkshunDef << std::endl << *it;
	
	codeSegments_.pop();
	codeSegments_.top().push_back( funkshunDecl.str() );
	codeSegments_.top().push_back( funkshunDef.str() );
}

void CodeGenerator::ProcessEnd( AST::FunkshunDeclare *node ) {
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	std::string code = SOURCE_DEFINED_PREFIX + *it + " = true;";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code );
}

void CodeGenerator::ProcessEnd( AST::LiteralVarDeclare *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << VARIABLE_STORAGE << " " << *it;
	++it;
	
	if( node->GetInitValue() != nullptr ) {
		code << " = " << *it;
	} else if( node->GetInitType() != nullptr ) {
		code << "(" << VARIABLE_TYPE_PREFIX << *it << ")";
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
	std::ostringstream code, funkshuns;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	// Dump globals.
	for( unsigned int i = 0; i < node->GetFunkshuns().size(); ++i ) {
		code << *it << std::endl;
		++it;
		
		funkshuns << *it << std::endl;
		++it;
	}
	code << std::endl << funkshuns.str() << std::endl;
	
	// Dump program body.
	code << "int main()" << std::endl;
	code << *it << std::endl;
	
	codeSegments_.pop();
	emittedCode_ = code.str();
}

void CodeGenerator::Process( AST::TroofLiteral *node ) {
	std::ostringstream code;
	code << CREATE_TROOF_LITERAL << "(" 
		 << ( node->GetValue() ? "true" : "false" ) << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NumbrLiteral *node ) {
	std::ostringstream code;
	code << CREATE_NUMBR_LITERAL << "(" << node->GetValue() << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NumbarLiteral *node ) {
	std::ostringstream code;
	code << CREATE_NUMBAR_LITERAL << "(" << node->GetValue() << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::YarnLiteral *node ) {
	std::ostringstream code;
	code << CREATE_YARN_LITERAL << "(\"" << node->GetValue() << "\")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NoobLiteral *node ) {
	codeSegments_.top().push_back( CREATE_NOOB_LITERAL );
}

void CodeGenerator::Process( AST::TypeIdentifier *node ) {
	std::string code = "";
	switch( node->GetType() ) {
		case AST::TypeIdentifier::Type::NUMBR:
			code = NUMBR_TYPE;
			break;
		case AST::TypeIdentifier::Type::NUMBAR:
			code = NUMBAR_TYPE;
			break;
		case AST::TypeIdentifier::Type::TROOF:
			code = TROOF_TYPE;
			break;
		case AST::TypeIdentifier::Type::YARN:
			code = YARN_TYPE;
			break;
		case AST::TypeIdentifier::Type::BUKKIT:
			code = BUKKIT_TYPE;
			break;
		case AST::TypeIdentifier::Type::NOOB:
			code = NOOB_TYPE;
			break;
		default:
			break;
	}
	codeSegments_.top().push_back( code );
}

void CodeGenerator::Process( AST::LiteralIdentifier *node ) {
	codeSegments_.top().push_back( SOURCE_PREFIX + node->GetIdentifier() );
}

void CodeGenerator::Process( AST::ItIdentifier *node ) {
	codeSegments_.top().push_back( IT_VARIABLE );
}

void CodeGenerator::ProcessEnd( AST::SlotIdentifier *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << *it << "." 
		 << ( node->GetSafety() ? EXTRACT_FROM_BUKKIT : EXTRACT_FROM_BUKKIT_UNSAFE );
	++it;
	code << "(" << *it << ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::CastExpression *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << *it << "." << CAST_TO;
	++it;
	code << "(" << VARIABLE_TYPE_PREFIX << *it << ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
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