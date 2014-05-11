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
const std::string CodeGenerator::BUKKIT_ITERATOR_TYPE = "BUKKIT_ITERATOR";
const std::string CodeGenerator::BUKKIT_REF_TYPE = "BUKKIT_REF";
const std::string CodeGenerator::NOOB_TYPE = "NOOB";
const std::string CodeGenerator::TEMP_PREFIX = "Temp";
const std::string CodeGenerator::SOURCE_PREFIX = "Source_";
const std::string CodeGenerator::SOURCE_DEFINED_PREFIX = "Defined_";
const std::string CodeGenerator::MAEK_PREFIX = "Maek";
const std::string CodeGenerator::MAEK_TROOF = MAEK_PREFIX + TROOF_TYPE + "()";
const std::string CodeGenerator::MAEK_NUMBR = MAEK_PREFIX + NUMBR_TYPE + "()";
const std::string CodeGenerator::MAEK_NUMBAR = MAEK_PREFIX + NUMBAR_TYPE + "()";
const std::string CodeGenerator::MAEK_YARN = MAEK_PREFIX + YARN_TYPE + "()";
const std::string CodeGenerator::MAEK_BUKKIT = MAEK_PREFIX + BUKKIT_TYPE + "()";
const std::string CodeGenerator::MAEK_NOOB = MAEK_PREFIX + NOOB_TYPE + "()";
const std::string CodeGenerator::EXTRACT_PREFIX = "Extract";
const std::string CodeGenerator::EXTRACT_TROOF = EXTRACT_PREFIX + TROOF_TYPE + "()";
const std::string CodeGenerator::EXTRACT_YARN = EXTRACT_PREFIX + YARN_TYPE + "()";
const std::string CodeGenerator::EXTRACT_FROM_BUKKIT = EXTRACT_PREFIX + "FromBukkit";
const std::string CodeGenerator::EXTRACT_FROM_BUKKIT_UNSAFE = 
	EXTRACT_FROM_BUKKIT + "Unsafe";
const std::string CodeGenerator::EXTRACT_NUMERIC_FROM_YARN = "ExtractNumericFromYarn";
const std::string CodeGenerator::VARIABLE_STORAGE = "Variable";
const std::string CodeGenerator::VARIABLE_STORAGE_PREFIX = VARIABLE_STORAGE + "::";
const std::string CodeGenerator::VARIABLE_TYPE_ENUM = "Type";
const std::string CodeGenerator::VARIABLE_TYPE = 
	VARIABLE_STORAGE_PREFIX + VARIABLE_TYPE_ENUM;
