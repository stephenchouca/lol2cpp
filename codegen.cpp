#include <cassert>
#include <string>
#include <list>
#include <stack>
#include <sstream>
#include <unordered_map>

#include "codegen.h"

const std::string CodeGenerator::TROOF_TYPE = "TROOF";
const std::string CodeGenerator::NUMBR_TYPE = "NUMBR";
const std::string CodeGenerator::NUMBAR_TYPE = "NUMBAR";
const std::string CodeGenerator::YARN_TYPE = "YARN";
const std::string CodeGenerator::BUKKIT_TYPE = "BUKKIT";
const std::string CodeGenerator::NOOB_TYPE = "NOOB";
const std::string CodeGenerator::SOURCE_PREFIX = "Source_";
const std::string CodeGenerator::SOURCE_DEFINED_PREFIX = "Defined_";
const std::string CodeGenerator::EXTRACT_PREFIX = "Extract";
const std::string CodeGenerator::EXTRACT_TROOF = EXTRACT_PREFIX + TROOF_TYPE;
const std::string CodeGenerator::EXTRACT_NUMBR = EXTRACT_PREFIX + NUMBR_TYPE;
const std::string CodeGenerator::EXTRACT_NUMBAR = EXTRACT_PREFIX + NUMBAR_TYPE;
const std::string CodeGenerator::EXTRACT_YARN = EXTRACT_PREFIX + YARN_TYPE;
const std::string CodeGenerator::EXTRACT_BUKKIT = EXTRACT_PREFIX + BUKKIT_TYPE;
const std::string CodeGenerator::EXTRACT_NOOB = EXTRACT_PREFIX + NOOB_TYPE;
const std::string CodeGenerator::EXTRACT_FROM_BUKKIT = EXTRACT_PREFIX + "FromBukkit";
const std::string CodeGenerator::EXTRACT_FROM_BUKKIT_UNSAFE = 
	EXTRACT_FROM_BUKKIT + "Unsafe";
const std::string CodeGenerator::EXTRACT_NUMERIC_FROM_YARN = "ExtractNumericFromYarn";
const std::string CodeGenerator::CAST_TO = "CastTo";
const std::string CodeGenerator::VARIABLE_STORAGE = "Variable";
const std::string CodeGenerator::VARIABLE_TYPE = VARIABLE_STORAGE + "::Type";
const std::string CodeGenerator::VARIABLE_TYPE_PREFIX = VARIABLE_TYPE + "::";
const std::string CodeGenerator::VARIABLE_TYPEID = "type";
const std::string CodeGenerator::TROOF_VARIABLE = "troof";
const std::string CodeGenerator::NUMBR_VARIABLE = "numbr";
const std::string CodeGenerator::NUMBAR_VARIABLE = "numbar";
const std::string CodeGenerator::YARN_VARIABLE = "yarn";
const std::string CodeGenerator::BUKKIT_VARIABLE = "bukkit";
const std::string CodeGenerator::CREATE_LITERAL_PREFIX = 
	VARIABLE_STORAGE + "::Create";
const std::string CodeGenerator::CREATE_TROOF_LITERAL = 
	CREATE_LITERAL_PREFIX + TROOF_TYPE;
const std::string CodeGenerator::CREATE_NUMBR_LITERAL = 
	CREATE_LITERAL_PREFIX + NUMBR_TYPE;
const std::string CodeGenerator::CREATE_NUMBAR_LITERAL = 
	CREATE_LITERAL_PREFIX + NUMBAR_TYPE;
const std::string CodeGenerator::CREATE_YARN_LITERAL = 
	CREATE_LITERAL_PREFIX + YARN_TYPE;
const std::string CodeGenerator::CREATE_NOOB_LITERAL = 
	CREATE_LITERAL_PREFIX + NOOB_TYPE + "()";
const std::string CodeGenerator::IT_VARIABLE = "ItVariable";
const std::string CodeGenerator::LOLCODE_EXCEPTION = "std::exception()";

