#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>
#include <list>
#include <string>

#include "programvisitor.h"

#define SOURCE_PREFIX std::string("Source_")
#define SOURCE_DEFINED_PREFIX std::string("Defined_")
#define CAST_TO_PREFIX std::string("CastTo")
#define CAST_TO_TROOF (CAST_TO_PREFIX + TROOF_TYPE)
#define CAST_TO_NUMBR (CAST_TO_PREFIX + NUMBR_TYPE)
#define CAST_TO_NUMBAR (CAST_TO_PREFIX + NUMBAR_TYPE)
#define CAST_TO_YARN (CAST_TO_PREFIX + YARN_TYPE)
#define CAST_TO_BUKKIT (CAST_TO_PREFIX + BUKKIT_TYPE)
#define CAST_TO_NOOB (CAST_TO_PREFIX + NOOB_TYPE)
#define VARIABLE_STORAGE std::string("Variable")
#define VARIABLE_TYPE_PREFIX std::string("Variable::Type::")
#define TROOF_TYPE std::string("TROOF")
#define NUMBR_TYPE std::string("NUMBR")
#define NUMBAR_TYPE std::string("NUMBAR")
#define YARN_TYPE std::string("YARN")
#define BUKKIT_TYPE std::string("BUKKIT")
#define NOOB_TYPE std::string("NOOB")
#define IT_VARIABLE std::string("ItVariable")

class CodeGenerator : public AST::ASTProgramOrderVisitor {
#if 0
	private:
		static const std::string USER_FUNKSHUN_PREFIX;
		static const std::string USER_FUNKSHUN_DEFINED_PREFIX;
		static const std::string USER_VARIABLE_PREFIX;
		static const std::string CAST_TO_PREFIX;
		static const std::string CAST_TO_TROOF;
		static const std::string CAST_TO_NUMBR;
		static const std::string CAST_TO_NUMBAR;
		static const std::string CAST_TO_YARN;
		static const std::string CAST_TO_BUKKIT;
		static const std::string CAST_TO_NOOB;
		static const std::string VARIABLE_STORAGE;
		static const std::string VARIABLE_TYPE_PREFIX;
		static const std::string TROOF_TYPE;
		static const std::string NUMBR_TYPE;
		static const std::string NUMBAR_TYPE;
		static const std::string YARN_TYPE;
		static const std::string BUKKIT_TYPE;
		static const std::string NOOB_TYPE;
		static const std::string IT_VARIABLE;
#endif
		
	public:
		std::string &GetEmittedCode() { return codeOutput_; }
		
		void ProcessBegin( AST::ProgramBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::ProgramBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::ProgramGlobals *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::ProgramGlobals *node ) { 
			ProcessStatementBlockEnd( node, false ); 
		}
		void ProcessBegin( AST::FunkshunBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::FunkshunBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::ORlyYaBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::ORlyYaBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::ORlyMebbeBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::ORlyMebbeBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::ORlyNoBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::ORlyNoBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::WtfOmgBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::WtfOmgBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::WtfOmgwtfBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::WtfOmgwtfBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::LoopBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::LoopBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
#if 0
		void ProcessBegin( AST::PlzBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::PlzBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::PlzONoesBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::PlzONoesBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
		void ProcessBegin( AST::PlzOWelBody *node ) { 
			ProcessStatementBlockBegin( node ); 
		}
		void ProcessEnd( AST::PlzOWelBody *node ) { 
			ProcessStatementBlockEnd( node ); 
		}
#endif
			
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
		void ProcessBegin( AST::VarDeclare *node ) { ProcessBegin(); }
		void ProcessEnd( AST::VarDeclare * );
		void ProcessBegin( AST::VarAssign *node ) { ProcessBegin(); }
		void ProcessEnd( AST::VarAssign * );
		//void ProcessBegin( AST::VarCast * );
		//void ProcessEnd( AST::VarCast * );
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
		//void ProcessBegin( AST::SlotIdentifier *node ) { ProcessBegin(); }
		//void ProcessEnd( AST::SlotIdentifier * );
		//void ProcessBegin( AST::CastExpression *node ) { ProcessBegin(); }
		//void ProcessEnd( AST::CastExpression * );
		
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
	
		void ProcessStatementBlockBegin( AST::StatementBlock *node ) { 
			ProcessBegin(); 
		}
		void ProcessStatementBlockEnd( AST::StatementBlock *, bool = true );
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
	
	private:
		std::string codeOutput_;
		std::stack< std::list<std::string> > codeSegments_;
};

#endif