const std::string CodeGenerator::VARIABLE_TYPE_PREFIX = VARIABLE_TYPE + "::";
const std::string CodeGenerator::VARIABLE_TYPEID = "type";
const std::string CodeGenerator::TROOF_VALUE = "troof";
const std::string CodeGenerator::NUMBR_VALUE = "numbr";
const std::string CodeGenerator::NUMBAR_VALUE = "numbar";
const std::string CodeGenerator::YARN_VALUE = "yarn";
const std::string CodeGenerator::BUKKIT_VALUE = "bukkit";
const std::string CodeGenerator::CREATE_LITERAL_PREFIX = "Create";
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
	
	code << "{" << std::endl;
	code << VARIABLE_STORAGE << " " << IT_VARIABLE << ";" << std::endl;
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
	
	code << "if(" << IT_VARIABLE << "." << EXTRACT_TROOF << ")" << *it;
	++it;
	
	for( unsigned int i = 0; i < node->GetMebbeBlocks().size(); ++i ) {
		code << "else if(" << *it << "." << EXTRACT_TROOF << ")";
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
	std::ostringstream code, numbrCases, yarnCases, numbarCases, troofCases, noobCase;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	const std::string switchVar = "switchVar";
	bool firstYarnCase = true;
	bool firstNumbarCase = true;
	bool firstTroofCase = true;
	
	unsigned int i = 1;
	for( AST::OmgBlockListIterator omgIt = node->GetOmgBlocks().begin(); 
		 omgIt != node->GetOmgBlocks().end(); ++omgIt, ++i ) {
		const AST::Literal *label = omgIt->first;
		switch( label->GetType() ) {
			case AST::Literal::Type::NUMBR:
			{
				const AST::NumbrLiteral *numbrLabel = 
					static_cast<const AST::NumbrLiteral *>( label );
				numbrCases << "case " << numbrLabel->GetValue() << ":" << std::endl;
				numbrCases << switchVar << " = " << i << ";" << std::endl;
				numbrCases << "break;" << std::endl;
				firstTroofCase = false;
				break;
			}
			case AST::Literal::Type::YARN:
			{
				const AST::YarnLiteral *yarnLabel = 
					static_cast<const AST::YarnLiteral *>( label );
				yarnCases << ( firstYarnCase ? "" : "else " ) << "if(" 
					   	  << IT_VARIABLE << "." << YARN_VALUE << " == " << "\"" 
						  << yarnLabel->GetValue() << "\") " << switchVar 
						  << " = " << i << ";" << std::endl;
				firstYarnCase = false;
				break;
			}
			case AST::Literal::Type::NUMBAR:
			{
				const AST::NumbarLiteral *numbarLabel = 
					static_cast<const AST::NumbarLiteral *>( label );
				numbarCases << ( firstNumbarCase ? "" : "else " ) << "if(" 
							<< IT_VARIABLE << "." << NUMBAR_VALUE << " == "  
							<< numbarLabel->GetValue() << ") " << switchVar 
							<< " = " << i << ";" << std::endl;
				firstNumbarCase = false;
				break;
			}
			case AST::Literal::Type::TROOF:
			{
				const AST::TroofLiteral *troofLabel = 
					static_cast<const AST::TroofLiteral *>( label );
				troofCases << ( firstTroofCase ? "" : "else " ) << "if(" 
						   << ( troofLabel->GetValue() ? "" : "!" ) << IT_VARIABLE
						   << "." << TROOF_VALUE << ") " << switchVar << " = "
						   << i << ";" << std::endl;
				break;
			}
			case AST::Literal::Type::NOOB:
				assert( noobCase.str() == "" );
				noobCase << switchVar << " = " << i << ";" << std::endl;
				break;
			default:
				assert( false );
				break;
		}
	}
	
	bool mixedTypes = false;
	const std::string numbrCasesStr = numbrCases.str();
	const std::string yarnCasesStr = yarnCases.str();
	const std::string numbarCasesStr = numbarCases.str();
	const std::string troofCasesStr = troofCases.str();
	const std::string noobCaseStr = noobCase.str();
	
	code << "{" << std::endl;
	code << "unsigned int " << switchVar << " = 0;" << std::endl;
	if( numbrCasesStr != "" ) {
		code << ( mixedTypes ? "else " : "" ) << "if(" << IT_VARIABLE << "." 
			 << VARIABLE_TYPEID << " == " << VARIABLE_TYPE_PREFIX << NUMBR_TYPE
			 << ") {" << std::endl;
		code << "switch(" << IT_VARIABLE << "." << NUMBR_VALUE << ") {" << std::endl;
		code << numbrCasesStr;
		code << "}" << std::endl;
		code << "}";
		mixedTypes = true;
	}
	if( yarnCasesStr != "" ) {
		code << ( mixedTypes ? "else " : "" ) << "if(" << IT_VARIABLE << "." 
			 << VARIABLE_TYPEID << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE
			 << ") {" << std::endl;
		code << yarnCasesStr;
		code << "}";
		mixedTypes = true;
	}
	if( numbarCasesStr != "" ) {
		code << ( mixedTypes ? "else " : "" ) << "if(" << IT_VARIABLE << "." 
			 << VARIABLE_TYPEID << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE
			 << ") {" << std::endl;
		code << numbarCasesStr;
		code << "}";
		mixedTypes = true;
	}
	if( troofCasesStr != "" ) {
		code << ( mixedTypes ? "else " : "" ) << "if(" << IT_VARIABLE << "." 
			 << VARIABLE_TYPEID << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE
			 << ") {" << std::endl;
		code << troofCasesStr;
		code << "}";
		mixedTypes = true;
	}
	if( noobCaseStr != "" ) {
		code << ( mixedTypes ? "else " : "" ) << "if(" << IT_VARIABLE << "." 
			 << VARIABLE_TYPEID << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE
			 << ") {" << std::endl;
		code << noobCaseStr;
		code << "}";
		mixedTypes = true;
	}
	code << std::endl;
	
	i = 1;
	
	code << "switch(" << switchVar << ") {" << std::endl;
	for( AST::OmgBlockListIterator omgIt = node->GetOmgBlocks().begin(); 
		 omgIt != node->GetOmgBlocks().end(); ++omgIt, ++i ) {
		++it;
		code << "case " << i << ":" << std::endl;
		if( omgIt->second->GetStatements().size() > 0 ) {
			code << *it << std::endl;
		}
		++it;
	}
	if( node->GetOmgwtfBody() != nullptr ) {
		code << "default:" << std::endl;
		code << *it << std::endl;
	}
	code << "}" << std::endl;
	
	code << "}" << std::endl;
	
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
			} else if( node->GetLoopVariableIsLocal() ) {
				// If loop variable is local and not initialized in source code, 
				// assume it is a NUMBR (and initialize it to 0).
				code << "(" << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ")";
			}
		}
	}
	code << "; ";
	if( node->GetLoopGuard() != nullptr ) {
		code << *it << "." << EXTRACT_TROOF;
		++it;
	}
	code << "; ";
	if( node->GetLoopVariableIncExpr() != nullptr ) {
		code << loopVar << " = " << *it;
		++it;
	}
	code << ")" << *it;
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessEnd( AST::RangeLoopBlock *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	const std::string loopVar = *(it++);
	const std::string bukkitRef = *(it++);
	const std::string tempBukkitRef = TEMP_PREFIX + "_BukkitRef";
	const std::string loopVarIterator = tempBukkitRef + "_Iterator";
	
	code << "{" << std::endl;
	code << VARIABLE_STORAGE << " " << tempBukkitRef << " = " << bukkitRef 
		 << "." << MAEK_BUKKIT << ";" << std::endl;
	code << BUKKIT_ITERATOR_TYPE << " " << loopVarIterator << " = " 
		 << tempBukkitRef << "." << BUKKIT_VALUE << "->begin();" << std::endl;
	code << "for(" << VARIABLE_STORAGE << " " << loopVar << "; " << loopVarIterator 
		 << " != " << tempBukkitRef << "." << BUKKIT_VALUE << "->end(); ++"
		 << loopVarIterator << ") {" << std::endl;
	code << loopVar << " = " << loopVarIterator << "->first;" << std::endl;
	code << *it;
	code << "}" << std::endl;
	code << "}" << std::endl;
	
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
	
	funkshunDef << "{" << std::endl;
	funkshunDef << "if(!" << SOURCE_DEFINED_PREFIX << funkshunName 
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

void CodeGenerator::ProcessEnd( AST::SlotVarDeclare *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << *it;
	++it;
	
	if( node->GetInitValue() != nullptr ) {
		code << " = " << *it;
	} else if( node->GetInitType() != nullptr ) {
		code << " = " << VARIABLE_STORAGE << "(" << VARIABLE_TYPE_PREFIX 
			 << *it << ")";
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

void CodeGenerator::Process( AST::WhatevrStatement *node ) {
	codeSegments_.top().push_back( "continue;" );
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
	code << "int main()" << *it << std::endl;
	
	codeSegments_.pop();
	emittedCode_ = code.str();
}

void CodeGenerator::Process( AST::TroofLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE_PREFIX << CREATE_TROOF_LITERAL << "(" 
		 << ( node->GetValue() ? "true" : "false" ) << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NumbrLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE_PREFIX << CREATE_NUMBR_LITERAL << "(" 
		 << node->GetValue() << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NumbarLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE_PREFIX << CREATE_NUMBAR_LITERAL << "(" 
		 << node->GetValue() << ")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::YarnLiteral *node ) {
	std::ostringstream code;
	code << VARIABLE_STORAGE_PREFIX << CREATE_YARN_LITERAL << "(\"" 
		 << node->GetValue() << "\")";
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::Process( AST::NoobLiteral *node ) {
	codeSegments_.top().push_back( VARIABLE_STORAGE_PREFIX + CREATE_NOOB_LITERAL );
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

void CodeGenerator::ProcessEnd( AST::MaekExpression *node ) {
	std::ostringstream code;
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	code << *it;
	++it;
	code << "." << MAEK_PREFIX << *it << "()";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code.str() );
}

void CodeGenerator::ProcessUnaryExpressionEnd( AST::UnaryExpression *node ) {
	std::list<std::string>::const_iterator it = codeSegments_.top().cbegin();
	
	std::string code = node->GetOperatorName() + "(" + *it + ")";
	
	codeSegments_.pop();
	codeSegments_.top().push_back( code );
	
	requiredOperators_[node->GetOperatorName()] = node->GetOperator();
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
	
	requiredOperators_[node->GetOperatorName()] = node->GetOperator();
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
	EmitTypedefs( code );
	
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
				EmitNaryOperator( it->first, it->second, code );
				break;
			default:
				assert( false );
		}
	}
}

void CodeGenerator::EmitTypedefs( std::ostringstream &code ) {
	const std::string variableTypePrefix = VARIABLE_TYPE_ENUM + "::";
	const std::string srcVar = "src";
	const std::string srcType = "src";
	const std::string val = "val";
	const std::string numbrRet = "numbr";
	const std::string numbarRet = "numbar";
	const std::string ret = "ret";
	const std::string out = "out";
	const std::string end = "end";
	
	code << "#include <iostream>" << std::endl;
	code << "#include <iomanip>" << std::endl;
	code << "#include <sstream>" << std::endl;
	code << "#include <cstdint>" << std::endl;
	code << "#include <cstdlib>" << std::endl;
	code << "#include <memory>" << std::endl;
	code << "#include <string>" << std::endl;
	code << "#include <unordered_map>" << std::endl;
	code << "#include <exception>" << std::endl;
	code << "#include <new>" << std::endl;
	
	code << "struct " << VARIABLE_STORAGE << ";" << std::endl;
	
	code << "typedef int " << NUMBR_TYPE << ";" << std::endl;
	code << "typedef float " << NUMBAR_TYPE << ";" << std::endl;
	code << "typedef bool " << TROOF_TYPE << ";" << std::endl;
	code << "typedef std::string " << YARN_TYPE << ";" << std::endl;
	code << "typedef std::unordered_map<" << VARIABLE_STORAGE << ","
		 << VARIABLE_STORAGE << "," << VARIABLE_STORAGE << "> " 
		 << BUKKIT_TYPE << ";" << std::endl;
	code << "typedef std::shared_ptr<" << BUKKIT_TYPE << "> " 
		 << BUKKIT_REF_TYPE << ";" << std::endl;
		 
	code << "struct " << VARIABLE_STORAGE << "{" << std::endl;
		code << "enum class " << VARIABLE_TYPE_ENUM << ": std::int8_t {" << std::endl;
			code << NUMBR_TYPE << " = 0, " << NUMBAR_TYPE << ", " << YARN_TYPE 
				 << ", " << TROOF_TYPE << ", " << NOOB_TYPE << ", " << BUKKIT_TYPE 
				 << std::endl;
		code << "};" << std::endl;
		
		code << VARIABLE_TYPE_ENUM << " " << VARIABLE_TYPEID << ";" << std::endl;
		code << "union {" << std::endl;
			code << NUMBR_TYPE << " " << NUMBR_VALUE << ";" << std::endl;
			code << NUMBAR_TYPE << " " << NUMBAR_VALUE << ";" << std::endl;
			code << TROOF_TYPE << " " << TROOF_VALUE << ";" << std::endl;
			code << YARN_TYPE << " " << YARN_VALUE << ";" << std::endl;
			code << BUKKIT_REF_TYPE << " " << BUKKIT_VALUE << ";" << std::endl;
		code << "};" << std::endl;
		
		code << VARIABLE_STORAGE << "() : " << VARIABLE_TYPEID << "("
			 << variableTypePrefix << NOOB_TYPE << ") {}" << std::endl;
		code << VARIABLE_STORAGE << "(" << VARIABLE_TYPE_ENUM << " " << srcType
			 << ") : " << VARIABLE_TYPEID << "(" << srcType << ") {" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << NUMBR_VALUE << " = 0;" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << NUMBAR_VALUE << " = 0.0;" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << TROOF_TYPE 
					 << ":" << std::endl;
					code << TROOF_VALUE << " = false;" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << "new (&" << YARN_VALUE << ") " << YARN_TYPE 
						 << "(\"\");" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << "new (&" << BUKKIT_VALUE << ") " << BUKKIT_REF_TYPE 
						 << "(new " << BUKKIT_TYPE << "());" << std::endl;
					code << "break;" << std::endl;
			code << "}" << std::endl;
		code << "}" << std::endl;
		code << VARIABLE_STORAGE << "(const " << VARIABLE_STORAGE << " &" << srcVar
			 << ") : " << VARIABLE_TYPEID << "(" << srcVar << "." << VARIABLE_TYPEID
			 << ") {" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << NUMBR_VALUE << " = " << srcVar << "." << NUMBR_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << NUMBAR_VALUE << " = " << srcVar << "." << NUMBAR_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << TROOF_TYPE 
					 << ":" << std::endl;
					code << TROOF_VALUE << " = " << srcVar << "." << TROOF_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << "new (&" << YARN_VALUE << ") " << YARN_TYPE << "("
						 << srcVar << "." << YARN_VALUE << ");" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << "new (&" << BUKKIT_VALUE << ") " << BUKKIT_REF_TYPE << "("
						 << srcVar << "." << BUKKIT_VALUE << ");" << std::endl;
					code << "break;" << std::endl;
			code << "}" << std::endl;
		code << "}" << std::endl;
		code << "~" << VARIABLE_STORAGE << "() {" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << BUKKIT_VALUE << ".reset();" << std::endl;
					code << "break;" << std::endl;
			code << "}" << std::endl;
		code << "}" << std::endl;
		
		code << "inline " << TROOF_TYPE << " " << EXTRACT_TROOF 
			 << " const {" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << TROOF_TYPE << ") return " << TROOF_VALUE << ";" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << "return " << NUMBR_VALUE << " != 0;" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << "return " << NUMBAR_VALUE << " != 0.0;" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << "return " << YARN_VALUE << " != \"\";" << std::endl;
				code << "case " << variableTypePrefix << NOOB_TYPE 
					 << ":" << std::endl;
					code << "return false;" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << "return " << BUKKIT_VALUE << "->size() > 0;" << std::endl;
			code << "}" << std::endl;
			code << "return false;" << std::endl;
		code << "}" << std::endl;
		code << "inline " << YARN_TYPE << " " << EXTRACT_YARN 
			 << " const {" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << YARN_TYPE << ") return " << YARN_VALUE << ";" << std::endl;
			code << "std::ostringstream " << out << ";" << std::endl;
			code << out << " << *this;" << std::endl;
			code << "return " << out << ".str();" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " &" << EXTRACT_FROM_BUKKIT 
			 << "(const " << VARIABLE_STORAGE << " &" << val << ") {" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " != " << variableTypePrefix
				 << BUKKIT_TYPE << ") throw " << LOLCODE_EXCEPTION 
				 << ";" << std::endl;
			code << "return (*" << BUKKIT_VALUE << ").at(" << val 
				 << ");" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " &" << EXTRACT_FROM_BUKKIT_UNSAFE 
			 << "(const " << VARIABLE_STORAGE << " &" << val << ") {" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " != " << variableTypePrefix
				 << BUKKIT_TYPE << ") throw " << LOLCODE_EXCEPTION 
				 << ";" << std::endl;
			code << "return (*" << BUKKIT_VALUE << ")[" << val << "];" << std::endl;
		code << "}" << std::endl;
		code << "static inline " << VARIABLE_TYPE_ENUM << " " 
			 << EXTRACT_NUMERIC_FROM_YARN << "(const " << YARN_TYPE << " &" << val
			 << ", " << NUMBR_TYPE << " &" << numbrRet << ", " << NUMBAR_TYPE 
			 << " &" << numbarRet << ") {" << std::endl;
			code << "char *" << end << ";" << std::endl;
			code << numbrRet << " = std::strtol(" << val << ".c_str(), &" << end 
				 << ", 10);" << std::endl;
			code << "if(*" << end << " == '\\0') return " << variableTypePrefix
				 << NUMBR_TYPE << ";" << std::endl;
			code << numbarRet << " = std::strtod(" << val << ".c_str(), &" << end 
				 << ");" << std::endl;
			code << "if(*" << end << " == '\\0') return " << variableTypePrefix
				 << NUMBAR_TYPE << ";" << std::endl;
			code << "return " << variableTypePrefix << NOOB_TYPE << ";" << std::endl;
		code << "}" << std::endl;
		
		code << "inline " << VARIABLE_STORAGE << " " << MAEK_NUMBR 
			 << " const {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix 
				 << NUMBR_TYPE << ");" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NUMBAR_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBR_VALUE << " = (" << NUMBR_TYPE << ")" 
					 << NUMBAR_VALUE << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NUMBR_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBR_VALUE << " = " << NUMBR_VALUE 
					 << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << YARN_TYPE << ") {" << std::endl;
				code << "char *" << end << ";" << std::endl;
				code << ret << "." << NUMBR_VALUE << " = std::strtol(" << YARN_VALUE
					 << ".c_str(), &" << end << ", 10);" << std::endl;
				code << "if(*" << end << " != '\\0') throw " << LOLCODE_EXCEPTION
					 << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << TROOF_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBR_VALUE << " = " << TROOF_VALUE 
					 << " ? 1 : 0;" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NOOB_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBR_VALUE << " = 0;" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "}" << std::endl;
			code << "throw " << LOLCODE_EXCEPTION << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " " << MAEK_NUMBAR 
			 << " const {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix 
				 << NUMBAR_TYPE << ");" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NUMBR_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBAR_VALUE << " = (" << NUMBAR_TYPE << ")" 
					 << NUMBR_VALUE << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NUMBAR_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBAR_VALUE << " = " << NUMBAR_VALUE 
					 << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << YARN_TYPE << ") {" << std::endl;
				code << "char *" << end << ";" << std::endl;
				code << ret << "." << NUMBAR_VALUE << " = std::strtod(" << YARN_VALUE
					 << ".c_str(), &" << end << ");" << std::endl;
				code << "if(*" << end << " != '\\0') throw " << LOLCODE_EXCEPTION
					 << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << TROOF_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBAR_VALUE << " = " << TROOF_VALUE 
					 << " ? 1.0 : 0.0;" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NOOB_TYPE << ") {" << std::endl;
				code << ret << "." << NUMBAR_VALUE << " = 0.0;" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "}" << std::endl;
			code << "throw " << LOLCODE_EXCEPTION << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " " << MAEK_YARN
			 << " const {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix 
				 << YARN_TYPE << ");" << std::endl;
			code << ret << "." << YARN_VALUE << " = " << EXTRACT_YARN 
				 << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " " << MAEK_TROOF
			 << " const {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix 
				 << TROOF_TYPE << ");" << std::endl;
			code << ret << "." << TROOF_VALUE << " = " << EXTRACT_TROOF 
				 << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " " << MAEK_NOOB
			 << " const {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "inline " << VARIABLE_STORAGE << " " << MAEK_BUKKIT
			 << " const {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << ";" << std::endl;
			code << ret << "." << VARIABLE_TYPEID << " = " << variableTypePrefix
				 << BUKKIT_TYPE << ";" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << NOOB_TYPE << ") {" << std::endl;
				code << "new (&" << ret << "." << BUKKIT_VALUE << ") " 
					 << BUKKIT_REF_TYPE << "(new " << BUKKIT_TYPE 
					 << "());" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << VARIABLE_TYPEID << " == " << variableTypePrefix
				 << BUKKIT_TYPE << ") {" << std::endl;
				code << "new (&" << ret << "." << BUKKIT_VALUE << ") " 
					 << BUKKIT_REF_TYPE << "(" << BUKKIT_VALUE << ");" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "}" << std::endl;
			code << "throw " << LOLCODE_EXCEPTION << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		
		code << "static inline " << VARIABLE_STORAGE << " " << CREATE_NUMBR_LITERAL 
			 << "(const " << NUMBR_TYPE << " &" << val << ") {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix
				 << NUMBR_TYPE << ");" << std::endl;
			code << ret << "." << NUMBR_VALUE << " = " << val << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "static inline " << VARIABLE_STORAGE << " " << CREATE_NUMBAR_LITERAL 
			 << "(const " << NUMBAR_TYPE << " &" << val << ") {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix
				 << NUMBAR_TYPE << ");" << std::endl;
			code << ret << "." << NUMBAR_VALUE << " = " << val << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "static inline " << VARIABLE_STORAGE << " " << CREATE_TROOF_LITERAL 
			 << "(const " << TROOF_TYPE << " &" << val << ") {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix
				 << TROOF_TYPE << ");" << std::endl;
			code << ret << "." << TROOF_VALUE << " = " << val << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;
		code << "static inline " << VARIABLE_STORAGE << " " << CREATE_YARN_LITERAL 
			 << "(const " << YARN_TYPE << " &" << val << ") {" << std::endl;
			code << VARIABLE_STORAGE << " " << ret << "(" << variableTypePrefix
				 << YARN_TYPE << ");" << std::endl;
			code << ret << "." << YARN_VALUE << " = " << val << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
		code << "}" << std::endl;		 
		
		code << VARIABLE_STORAGE << " &operator=(const " << VARIABLE_STORAGE << " &"
			 << srcVar << ") {" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << BUKKIT_VALUE << ".reset();" << std::endl;
					code << "break;" << std::endl;
			code << "}" << std::endl;
			code << VARIABLE_TYPEID << " = " << srcVar << "." << VARIABLE_TYPEID
				 << ";" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << NUMBR_VALUE << " = " << srcVar << "." << NUMBR_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << NUMBAR_VALUE << " = " << srcVar << "." << NUMBAR_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << TROOF_TYPE 
					 << ":" << std::endl;
					code << TROOF_VALUE << " = " << srcVar << "." << TROOF_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << "new (&" << YARN_VALUE << ") " << YARN_TYPE << "("
						 << srcVar << "." << YARN_VALUE << ");" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << "new (&" << BUKKIT_VALUE << ") " << BUKKIT_REF_TYPE << "("
						 << srcVar << "." << BUKKIT_VALUE << ");" << std::endl;
					code << "break;" << std::endl;
			code << "}" << std::endl;
			code << "return *this;" << std::endl;
		code << "}" << std::endl;
		code << "bool operator==(const " << VARIABLE_STORAGE << " &" << srcVar
			 << ") const {" << std::endl;
			code << "if(" << VARIABLE_TYPEID << " != " << srcVar << "." 
				 << VARIABLE_TYPEID << ") return false;" << std::endl;
			code << "switch(" << VARIABLE_TYPEID << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << "return " << NUMBR_VALUE << " == " << srcVar << "." 
						 << NUMBR_VALUE << ";" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << "return " << NUMBAR_VALUE << " == " << srcVar << "." 
						 << NUMBAR_VALUE << ";" << std::endl;
				code << "case " << variableTypePrefix << TROOF_TYPE 
					 << ":" << std::endl;
					code << "return " << TROOF_VALUE << " == " << srcVar << "." 
						 << TROOF_VALUE << ";" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << "return " << YARN_VALUE << " == " << srcVar << "." 
						 << YARN_VALUE << ";" << std::endl;
				code << "case " << variableTypePrefix << NOOB_TYPE 
					 << ":" << std::endl;
					code << "return true;" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << "return " << BUKKIT_VALUE << " == " << srcVar << "." 
						 << BUKKIT_VALUE << ";" << std::endl;
			code << "}" << std::endl;
		code << "}" << std::endl;
		code << "std::size_t operator()(const " << VARIABLE_STORAGE << " &" << srcVar
			 << ") const {" << std::endl;
			code << "switch(" << srcVar << "." << VARIABLE_TYPEID 
				 << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << "return std::hash<" << NUMBR_TYPE << ">()(" << srcVar
						 << "." << NUMBR_VALUE << ");" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << "return std::hash<" << NUMBAR_TYPE << ">()(" << srcVar
						 << "." << NUMBAR_VALUE << ");" << std::endl;
				code << "case " << variableTypePrefix << TROOF_TYPE 
					 << ":" << std::endl;
					code << "return std::hash<" << TROOF_TYPE << ">()(" << srcVar
						 << "." << TROOF_VALUE << ");" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << "return std::hash<" << YARN_TYPE << ">()(" << srcVar
						 << "." << YARN_VALUE << ");" << std::endl;
				code << "case " << variableTypePrefix << NOOB_TYPE 
					 << ":" << std::endl;
					code << "return std::hash<" << NUMBR_TYPE << ">()(0);" 
						 << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					code << "return std::hash<" << BUKKIT_REF_TYPE << ">()(" 
						 << srcVar << "." << BUKKIT_VALUE << ");" << std::endl;
			code << "}" << std::endl;
		code << "}" << std::endl;
		
		code << "friend std::ostream &operator<<(std::ostream &" << out << ", const "
			 << VARIABLE_STORAGE << " &" << srcVar << ") {" << std::endl;
			code << "switch(" << srcVar << "." << VARIABLE_TYPEID 
				 << ") {" << std::endl;
				code << "case " << variableTypePrefix << NUMBR_TYPE 
					 << ":" << std::endl;
					code << out << " << " << srcVar << "." << NUMBR_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << out << " << std::setprecision(2) << std::fixed << " 
						 << srcVar << "." << NUMBAR_VALUE << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << TROOF_TYPE 
					 << ":" << std::endl;
					code << out << " << (" << srcVar << "." << TROOF_VALUE 
						 << " ? \"WIN\" : \"FAIL\");" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << YARN_TYPE 
					 << ":" << std::endl;
					code << out << " << " << srcVar << "." << YARN_VALUE 
						 << ";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << NOOB_TYPE 
					 << ":" << std::endl;
					code << out << " << \"NOOB\";" << std::endl;
					code << "break;" << std::endl;
				code << "case " << variableTypePrefix << BUKKIT_TYPE 
					 << ":" << std::endl;
					// TODO: Implement code-gen for printing bukkits, probably 
					//		 reasonable to print out key-value pairs.
					code << "break;" << std::endl;
			code << "}" << std::endl;
			code << "return " << out << ";" << std::endl;
		code << "}" << std::endl;
	code << "};" << std::endl;
	
	code << "typedef std::unordered_map<" << VARIABLE_STORAGE << ","
	 << VARIABLE_STORAGE << "," << VARIABLE_STORAGE << ">::iterator " 
	 << BUKKIT_ITERATOR_TYPE << ";" << std::endl;
};

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
			op = " + ";
			break;
		case AST::OperatorType::NERFIN:
			op = " - ";
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
					code << ret << "." << NUMBR_VALUE << " = " << operand << "." 
						 << NUMBR_VALUE << op << "1;" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "case " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VALUE << " = " << operand << "." 
						 << NUMBAR_VALUE << op << "1.0;" << std::endl;
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
			code << ret << "." << TROOF_VALUE << " = " << op << operand 
				 << "." << EXTRACT_TROOF << ";" << std::endl;
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
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBR_VALUE << ", " << rightOperand << "." 
							 << NUMBR_VALUE << ")";
					} else {
						code << leftOperand << "." << NUMBR_VALUE << infixOp
							 << rightOperand << "." << NUMBR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
							 << leftOperand << "." << NUMBR_VALUE << ", "
							 << rightOperand << "." << NUMBAR_VALUE << ")";
					} else {
						code << leftOperand << "." << NUMBR_VALUE << infixOp
							 << rightOperand << "." << NUMBAR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
					 << ") {" << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN 
						 << "(" << rightOperand << "." << YARN_VALUE << ", " 
						 << rightNumbr << ", " << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VALUE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << NUMBR_VALUE << ", " << rightNumbr << ")";
						} else {
							code << leftOperand << "." << NUMBR_VALUE 
								 << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
								 << leftOperand << "." << NUMBR_VALUE << ", "
								 << rightNumbar << ")";
						} else {
							code << leftOperand << "." << NUMBR_VALUE 
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
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBR_VALUE << ", " << rightOperand << "." 
							 << TROOF_VALUE << " ? 1 : 0)";
					} else {
						code << leftOperand << "." << NUMBR_VALUE << infixOp << "("
							 << rightOperand << "." << TROOF_VALUE << " ? 1 : 0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBR_VALUE << ", 0)";
					} else {
						code << leftOperand << "." << NUMBR_VALUE 
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
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBAR_VALUE << ", (" << NUMBAR_TYPE << ")" 
							 << rightOperand << "." << NUMBR_VALUE << ")";
					} else {
						code << leftOperand << "." << NUMBAR_VALUE << infixOp
							 << rightOperand << "." << NUMBR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBAR_VALUE << ", " << rightOperand << "." 
							 << NUMBAR_VALUE << ")";
					} else {
						code << leftOperand << "." << NUMBAR_VALUE << infixOp
							 << rightOperand << "." << NUMBAR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE << ") {" 
					 << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN 
						 << "(" << rightOperand << "." << YARN_VALUE << ", " 
						 << rightNumbr << ", " << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << NUMBAR_VALUE << ", (" << NUMBAR_TYPE 
								 << ")" << rightNumbr << ")";
						} else {
							code << leftOperand << "." << NUMBAR_VALUE 
								 << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << NUMBAR_VALUE << ", " << rightNumbar << ")";
						} else {
							code << leftOperand << "." << NUMBAR_VALUE 
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
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBAR_VALUE << ", " << rightOperand << "." 
							 << TROOF_VALUE << " ? 1.0 : 0.0)";
					} else {
						code << leftOperand << "." << NUMBAR_VALUE << infixOp 
							 << "(" << rightOperand << "." << TROOF_VALUE 
							 << " ? 1.0 : 0.0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" 
					 << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << NUMBAR_VALUE << ", 0.0)";
					} else {
						code << leftOperand << "." << NUMBAR_VALUE 
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
					 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN << "(" 
					 << leftOperand << "." << YARN_VALUE << ", " << leftNumbr 
					 << ", " << leftNumbar << ");" << std::endl;
				code << "if(" << leftType << " == " << VARIABLE_TYPE_PREFIX 
					 << NUMBR_TYPE << ") {" << std::endl;
					code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VALUE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftNumbr << ", "
								 << rightOperand << "." << NUMBR_VALUE << ")";
						} else {
							code << leftNumbr << infixOp << rightOperand 
								 << "." << NUMBR_VALUE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "((" << NUMBAR_TYPE << ")"
								 << leftNumbr << ", " << rightOperand << "." 
								 << NUMBAR_VALUE << ")";
						} else {
							code << leftNumbr << infixOp << rightOperand 
								 << "." << NUMBAR_VALUE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
						 << ") {" << std::endl;
						code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
						code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
						code << VARIABLE_TYPE << " " << rightType << " = "
							 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN 
							 << "(" << rightOperand << "." << YARN_VALUE << ", " 
							 << rightNumbr << ", " << rightNumbar << ");" 
							 << std::endl;
						code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
							 << NUMBR_TYPE << ") {" << std::endl;
							code << ret << "." << VARIABLE_TYPEID << " = " 
								 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
								 << ";" << std::endl;
							code << ret << "." << NUMBR_VALUE << " = ";
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
							code << ret << "." << NUMBAR_VALUE << " = ";
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
						code << ret << "." << NUMBR_VALUE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftNumbr << ", " 
								 << rightOperand << "." << TROOF_VALUE 
								 << " ? 1 : 0)";
						} else {
							code << leftNumbr << infixOp << "(" << rightOperand 
								 << "." << TROOF_VALUE << " ? 1 : 0)";
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VALUE << " = ";
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
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftNumbar
								 << ", (" << NUMBAR_TYPE << ")" << rightOperand 
								 << "." << NUMBR_VALUE << ")";
						} else {
							code << leftNumbar << infixOp << rightOperand 
								 << "." << NUMBR_VALUE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftNumbar << ", "
								 << rightOperand << "." << NUMBAR_VALUE << ")";
						} else {
							code << leftNumbar << infixOp << rightOperand 
								 << "." << NUMBAR_VALUE;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE << ") {" 
						 << std::endl;
						code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
						code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
						code << VARIABLE_TYPE << " " << rightType << " = "
							 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN 
							 << "(" << rightOperand << "." << YARN_VALUE << ", " 
							 << rightNumbr << ", " << rightNumbar << ");" 
							 << std::endl;
						code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
							 << NUMBR_TYPE << ") {" << std::endl;
							code << ret << "." << VARIABLE_TYPEID << " = " 
								 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
								 << ";" << std::endl;
							code << ret << "." << NUMBAR_VALUE << " = ";
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
							code << ret << "." << NUMBAR_VALUE << " = ";
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
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftNumbar << ", "
								 << rightOperand << "." << TROOF_VALUE 
								 << " ? 1.0 : 0.0)";
						} else {
							code << leftNumbar << infixOp << "(" << rightOperand 
								 << "." << TROOF_VALUE << " ? 1.0 : 0.0)";
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
						 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" 
						 << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand 
								 << "." << NUMBAR_VALUE << ", 0.0)";
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
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VALUE << " ? 1 : 0, " 
							 << rightOperand << "." << NUMBR_VALUE << ")";
					} else {
						code << "(" << leftOperand << "." << TROOF_VALUE 
							 << " ? 1 : 0)" << infixOp << rightOperand 
							 << "." << NUMBR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VALUE << " ? 1.0 : 0.0, "
							 << rightOperand << "." << NUMBAR_VALUE << ")";
					} else {
						code << "(" << leftOperand << "." << TROOF_VALUE 
							 << " ? 1.0 : 0.0)" << infixOp << rightOperand 
							 << "." << NUMBAR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
					 << ") {" << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN 
						 << "(" << rightOperand << "." << YARN_VALUE << ", " 
						 << rightNumbr << ", " << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VALUE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << TROOF_VALUE << " ? 1 : 0, " 
								 << rightNumbr << ")";
						} else {
							code << "(" << leftOperand << "." << TROOF_VALUE 
								 << " ? 1 : 0)" << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(" << leftOperand << "." 
								 << TROOF_VALUE << " ? 1.0 : 0.0, " 
								 << rightNumbar << ")";
						} else {
							code << "(" << leftOperand << "." << TROOF_VALUE 
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
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VALUE << " ? 1 : 0, " << rightOperand 
							 << "." << TROOF_VALUE << " ? 1 : 0)";
					} else {
						code << "(" << leftOperand << "." << TROOF_VALUE 
							 << " ? 1 : 0)" << infixOp << "(" << rightOperand
							 << "." << TROOF_VALUE << " ? 1 : 0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(" << leftOperand << "." 
							 << TROOF_VALUE << " ? 1 : 0, 0)";
					} else {
						code << "(" << leftOperand << "." << TROOF_VALUE 
							 << " ? 1 : 0)" << infixOp << "0";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(0, " << rightOperand << "." 
							 << NUMBR_VALUE << ")";
					} else {
						code << "0" << infixOp << rightOperand << "." 
							 << NUMBR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBAR_VALUE << " = ";
					if( nonInfixOp != "" ) {
						code << nonInfixOp << "(" << "0.0, " << rightOperand 
							 << "." << NUMBAR_VALUE << ")";
					} else {
						code << "0.0" << infixOp << rightOperand << "." 
							 << NUMBAR_VALUE;
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << YARN_TYPE 
					 << ") {" << std::endl;
					code << NUMBR_TYPE << " " << rightNumbr << ";" << std::endl;
					code << NUMBAR_TYPE << " " << rightNumbar << ";" << std::endl;
					code << VARIABLE_TYPE << " " << rightType << " = "
						 << VARIABLE_STORAGE_PREFIX << EXTRACT_NUMERIC_FROM_YARN 
						 << "(" << rightOperand << "." << YARN_VALUE << ", " 
						 << rightNumbr << ", " << rightNumbar << ");" << std::endl;
					code << "if(" << rightType << " == " << VARIABLE_TYPE_PREFIX 
						 << NUMBR_TYPE << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBR_VALUE << " = ";
						if( nonInfixOp != "" && infixOp == "" ) {
							code << nonInfixOp << "(0, " << rightNumbr << ")";
						} else {
							code << "0" << infixOp << rightNumbr;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "} else if(" << rightType << " == " 
						 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
						 << ") {" << std::endl;
						code << ret << "." << VARIABLE_TYPEID << " = " 
							 << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ";" << std::endl;
						code << ret << "." << NUMBAR_VALUE << " = ";
						if( nonInfixOp != "" ) {
							code << nonInfixOp << "(0.0, " << rightNumbar << ")";
						} else {
							code << "0.0" << infixOp << rightNumbar;
						}
						code << ";" << std::endl;
						code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << TROOF_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(0, " << rightOperand 
							 << "." << TROOF_VALUE << " ? 1 : 0)";
					} else {
						code << "0" << infixOp << "(" << rightOperand
							 << "." << TROOF_VALUE << " ? 1 : 0)";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "} else if(" << rightOperand << "." << VARIABLE_TYPEID 
					 << " == " << VARIABLE_TYPE_PREFIX << NOOB_TYPE 
					 << ") {" << std::endl;
					code << ret << "." << VARIABLE_TYPEID << " = " 
						 << VARIABLE_TYPE_PREFIX << NUMBR_TYPE << ";" << std::endl;
					code << ret << "." << NUMBR_VALUE << " = ";
					if( nonInfixOp != "" && infixOp == "" ) {
						code << nonInfixOp << "(0, 0)";
					} else {
						code << "0" << infixOp << "0";
					}
					code << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "}" << std::endl;
			code << "throw " << LOLCODE_EXCEPTION << ";" << std::endl;
			break;
		}
		case AST::OperatorType::BOTH:
		case AST::OperatorType::EITHER:
		case AST::OperatorType::WON:
			code << ret << "." << VARIABLE_TYPEID << " = " << VARIABLE_TYPE_PREFIX 
				 << TROOF_TYPE << ";" << std::endl;
			code << ret << "." << TROOF_VALUE << " = " << leftOperand << "."
				 << EXTRACT_TROOF << infixOp << rightOperand << "." << EXTRACT_TROOF
				 << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
			break;
		case AST::OperatorType::SAEM:
		case AST::OperatorType::DIFFRINT:
			code << ret << "." << VARIABLE_TYPEID << " = " << VARIABLE_TYPE_PREFIX 
				 << TROOF_TYPE << ";" << std::endl;
			code << "switch(" << leftOperand << "." << VARIABLE_TYPEID
				 << ") {" << std::endl;
				code << "case " << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
					 << ":" << std::endl;
					code << "switch(" << rightOperand << "." << VARIABLE_TYPEID
					 << ") {" << std::endl;
						code << "case " << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ":" << std::endl;
							code << ret << "." << TROOF_VALUE << " = "
								 << leftOperand << "." << NUMBR_VALUE << infixOp
								 << rightOperand << "." << NUMBR_VALUE 
								 << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
						code << "case " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ":" << std::endl;
							code << ret << "." << TROOF_VALUE << " = "
								 << leftOperand << "." << NUMBR_VALUE << infixOp
								 << rightOperand << "." << NUMBAR_VALUE 
								 << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
					code << ret << "." << TROOF_VALUE << " = " 
						 << ( operatorType == AST::OperatorType::SAEM ? 
							  "false" : "true" ) << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "case " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
					 << ":" << std::endl;
					code << "switch(" << rightOperand << "." << VARIABLE_TYPEID
					 << ") {" << std::endl;
						code << "case " << VARIABLE_TYPE_PREFIX << NUMBR_TYPE 
							 << ":" << std::endl;
							code << ret << "." << TROOF_VALUE << " = "
								 << leftOperand << "." << NUMBAR_VALUE << infixOp
								 << rightOperand << "." << NUMBR_VALUE 
								 << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
						code << "case " << VARIABLE_TYPE_PREFIX << NUMBAR_TYPE 
							 << ":" << std::endl;
							code << ret << "." << TROOF_VALUE << " = "
								 << leftOperand << "." << NUMBAR_VALUE << infixOp
								 << rightOperand << "." << NUMBAR_VALUE 
								 << ";" << std::endl;
							code << "return " << ret << ";" << std::endl;
					code << "}" << std::endl;
					code << ret << "." << TROOF_VALUE << " = " 
						 << ( operatorType == AST::OperatorType::SAEM ? 
							  "false" : "true" ) << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
			code << "}" << std::endl;
			code << "if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << TROOF_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << TROOF_TYPE << ") {" << std::endl;
					code << ret << "." << TROOF_VALUE << " = " << leftOperand 
						 << "." << TROOF_VALUE << infixOp << rightOperand 
						 << "." << TROOF_VALUE << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << YARN_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << TROOF_TYPE << ") {" << std::endl;
					code << ret << "." << TROOF_VALUE << " = " << leftOperand 
						 << "." << YARN_VALUE << infixOp << rightOperand 
						 << "." << YARN_VALUE << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << NOOB_TYPE << ") {" << std::endl;
				code << ret << "." << TROOF_VALUE << " = " << rightOperand << "."
					 << VARIABLE_TYPEID << infixOp << VARIABLE_TYPE_PREFIX
					 << TROOF_TYPE << ";" << std::endl;
				code << "return " << ret << ";" << std::endl;
			code << "} else if(" << leftOperand << "." << VARIABLE_TYPEID << " == " 
				 << VARIABLE_TYPE_PREFIX << BUKKIT_TYPE << ") {" << std::endl;
				code << "if(" << rightOperand << "." << VARIABLE_TYPEID << " == " 
					 << VARIABLE_TYPE_PREFIX << BUKKIT_TYPE << ") {" << std::endl;
					code << ret << "." << TROOF_VALUE << " = " << leftOperand 
						 << "." << BUKKIT_VALUE << infixOp << rightOperand 
						 << "." << BUKKIT_VALUE << ";" << std::endl;
					code << "return " << ret << ";" << std::endl;
				code << "}" << std::endl;
			code << "}" << std::endl;
			code << ret << "." << TROOF_VALUE << " = " 
				 << ( operatorType == AST::OperatorType::SAEM ? "false" : "true" ) 
				 << ";" << std::endl;
			code << "return " << ret << ";" << std::endl;
			break;
	}
	
	code << "}" << std::endl;
}

void CodeGenerator::EmitNaryOperator( const std::string &operatorName, 
									  AST::OperatorType operatorType,
									  std::ostringstream &code ) {
	const std::string implFunction = "Impl" + operatorName;
	const std::string operand = "operand";
	const std::string operands = operand + "s";
	const std::string ret = "ret";
	std::string retType = "";
	std::string retValue = "";
	std::string operandType = "";
	std::string op = "";
	
	switch( operatorType ) {
		case AST::OperatorType::ANY:
			retType = TROOF_TYPE;
			operandType = EXTRACT_TROOF;
			op = " || ";
		case AST::OperatorType::ALL:
			retType = TROOF_TYPE;
			operandType = EXTRACT_TROOF;
			op = " && ";
			break;
		case AST::OperatorType::SMOOSH:
			retType = YARN_TYPE;
			operandType = EXTRACT_YARN;
			op = " + ";
			break;
		default:
			assert( false );
			break;
	}
	
	code << retType << " " << implFunction << "(const " << VARIABLE_STORAGE 
		 << " &" << operand << ") {" << std::endl;
	code << "return " << operand << "." << operandType << ";" << std::endl;
	code << "}" << std::endl;
	
	code << "template<typename... Operands>" << std::endl;
	code << retType << " " << implFunction << "(const " << VARIABLE_STORAGE << " &"
		 << operand << ", const Operands&... " << operands << ") {" << std::endl;
	code << "return " << operand << "." << operandType << op << implFunction 
		 << "(" << operands << "...);" << std::endl;
	code << "}" << std::endl;
	
	code << "template<typename... Operands>" << std::endl;
	code << retType << " " << operatorName << "(const Operands&... " 
		 << operands << ") {" << std::endl;
	code << VARIABLE_STORAGE << " " << ret << ";" << std::endl;
	code << ret << "." << VARIABLE_TYPEID << " = " << VARIABLE_TYPE_PREFIX 
		 << retType << ";" << std::endl;
	code << ret << "." << TROOF_VALUE << " = " << implFunction << "(" << operands
		 << "...);" << std::endl;
	code << "return " << ret << ";" << std::endl;
	code << "}" << std::endl;
}