void CodeGenerator::ProcessEnd( AST::StatementBlock *node ) {
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
				++it;
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
	std::string funkshunName;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	funkshunName = *it;
	++it;
	
	funkshunDecl << "bool " << SOURCE_DEFINED_PREFIX << funkshunName
				 << " = false;" << std::endl;
	
	funkshunDecl << VARIABLE_STORAGE << " " << funkshunName;
	funkshunDef << VARIABLE_STORAGE << " " << funkshunName;

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
	
	funkshunDef << std::endl << "{" << std::endl << "if(!" 
				<< SOURCE_DEFINED_PREFIX << funkshunName 
				<< ") throw " << LOLCODE_EXCEPTION << ";" << std::endl;
	funkshunDef << *it;
	funkshunDef << std::endl << "}";
	
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
	
	EmitBoilerplate( code );
	
	// Dump program functions.
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
	
	//requiredOperators_[node->GetOperatorName()] = node->GetOperator();
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
	
	requiredOperators_[node->GetOperatorName()] = node->GetOperator();
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
	
	//requiredOperators_[node->GetOperatorName()] = node->GetOperator();
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

void CodeGenerator::EmitBoilerplate( std::ostringstream &code ) {
	for( std::unordered_map<std::string, AST::OperatorType>::const_iterator it = 
		 requiredOperators_.cbegin(); 
		 it != requiredOperators_.cend(); ++it ) {
		switch( it->second ) {
			case AST::OperatorType::NOT:
			case AST::OperatorType::UPPIN:
			case AST::OperatorType::NERFIN:
			case AST::OperatorType::TIL:
			case AST::OperatorType::WILE:
				EmitUnaryOperator( it->first, it->second, code );
				break;
			case AST::OperatorType::SUM:
			case AST::OperatorType::DIFF:
			case AST::OperatorType::PRODUKT:
			case AST::OperatorType::QUOSHUNT:
			case AST::OperatorType::MOD:
			case AST::OperatorType::BIGGR:
			case AST::OperatorType::SMALLR:
			case AST::OperatorType::BOTH:
			case AST::OperatorType::EITHER:
			case AST::OperatorType::WON:
			case AST::OperatorType::SAEM:
			case AST::OperatorType::DIFFRINT:
				EmitBinaryOperator( it->first, it->second, code );
				break;
			case AST::OperatorType::ALL:
			case AST::OperatorType::ANY:
			case AST::OperatorType::SMOOSH:
				//EmitNaryOperator( it->first, it->second, code );
				break;
			default:
				assert( false );
		}
	}
}

void CodeGenerator::EmitUnaryOperator( const std::string &operatorName, 
									   AST::OperatorType operatorType,
									   std::ostringstream &code ) {
	const std::string operand = "operand";
	const std::string ret = "ret";
	
	code << "inline " << VARIABLE_STORAGE << " " << operatorName << "("
		 << "const " << VARIABLE_STORAGE << " &" << operand << ") {" << std::endl;
	code << VARIABLE_STORAGE << " " << ret << ";" << std::endl;
	
	std::string op = "";
	switch( operatorType ) {
		case AST::OperatorType::UPPIN:
			op = "++";
			break;
		case AST::OperatorType::NERFIN:
			op = "--";
			break;
		case AST::OperatorType::NOT:
		case AST::OperatorType::TIL:
			op = "!";
			break;
		case AST::OperatorType::WILE:
		default:
			break;
	}
	
	switch( operatorType ) {
		case AST::OperatorType::UPPIN:
		case AST::OperatorType::NERFIN:
			code << "switch(" << operand << "." << VARIABLE_TYPEID 
				 << ") {" << std::endl;
				code << "case " << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
					 << ":" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = " << op << operand
						 << "." << NUMBR_VARIABLE << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "case " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = " << op << operand
						 << "." << NUMBAR_VARIABLE << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
			code << "}" << std::endl;
			code << "throw " << LOLCODE_EXCEPTION << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
			break;
		case AST::OperatorType::NOT:
		case AST::OperatorType::TIL:
		case AST::OperatorType::WILE:
			code << ret << "." << VARIABLE_TYPEID << " = " 
				 << VARIABLE_TYPE_PREFIX << TROOF_TYPE << ";" << std::endl;
			code << ret << "." << TROOF_VARIABLE << " = " << op << operand 
				 << "." << EXTRACT_TROOF << "();" << std::endl;
			code << "return " << ret << ";" << std::endl;
			break;
		default:
			assert( false );
			break;
	}
	
	code << "}" << std::endl;
}

void CodeGenerator::EmitBinaryOperator( const std::string &operatorName, 
										AST::OperatorType operatorType,
										std::ostringstream &code ) {
	const std::string leftOperand = "leftOperand";
	const std::string rightOperand = "rightOperand";
	const std::string ret = "ret";
	
	code << "inline " << VARIABLE_STORAGE << " " << operatorName << "("
		 << "const " << VARIABLE_STORAGE << " &" << leftOperand << ", "
		 << "const " << VARIABLE_STORAGE << " &" << rightOperand 
		 << ") {" << std::endl;
	code << VARIABLE_STORAGE << " " << ret << ";" << std::endl;
	
	std::string infixOp = "";
	std::string nonInfixOp = "";
	switch( operatorType ) {
		case AST::OperatorType::SUM:
			infixOp = " + ";
			break;
		case AST::OperatorType::DIFF:
			infixOp = " - ";
			break;
		case AST::OperatorType::PRODUKT:
			infixOp = " * ";
			break;
		case AST::OperatorType::QUOSHUNT:
			infixOp = " / ";
			break;
		case AST::OperatorType::MOD:
			infixOp = " % ";
			nonInfixOp = "std::fmod";
			break;
		case AST::OperatorType::BIGGR:
			nonInfixOp = "std::max";
			break;
		case AST::OperatorType::SMALLR:
			nonInfixOp = "std::min";
			break;
		case AST::OperatorType::BOTH:
			infixOp = " && ";
			break;
		case AST::OperatorType::EITHER:
			infixOp = " || ";
			break;
		case AST::OperatorType::WON:
			infixOp = " ^ ";
			break;
		case AST::OperatorType::SAEM:
			infixOp = " == ";
			break;
		case AST::OperatorType::DIFFRINT:
			infixOp = " != ";
			break;
	}
	
	switch( operatorType ) {
		case AST::OperatorType::SUM:
		case AST::OperatorType::DIFF:
		case AST::OperatorType::PRODUKT:
		case AST::OperatorType::QUOSHUNT:
		case AST::OperatorType::MOD:
		case AST::OperatorType::BIGGR:
		case AST::OperatorType::SMALLR:
		{
			const std::string leftNumbr = "leftNumbr";
			const std::string rightNumbr = "rightNumbr";
			const std::string leftNumbar = "leftNumbar";
			const std::string rightNumbar = "rightNumbar";
			const std::string leftType = "leftType";
			const std::string rightType = "rightType";
			
			code << "if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "("  
							 << leftOperand << "." << NUMBR_VARIABLE << ", "
							 << rightOperand << "." << NUMBR_VARIABLE << ")";
					} else {
						code << leftOperand << "." << NUMBR_VARIABLE << infixOp
							 << rightOperand << "." << NUMBR_VARIABLE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
							 << leftOperand << "." << NUMBR_VARIABLE << ", "
							 << rightOperand << "." << NUMBAR_VARIABLE << ")";
					} else {
						code << leftOperand << "." << NUMBR_VARIABLE << infixOp
							 << rightOperand << "." << NUMBAR_VARIABLE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
					 << ") {" << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << EXTRACT_NUMERIC_FROM_YARN << "(" << rightOperand << "."
						 << YARN_VARIABLE << ", " << rightNumbr << ", " 
						 << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VARIABLE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "("  
								 << leftOperand << "." << NUMBR_VARIABLE 
								 << ", " << rightNumbr << ")";
						} else {
							code << leftOperand << "." << NUMBR_VARIABLE 
								 << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
								 << leftOperand << "." << NUMBR_VARIABLE << ", "
								 << rightNumbar << ")";
						} else {
							code << leftOperand << "." << NUMBR_VARIABLE 
								 << infixOp << rightNumbar;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "("  
							 << leftOperand << "." << NUMBR_VARIABLE << ", "
							 << rightOperand << "." << TROOF_VARIABLE << " ? 1 : 0)";
					} else {
						code << leftOperand << "." << NUMBR_VARIABLE << infixOp << "("
							 << rightOperand << "." << TROOF_VARIABLE << " ? 1 : 0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "("  
							 << leftOperand << "." << NUMBR_VARIABLE << ", 0)";
					} else {
						code << leftOperand << "." << NUMBR_VARIABLE 
							 << infixOp << "0";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "("  
							 << leftOperand << "." << NUMBAR_VARIABLE 
							 << ", (" << NUMBAR_TYPE << ")" << rightOperand 
							 << "." << NUMBR_VARIABLE << ")";
					} else {
						code << leftOperand << "." << NUMBAR_VARIABLE << infixOp
							 << rightOperand << "." << NUMBR_VARIABLE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "("
							 << leftOperand << "." << NUMBAR_VARIABLE << ", "
							 << rightOperand << "." << NUMBAR_VARIABLE << ")";
					} else {
						code << leftOperand << "." << NUMBAR_VARIABLE << infixOp
							 << rightOperand << "." << NUMBAR_VARIABLE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE << ") {" 
					 << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << EXTRACT_NUMERIC_FROM_YARN << "(" << rightOperand << "."
						 << YARN_VARIABLE << ", " << rightNumbr << ", " 
						 << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << NUMBAR_VARIABLE << ", (" << NUMBAR_TYPE 
								 << ")" << rightNumbr << ")";
						} else {
							code << leftOperand << "." << NUMBAR_VARIABLE 
								 << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << NUMBAR_VARIABLE << ", " << rightNumbar << ")";
						} else {
							code << leftOperand << "." << NUMBAR_VARIABLE 
								 << infixOp << rightNumbar;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "("  
							 << leftOperand << "." << NUMBAR_VARIABLE << ", "
							 << rightOperand << "." << TROOF_VARIABLE 
							 << " ? 1.0 : 0.0)";
					} else {
						code << leftOperand << "." << NUMBAR_VARIABLE << infixOp 
							 << "(" << rightOperand << "." << TROOF_VARIABLE 
							 << " ? 1.0 : 0.0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" 
					 << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "("  
							 << leftOperand << "." << NUMBAR_VARIABLE << ", 0.0)";
					} else {
						code << leftOperand << "." << NUMBAR_VARIABLE 
							 << infixOp << "0.0";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << YARN_TYPE << ") {" << std::endl;
				code << NUMBR_TYPE << " " << leftNumbr << ";" << std::endl;
				code << NUMBAR_TYPE << " " << leftNumbar << ";" << std::endl;
				code << VARIABLE_TYPE << " " << leftType << " = "
					 << EXTRACT_NUMERIC_FROM_YARN << "(" << leftOperand << "."
					 << YARN_VARIABLE << ", " << leftNumbr << ", " << leftNumbar 
					 << ");" << std::endl;
				code << "if(" << leftType << " == " << VARIABLE_TYPE_PREFIX 
					 << NUMBR_TYPE << ") {" << std::endl;
					code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VARIABLE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftNumbr << ", "
								 << rightOperand << "." << NUMBR_VARIABLE << ")";
						} else {
							code << leftNumbr << infixOp << rightOperand 
								 << "." << NUMBR_VARIABLE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
								 << leftNumbr << ", " << rightOperand << "." 
								 << NUMBAR_VARIABLE << ")";
						} else {
							code << leftNumbr << infixOp << rightOperand 
								 << "." << NUMBAR_VARIABLE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
						 << ") {" << std::endl;
						code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
						code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
						code << VARIABLE_TYPE << " " << rightType << " = "
							 << EXTRACT_NUMERIC_FROM_YARN << "(" << rightOperand 
							 << "." << YARN_VARIABLE << ", " << rightNumbr << ", " 
							 << rightNumbar << ");" << std::endl;
						code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
							 << NUMBR_TYPE << ") {" << std::endl;
							code << ret << "." << VARIABLE_TYPEID << " = " 
								 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
								 << ";" << std::endl;
							code << ret << "." << NUMBR_VARIABLE << " = ";
							if( nonInfixOp != "" && infixOp == "" ) {
								code << nonInfixOp << "(" << leftNumbr 
									 << ", " << rightNumbr << ")";
							} else {
								code << leftNumbr << infixOp << rightNumbr;
							}
							code << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
						code << "} else if(" << rightType << " == " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ") {" << std::endl;
							code << ret << "." << VARIABLE_TYPEID << " = " 
								 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
								 << ";" << std::endl;
							code << ret << "." << NUMBAR_VARIABLE << " = ";
							if( nonInfixOp != "" ) {
								code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
									 << leftNumbr << ", " << rightNumbar << ")";
							} else {
								code << leftNumbr << infixOp << rightNumbar;
							}
							code << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
						code << "}" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VARIABLE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftNumbr << ", " 
								 << rightOperand << "." << TROOF_VARIABLE 
								 << " ? 1 : 0)";
						} else {
							code << leftNumbr << infixOp << "(" << rightOperand 
								 << "." << TROOF_VARIABLE << " ? 1 : 0)";
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VARIABLE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftNumbr << ", 0)";
						} else {
							code << leftNumbr << infixOp << "0";
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
				code << "} else if(" << leftType << " == " << VARIABLE_TYPE_PREFIX 
					 << NUMBAR_TYPE << ") {" << std::endl;
					code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftNumbar
								 << ", (" << NUMBAR_TYPE << ")" << rightOperand 
								 << "." << NUMBR_VARIABLE << ")";
						} else {
							code << leftNumbar << infixOp << rightOperand 
								 << "." << NUMBR_VARIABLE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftNumbar << ", "
								 << rightOperand << "." << NUMBAR_VARIABLE << ")";
						} else {
							code << leftNumbar << infixOp << rightOperand 
								 << "." << NUMBAR_VARIABLE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE << ") {" 
						 << std::endl;
						code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
						code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
						code << VARIABLE_TYPE << " " << rightType << " = "
							 << EXTRACT_NUMERIC_FROM_YARN << "(" << rightOperand 
							 << "." << YARN_VARIABLE << ", " << rightNumbr << ", " 
							 << rightNumbar << ");" << std::endl;
						code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
							 << NUMBR_TYPE << ") {" << std::endl;
							code << ret << "." << VARIABLE_TYPEID << " = " 
								 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
								 << ";" << std::endl;
							code << ret << "." << NUMBAR_VARIABLE << " = ";
							if( nonInfixOp != "" ) {
								code << nonInfixOp << "(" << leftNumbar << ", (" 
									 << NUMBAR_TYPE << ")" << rightNumbr << ")";
							} else {
								code << leftNumbar << infixOp << rightNumbr;
							}
							code << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
						code << "} else if(" << rightType << " == " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ") {" << std::endl;
							code << ret << "." << VARIABLE_TYPEID << " = " 
								 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
								 << ";" << std::endl;
							code << ret << "." << NUMBAR_VARIABLE << " = ";
							if( nonInfixOp != "" ) {
								code << nonInfixOp << leftNumbar << ", " 
									 << rightNumbar << ")";
							} else {
								code << leftNumbar << infixOp << rightNumbar;
							}
							code << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
						code << "}" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftNumbar << ", "
								 << rightOperand << "." << TROOF_VARIABLE 
								 << " ? 1.0 : 0.0)";
						} else {
							code << leftNumbar << infixOp << "(" << rightOperand 
								 << "." << TROOF_VARIABLE << " ? 1.0 : 0.0)";
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" 
						 << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand 
								 << "." << NUMBAR_VARIABLE << ", 0.0)";
						} else {
							code << leftNumbar << infixOp << "0.0";
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;					 
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << TROOF_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VARIABLE << " ? 1 : 0, " 
							 << rightOperand << "." << NUMBR_VARIABLE << ")";
					} else {
						code << "(" << leftOperand << "." << TROOF_VARIABLE 
							 << " ? 1 : 0)" << infixOp << rightOperand 
							 << "." << NUMBR_VARIABLE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VARIABLE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VARIABLE << " ? 1.0 : 0.0, "
							 << rightOperand << "." << NUMBAR_VARIABLE << ")";
					} else {
						code << "(" << leftOperand << "." << TROOF_VARIABLE 
							 << " ? 1.0 : 0.0)" << infixOp << rightOperand 
							 << "." << NUMBAR_VARIABLE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
					 << ") {" << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << EXTRACT_NUMERIC_FROM_YARN << "(" << rightOperand << "."
						 << YARN_VARIABLE << ", " << rightNumbr << ", " 
						 << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VARIABLE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << TROOF_VARIABLE << " ? 1 : 0, " 
								 << rightNumbr << ")";
						} else {
							code << "(" << leftOperand << "." << TROOF_VARIABLE 
								 << " ? 1 : 0)" << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VARIABLE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << TROOF_VARIABLE << " ? 1.0 : 0.0, " 
								 << rightNumbar << ")";
						} else {
							code << "(" << leftOperand << "." << TROOF_VARIABLE 
								 << " ? 1.0 : 0.0)" << infixOp << rightNumbar;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VARIABLE << " ? 1 : 0, " << rightOperand 
							 << "." << TROOF_VARIABLE << " ? 1 : 0)";
					} else {
						code << "(" << leftOperand << "." << TROOF_VARIABLE 
							 << " ? 1 : 0)" << infixOp << "(" << rightOperand
							 << "." << TROOF_VARIABLE << " ? 1 : 0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VARIABLE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VARIABLE << " ? 1 : 0, 0)";
					} else {
						code << "(" << leftOperand << "." << TROOF_VARIABLE 
							 << " ? 1 : 0)" << infixOp << "0";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" << std::endl;
				 
			code << "}" << std::endl;
			code << "throw " << LOLCODE_EXCEPTION << ";" << std::endl;
			break;
		}
		case AST::OperatorType::BOTH:
		case AST::OperatorType::EITHER:
		case AST::OperatorType::WON:
			break;
		case AST::OperatorType::SAEM:
		case AST::OperatorType::DIFFRINT:
			break;
	}
	
	code << "}" << std::endl;
}