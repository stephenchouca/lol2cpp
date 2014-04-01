#include <cassert>
#include <iostream>

#include "statement.h"
#include "expression.h"

namespace AST {	
	StatementBlock::~StatementBlock() {
		while( !stmts_.empty() ) {
			delete stmts_.back();
			stmts_.pop_back();
		}
	}
	
	void StatementBlock::Print( std::ostream &out ) {
		bool newline = false;
		for( StatementListIterator it = stmts_.cbegin();
			 it != stmts_.cend(); ++it, newline = true ) {
			if( newline ) {
				out << std::endl;
			}
			Statement *stmt = *it;
			out << *stmt;
		}
	}

	void StatementBlock::AddStatement( Statement *stmt ) {
		assert( stmt != nullptr );
		stmts_.push_back( stmt );
		stmt->SetParent( this );
	}
		
	ORlyBlock::ORlyBlock( StatementBlock *yaRlyBlock ) :
			yaRlyBlock_( yaRlyBlock ), noWaiBlock_( nullptr ) {
		assert( yaRlyBlock_ != nullptr );
		yaRlyBlock_->SetParent( this );
	}
	
	ORlyBlock::~ORlyBlock() {
		delete yaRlyBlock_;
		
		while( !meebeConditions_.empty() ) {
			assert( !meebeBlocks_.empty() );
			delete meebeConditions_.back();
			delete meebeBlocks_.back();
			meebeConditions_.pop_back();
			meebeBlocks_.pop_back();
		}
		assert( meebeBlocks_.empty() );
		
		delete noWaiBlock_;
	}
	
	void ORlyBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "O RLY:";
		
		if( yaRlyBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "YA RLY:" 
				<< std::endl << *yaRlyBlock_;
		}
		
		ExpressionListIterator conditionIt;
		StatementBlockListIterator stmtBlockIt;
		for( conditionIt = meebeConditions_.cbegin(), 
			 stmtBlockIt = meebeBlocks_.cbegin();
			 conditionIt != meebeConditions_.cend() && 
			 stmtBlockIt != meebeBlocks_.cend(); ++conditionIt, ++stmtBlockIt ) {
			assert( conditionIt != meebeConditions_.cend() );
			assert( stmtBlockIt != meebeBlocks_.cend() );
			Expression *condition = *conditionIt;
			StatementBlock *stmtBlock = *stmtBlockIt;
			out << std::endl << DebugIndent( 1 ) << "MEEBE:" 
				<< std::endl << *condition << std::endl << *stmtBlock;
		}
		
