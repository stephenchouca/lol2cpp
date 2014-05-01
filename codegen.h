#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>
#include <list>
#include <string>
#include <unordered_map>

#include "program_visitor.h"

class CodeGenerator : public AST::ASTProgramOrderVisitor {
	private:
		static const std::string TROOF_TYPE;
		static const std::string NUMBR_TYPE;
		static const std::string NUMBAR_TYPE;
		static const std::string YARN_TYPE;
		static const std::string BUKKIT_TYPE;
		static const std::string NOOB_TYPE;
		static const std::string SOURCE_PREFIX;
		static const std::string SOURCE_DEFINED_PREFIX;
		static const std::string EXTRACT_PREFIX;
		static const std::string EXTRACT_TROOF;
		static const std::string EXTRACT_NUMBR;
		static const std::string EXTRACT_NUMBAR;
		static const std::string EXTRACT_YARN;
		static const std::string EXTRACT_BUKKIT;
		static const std::string EXTRACT_NOOB;
		static const std::string EXTRACT_FROM_BUKKIT;
		static const std::string EXTRACT_FROM_BUKKIT_UNSAFE;
		static const std::string EXTRACT_NUMERIC_FROM_YARN;
		static const std::string VARIABLE_STORAGE;
		static const std::string VARIABLE_TYPE;
		static const std::string VARIABLE_TYPE_PREFIX;
		static const std::string VARIABLE_TYPEID;
		static const std::string TROOF_VALUE;
		static const std::string NUMBR_VALUE;
		static const std::string NUMBAR_VALUE;
		static const std::string YARN_VALUE;
		static const std::string BUKKIT_VALUE;
		static const std::string CREATE_LITERAL_PREFIX;
		static const std::string CREATE_TROOF_LITERAL;
		static const std::string CREATE_NUMBR_LITERAL;
		static const std::string CREATE_NUMBAR_LITERAL;
		static const std::string CREATE_YARN_LITERAL;
		static const std::string CREATE_NOOB_LITERAL;
		static const std::string IT_VARIABLE;
		static const std::string LOLCODE_EXCEPTION;
		
	public:
		std::string &GetEmittedCode() { return emittedCode_; }
		
		void ProcessBegin( AST::StatementBlock *node ) { ProcessBegin(); }
		void ProcessEnd( AST::StatementBlock *node );
		void ProcessBegin( AST::ORlyBlock *node ) { ProcessBegin(); }
		void ProcessEnd( AST::ORlyBlock * );
		void ProcessBegin( AST::WtfBlock *node ) { ProcessBegin(); }
		void ProcessEnd( AST::WtfBlock * );
		void ProcessBegin( AST::ForLoopBlock *node ) { ProcessBegin(); }
		void ProcessEnd( AST::ForLoopBlock * );
		void ProcessBegin( AST::RangeLoopBlock *node ) { ProcessBegin(); }
		void ProcessEnd( AST::RangeLoopBlock * );
		void ProcessBegin( AST::FunkshunBlock *node ) { ProcessBegin(); }
		void ProcessEnd( AST::FunkshunBlock * );
		void ProcessBegin( AST::FunkshunDeclare *node ) { ProcessBegin(); }
		void ProcessEnd( AST::FunkshunDeclare * );
		//void ProcessBegin( AST::PlzBlock * );
		//void ProcessEnd( AST::PlzBlock * );
		void ProcessBegin( AST::LiteralVarDeclare *node ) { ProcessBegin(); }
		void ProcessEnd( AST::LiteralVarDeclare * );
		//void ProcessBegin( AST::SrsVarDeclare *node ) { ProcessBegin(); }
		//void ProcessEnd( AST::SrsVarDeclare * );
		//void ProcessBegin( AST::SlotVarDeclare *node ) { ProcessBegin(); }
		//void ProcessEnd( AST::SlotVarDeclare * );
		void ProcessBegin( AST::VarAssign *node ) { ProcessBegin(); }
		void ProcessEnd( AST::VarAssign * );
		void ProcessBegin( AST::FunkshunReturn *node ) { ProcessBegin(); }
		void ProcessEnd( AST::FunkshunReturn * );
		void Process( AST::GtfoStatement * );
		void ProcessBegin( AST::VisibleStatement *node ) { ProcessBegin(); }
		void ProcessEnd( AST::VisibleStatement * );
		void ProcessBegin( AST::GimmehStatement *node ) { ProcessBegin(); }
		void ProcessEnd( AST::GimmehStatement * );
		void ProcessBegin( AST::Program *node ) { ProcessBegin(); }
		void ProcessEnd( AST::Program * );
		
		void Process( AST::TroofLiteral * );
		void Process( AST::NumbrLiteral * );
		void Process( AST::NumbarLiteral * );
		void Process( AST::YarnLiteral * );
		void Process( AST::NoobLiteral * );
		void Process( AST::TypeIdentifier * );
		void Process( AST::LiteralIdentifier * );
		//void ProcessBegin( AST::SrsIdentifier * );
		//void ProcessEnd( AST::SrsIdentifier * );
		void Process( AST::ItIdentifier * );
		void ProcessBegin( AST::SlotIdentifier *node ) { ProcessBegin(); }
		void ProcessEnd( AST::SlotIdentifier * );
		void ProcessBegin( AST::MaekExpression *node ) { ProcessBegin(); }
		void ProcessEnd( AST::MaekExpression * );
		
		void ProcessBegin( AST::NotExpression *node ) {
			ProcessUnaryExpressionBegin( node );
		}
		void ProcessEnd( AST::NotExpression *node ) {
			ProcessUnaryExpressionEnd( node );
		}
		void ProcessBegin( AST::UppinExpression *node ) {
			ProcessUnaryExpressionBegin( node );
		}
		void ProcessEnd( AST::UppinExpression *node ) {
			ProcessUnaryExpressionEnd( node );
		}
		void ProcessBegin( AST::NerfinExpression *node ) {
			ProcessUnaryExpressionBegin( node );
		}
		void ProcessEnd( AST::NerfinExpression *node ) {
			ProcessUnaryExpressionEnd( node );
		}
		void ProcessBegin( AST::TilExpression *node ) {
			ProcessUnaryExpressionBegin( node );
		}
		void ProcessEnd( AST::TilExpression *node ) {
			ProcessUnaryExpressionEnd( node );
		}
		void ProcessBegin( AST::WileExpression *node ) {
			ProcessUnaryExpressionBegin( node );
		}
		void ProcessEnd( AST::WileExpression *node ) {
			ProcessUnaryExpressionEnd( node );
		}
		
		void ProcessBegin( AST::SumExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::SumExpression *node ) { 
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::DiffExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::DiffExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::ProduktExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::ProduktExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::QuoshuntExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::QuoshuntExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::ModExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::ModExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::BiggrExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::BiggrExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::SmallrExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::SmallrExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::BothExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::BothExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::EitherExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::EitherExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::WonExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::WonExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::SaemExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::SaemExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		void ProcessBegin( AST::DiffrintExpression *node ) { 
			ProcessBinaryExpressionBegin( node );
		}
		void ProcessEnd( AST::DiffrintExpression *node ) {
			ProcessBinaryExpressionEnd( node );
		}
		
		void ProcessBegin( AST::AllExpression *node ) { 
			ProcessNaryExpressionBegin( node );
		}
		void ProcessEnd( AST::AllExpression *node ) { 
			ProcessNaryExpressionEnd( node );
		}
		void ProcessBegin( AST::AnyExpression *node ) { 
			ProcessNaryExpressionBegin( node );
		}
		void ProcessEnd( AST::AnyExpression *node ) { 
			ProcessNaryExpressionEnd( node );
		}
		void ProcessBegin( AST::SmooshExpression *node ) { 
			ProcessNaryExpressionBegin( node );
		}
		void ProcessEnd( AST::SmooshExpression *node ) { 
			ProcessNaryExpressionEnd( node );
		}
		void ProcessBegin( AST::FunkshunCall *node ) { ProcessBegin(); }
		void ProcessEnd( AST::FunkshunCall * );
	
	private:
		void ProcessBegin() { codeSegments_.emplace(); }
	
		void ProcessUnaryExpressionBegin( AST::UnaryExpression *node ) { 
			ProcessBegin();
		}
		void ProcessUnaryExpressionEnd( AST::UnaryExpression * );
		void ProcessBinaryExpressionBegin( AST::BinaryExpression *node ) { 
			ProcessBegin(); 
		}
		void ProcessBinaryExpressionEnd( AST::BinaryExpression * );
		void ProcessNaryExpressionBegin( AST::NaryExpression *node ) { 
			ProcessBegin(); 
		}
		void ProcessNaryExpressionEnd( AST::NaryExpression * );
	
		void EmitBoilerplate( std::ostringstream & );
		void EmitUnaryOperator( const std::string &, 
								AST::OperatorType, 
								std::ostringstream & );
		void EmitBinaryOperator( const std::string &, 
								 AST::OperatorType, 
								 std::ostringstream & );
		void EmitNaryOperator( const std::string &, 
							   AST::OperatorType, 
							   std::ostringstream & );
									   
	private:
		std::string emittedCode_;
		std::stack< std::list<std::string> > codeSegments_;
		
		std::unordered_map<std::string, AST::OperatorType> requiredOperators_;
};

#endif