		if( noWaiBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "NO WAI:" 
				<< std::endl << *noWaiBlock_;
		}
	}
	
	void ORlyBlock::AddMeebeBlock( Expression *meebeCond, 
								   StatementBlock *meebeBlock ) {
		assert( meebeCond != nullptr );
		assert( meebeBlock != nullptr );
		meebeConditions_.push_back( meebeCond );
		meebeBlocks_.push_back( meebeBlock );
		meebeCond->SetParent( this );
		meebeBlock->SetParent( this );
	}
	
	void ORlyBlock::SetNoWaiBlock( StatementBlock *noWaiBlock ) {
		assert( noWaiBlock != nullptr );
		noWaiBlock_ = noWaiBlock;
		noWaiBlock_->SetParent( this );
	}
	
	WtfBlock::~WtfBlock() {
		while( !omgLabels_.empty() ) {
			assert( !omgStmtBlocks_.empty() );
			delete omgLabels_.back();
			delete omgStmtBlocks_.back();
			omgLabels_.pop_back();
			omgStmtBlocks_.pop_back();
		}
		assert( omgStmtBlocks_.empty() );
		
		delete omgwtfBlock_;
	}
	
	void WtfBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "WTF:";
		
		LiteralListIterator labelIt;
		StatementBlockListIterator stmtBlockIt;
		for( labelIt = omgLabels_.cbegin(), stmtBlockIt = omgStmtBlocks_.cbegin();
			 labelIt != omgLabels_.cend() && stmtBlockIt != omgStmtBlocks_.cend();
			 ++labelIt, ++stmtBlockIt ) {
			assert( labelIt != omgLabels_.cend() );
			assert( stmtBlockIt != omgStmtBlocks_.cend() );
			Literal *label = *labelIt;
			StatementBlock *stmtBlock = *stmtBlockIt;
			out << std::endl << DebugIndent( 1 ) << "OMG:" 
				<< std::endl << *label << std::endl << *stmtBlock;
		}
		
		if( omgwtfBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "OMGWTF:" 
				<< std::endl << *omgwtfBlock_;
		}
	}
	
	void WtfBlock::AddOmgBlock( Literal *label, StatementBlock *stmtBlock ) {
		assert( label != nullptr );
		assert( stmtBlock != nullptr );
		omgLabels_.push_back( label );
		omgStmtBlocks_.push_back( stmtBlock );
		label->SetParent( this );
		stmtBlock->SetParent( this );
	}
	
	void WtfBlock::SetOmgwtfBlock( StatementBlock *omgwtfBlock ) {
		assert( omgwtfBlock != nullptr );
		omgwtfBlock_ = omgwtfBlock;
		omgwtfBlock_->SetParent( this );
	}
	
	LoopBlock::LoopBlock( LiteralIdentifier *loopId ) : loopId_( loopId ), 
			body_( nullptr ), loopVar_( nullptr ), loopVarIsLocal_( false ) {
		assert( loopId_ != nullptr );
		loopId_->SetParent( this );
	}
	
	LoopBlock::~LoopBlock() {
		delete loopId_;
		delete body_;
		delete loopVar_;
	}
	
	void LoopBlock::Print( std::ostream &out ) {
		out << DebugIndent() << GetLoopType() << ":" << loopId_->GetIdentifier();
		
		if( loopVar_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Loop Var:" 
				<< std::endl << *loopVar_;
			if( loopVarIsLocal_ ) {
				out << std::endl << DebugIndent( 2 ) << "isLocal";
			}
		}
		
		PrintLoopSpecific( out );
		
		if( body_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Body:" << std::endl << *body_;
		}
	}
	
	void LoopBlock::SetBody( StatementBlock *body ) {
		assert( body != nullptr );
		body_ = body;
		body_->SetParent( this );
	}
	
	void LoopBlock::SetLoopVariable( Identifier *loopVar, bool isLocal ) {
		assert( loopVar != nullptr );
		loopVar_ = loopVar;
		loopVarIsLocal_ = isLocal;
	}
	
	ForLoopBlock::ForLoopBlock( LiteralIdentifier *loopId ) : LoopBlock( loopId ), 
		loopVarIncExpr_( nullptr ), 
		loopVarInitExpr_( nullptr ),
		loopGuard_( nullptr ) {}
	
	ForLoopBlock::~ForLoopBlock() {
		delete loopVarIncExpr_;
		delete loopVarInitExpr_;
		delete loopGuard_;
	}
	
	void ForLoopBlock::PrintLoopSpecific( std::ostream &out ) {
		if( loopVarInitExpr_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Loop Var Init:"
				<< std::endl << *loopVarInitExpr_;
		}
		if( loopVarIncExpr_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Loop Var Inc:"
				<< std::endl << *loopVarIncExpr_;
		}
	}
	
	void ForLoopBlock::SetLoopVariable( Identifier *loopVar, bool isLocal ) {
		LoopBlock::SetLoopVariable( loopVar, isLocal );
		
		if( loopVarIncExpr_ == nullptr ) {
			return;
		}
		
		UnaryExpression *unaryIncExpr = 
			dynamic_cast<UnaryExpression *>( loopVarIncExpr_ );
		if( unaryIncExpr != nullptr ) {
			Identifier *loopVar = dynamic_cast<Identifier *>( loopVar_->Clone() );
			unaryIncExpr->SetOperand( loopVar );
			loopVar->SetParent( unaryIncExpr );
			return;
		}
		
		FunkshunCall *funkshunIncExpr = 
			dynamic_cast<FunkshunCall *>( loopVarIncExpr_ );
		if( funkshunIncExpr != nullptr ) {
			Identifier *loopVar = dynamic_cast<Identifier *>( loopVar_->Clone() );
			funkshunIncExpr->AddOperand( loopVar );
			loopVar->SetParent( funkshunIncExpr );
			return;
		}
		
		assert( false );
	}
	
	void ForLoopBlock::SetLoopVariableIncExpr( Expression *incExpr ) {
		assert( incExpr != nullptr );
		loopVarIncExpr_ = incExpr;
		loopVarIncExpr_->SetParent( this );
	}
	
	void ForLoopBlock::SetLoopVariableInitExpr( Expression *initExpr ) {
		assert( initExpr != nullptr );
		loopVarInitExpr_ = initExpr;
		loopVarInitExpr_->SetParent( this );
	}
	
	void ForLoopBlock::SetLoopGuard( UnaryBooleanExpression *guardExpr ) {
		assert( guardExpr != nullptr );
		loopGuard_ = guardExpr;
		loopGuard_->SetParent( this );
	}
	
	RangeLoopBlock::RangeLoopBlock( LiteralIdentifier *loopId ) : 
		LoopBlock( loopId ), bukkitRef_( nullptr ) {}
	
	RangeLoopBlock::~RangeLoopBlock() {
		delete bukkitRef_;
	}
	
	void RangeLoopBlock::PrintLoopSpecific( std::ostream &out ) {
		if( bukkitRef_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Bukkit Ref:"
				<< std::endl << *bukkitRef_;
		}
	}
	
	void RangeLoopBlock::SetBukkitRef( Expression *bukkitRef ) {
		assert( bukkitRef != nullptr );
		bukkitRef_ = bukkitRef;
		bukkitRef_->SetParent( this );
	}
	
	FunkshunBlock::FunkshunBlock( LiteralIdentifier *id ) : 
			funkshunId_( id ), body_( nullptr ) {
		assert( funkshunId_ != nullptr );
		funkshunId_->SetParent( this );
	}
	
	FunkshunBlock::~FunkshunBlock() {
		delete funkshunId_;
		
		while( !params_.empty() ) {
			delete params_.back();
			params_.pop_back();
		}
		
		delete body_;
	}
	
	void FunkshunBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "FUNKSHUN: " << funkshunId_->GetIdentifier();
		
		if( !params_.empty() ) {
			out << std::endl << DebugIndent( 1 ) << "Parameters:";
			for( LiteralIdentifierListIterator it = params_.cbegin();
				 it != params_.cend(); ++it ) {
				LiteralIdentifier *param = *it;
				out << std::endl << *param;
			}
		}
		
		if( body_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Body:" << std::endl << *body_;
		}
	}
	
	void FunkshunBlock::AddParameter( LiteralIdentifier *param ) {
		assert( param != nullptr );
		params_.push_back( param );
		param->SetParent( this );
	}
	
	void FunkshunBlock::SetBody( StatementBlock *body ) {
		assert( body != nullptr );
		body_ = body;
		body_->SetParent( this );
	}
	
	PlzBlock::PlzBlock( StatementBlock *tryBlock ) : 
			tryBlock_( tryBlock ), wellBlock_( nullptr ) {
		assert( tryBlock_ != nullptr );
		tryBlock_->SetParent( this );
	}
	
	PlzBlock::~PlzBlock() {
		delete tryBlock_;
		
		while( !exceptions_.empty() ) {
			assert( !handlers_.empty() );
			delete exceptions_.back();
			delete handlers_.back();
			exceptions_.pop_back();
			handlers_.pop_back();
		}
		assert( handlers_.empty() );
		
		delete wellBlock_;
	}
	
	void PlzBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "PLZ:";
		
		if( tryBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Do:" << std::endl << *tryBlock_;
		}
		
		ExpressionListIterator exceptIt;
		StatementBlockListIterator handlerIt;
		for( exceptIt = exceptions_.cbegin(), handlerIt = handlers_.cbegin();
			 exceptIt != exceptions_.cend() && handlerIt != handlers_.cend();
			 ++exceptIt, ++handlerIt ) {
			assert( exceptIt != exceptions_.cend() );
			assert( handlerIt != handlers_.cend() );
			Expression *except = *exceptIt;
			StatementBlock *handler = *handlerIt;
			out << std::endl << DebugIndent( 1 ) << "NOES:" 
				<< std::endl << *except << std::endl << *handler;
		}
		
		if( wellBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "WELL:" 
				<< std::endl << *wellBlock_;
		}
	}
	
	void PlzBlock::AddNoesBlock( Expression *exception, StatementBlock *handler ) {
		assert( exception != nullptr );
		assert( handler != nullptr );
		exceptions_.push_back( exception );
		handlers_.push_back( handler );
		exception->SetParent( this );
		handler->SetParent( this );
	}
	
	void PlzBlock::SetWellBlock( StatementBlock *wellBlock ) {
		assert( wellBlock != nullptr );
		wellBlock_ = wellBlock;
		wellBlock_->SetParent( this );
	}
	
	VarDeclare::VarDeclare( Identifier *varId ) :
			varId_( varId ), initVal_( nullptr ), initType_( nullptr ) {
		assert( varId_ != nullptr );
		varId_->SetParent( this );
	}
	
	VarDeclare::~VarDeclare() {
		delete varId_;
		delete initVal_;
		delete initType_;
	}
	
	void VarDeclare::Print( std::ostream &out ) {
		out << DebugIndent() << "VAR DECLARE:";
		if( varId_ != nullptr ) {
			out << std::endl << *varId_;
		}
		if( initVal_ != nullptr ) {
			out << std::endl << *initVal_;
		}
		if( initType_ != nullptr ) {
			out << std::endl << *initType_;
		}
	}
	
	void VarDeclare::SetInitValue( Expression *initVal ) {
		assert( initType_ == nullptr );
		assert( initVal != nullptr );
		initVal_ = initVal;
		initVal_->SetParent( this );
	}
	
	void VarDeclare::SetInitType( TypeIdentifier *initType ) {
		assert( initVal_ == nullptr );
		assert( initType != nullptr );
		initType_ = initType;
		initType_->SetParent( this );
	}
	
	VarAssign::VarAssign( Identifier *varId ) :
			varId_( varId ), assignVal_( nullptr ) {
		assert( varId_ != nullptr );
		varId_->SetParent( this );
	}
	
	VarAssign::~VarAssign() {
		delete varId_;
		delete assignVal_;
	}
	
	void VarAssign::Print( std::ostream &out ) {
		out << DebugIndent() << "VAR ASSIGN:";
		if( varId_ != nullptr ) {
			out << std::endl << *varId_;
		}
		if( assignVal_ != nullptr ) {
			out << std::endl << *assignVal_;
		}
	}
	
	void VarAssign::SetAssignValue( Expression *assignVal ) {
		assert( assignVal != nullptr );
		assignVal_ = assignVal;
		assignVal_->SetParent( this );
	}
	
	VarCast::VarCast( Identifier *varId ) :
			varId_( varId ), targetType_( nullptr ) {
		assert( varId_ != nullptr );
		varId_->SetParent( this );
	}
	
	VarCast::~VarCast() {
		delete varId_;
		delete targetType_;
	}
	
	void VarCast::Print( std::ostream &out ) {
		out << DebugIndent() << "VAR CAST:";
		if( varId_ != nullptr ) {
			out << std::endl << *varId_;
		}
		if( targetType_ != nullptr ) {
			out << std::endl << *targetType_;
		}
	}
	
	void VarCast::SetCastTargetType( TypeIdentifier *typeId ) {
		assert( typeId != nullptr );
		targetType_ = typeId;
		targetType_->SetParent( this );
	}
	
	FunkshunReturn::FunkshunReturn( Expression *retVal ) :
			retVal_( retVal ) {
		assert( retVal_ != nullptr );
		retVal_->SetParent( this );
	}
	
	FunkshunReturn::~FunkshunReturn() {
		delete retVal_;
	}
	
	void FunkshunReturn::Print( std::ostream &out ) {
		out << DebugIndent() << "RETURN:";
		if( retVal_ != nullptr ) {
			out << std::endl << *retVal_;
		}
	}
	
	VisibleStatement::VisibleStatement( Expression *expr ) {
		AddExpression( expr );
	}
	
	VisibleStatement::~VisibleStatement() {
		while( !exprs_.empty() ) {
			delete exprs_.back();
			exprs_.pop_back();
		}
	}
	
	void VisibleStatement::Print( std::ostream &out ) {
		out << DebugIndent() << "VISIBLE:";
		
		for( ExpressionListIterator it = exprs_.cbegin();
			 it != exprs_.cend(); ++it ) {
			Expression *expr = *it;
			assert( expr != nullptr );
			out << std::endl << *expr;
		}
	}
	
	void VisibleStatement::AddExpression( Expression *expr ) {
		assert( expr != nullptr );
		exprs_.push_back( expr );
		expr->SetParent( this );
	}
	
	GimmehStatement::GimmehStatement( Identifier *targetId, bool isLong ) :
			targetId_( targetId ), isLong_( isLong ) {
		assert( targetId_ != nullptr );
		targetId_->SetParent( this );
	}
	
	GimmehStatement::~GimmehStatement() {
		delete targetId_;
	}
	
	void GimmehStatement::Print( std::ostream &out ) {
		out << DebugIndent() << "GIMMEH:";
		if( targetId_ != nullptr ) {
			out << std::endl << *targetId_;
		}
		if( isLong_ ) {
			out << std::endl << DebugIndent( 1 ) << "isLong";
		}
	}
	
	Program::Program( StatementBlock *stmtBlock ) : stmtBlock_( stmtBlock ) {
		assert( stmtBlock_ != nullptr );
		stmtBlock_->SetParent( this );
	}
	
	void Program::Print( std::ostream &out ) {
		out << DebugIndent() << "PROGRAM:";
		if( stmtBlock_ != nullptr ) {
			out << std::endl << *stmtBlock_;
		}
	}
}