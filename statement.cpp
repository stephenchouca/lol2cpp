#include <cassert>

#include "statement.h"
#include "expression.h"

namespace AST {
	Program::Program( StatementBlock *stmtBlock ) : stmtBlock_( stmtBlock ) {
		assert( stmtBlock_ != nullptr );
		stmtBlock_->SetParent( this );
	}
	
	StatementBlock::~StatementBlock() {
		while( !stmts_.empty() ) {
			delete stmts_.back();
			stmts_.pop_back();
		}
	}

	void StatementBlock::AddStatement( Statement *stmt ) {
		assert( stmt != nullptr );
		stmts_.push_back( stmt );
		stmt->SetParent( this );
	}
	
	GimmehStatement::GimmehStatement( Identifier *targetId ) :
			targetId_( targetId ) {
		assert( targetId_ != nullptr );
		targetId_->SetParent( this );
	}
	
	GimmehStatement::~GimmehStatement() {
		delete targetId_;
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
	
	void VisibleStatement::AddExpression( Expression *expr ) {
		assert( expr != nullptr );
		exprs_.push_back( expr );
		expr->SetParent( this );
	}
	
	FunkshunReturn::FunkshunReturn( Expression *retVal ) :
			retVal_( retVal ) {
		assert( retVal_ != nullptr );
		retVal_->SetParent( this );
	}
	
	FunkshunReturn::~FunkshunReturn() {
		delete retVal_;
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
	
	void VarCast::SetCastTargetType( TypeIdentifier *typeId ) {
		assert( typeId != nullptr );
		targetType_ = typeId;
		targetType_->SetParent( this );
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
	
	void VarAssign::SetAssignValue( Expression *assignVal ) {
		assert( assignVal != nullptr );
		assignVal_ = assignVal;
		assignVal_->SetParent( this );
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
		while( !wtfConditions_.empty() ) {
			assert( !wtfStmtBlocks_.empty() );
			delete wtfConditions_.back();
			delete wtfStmtBlocks_.back();
			wtfConditions_.pop_back();
			wtfStmtBlocks_.pop_back();
		}
		assert( wtfStmtBlocks_.empty() );
		
		delete omgwtfBlock_;
	}
	
	void WtfBlock::AddWtfStmtBlock( Literal *cond, StatementBlock *stmtBlock ) {
		assert( cond != nullptr );
		assert( stmtBlock != nullptr );
		wtfConditions_.push_back( cond );
		wtfStmtBlocks_.push_back( stmtBlock );
		cond->SetParent( this );
		stmtBlock->SetParent( this );
	}
	
	void WtfBlock::SetOmgwtfBlock( StatementBlock *omgwtfBlock ) {
		assert( omgwtfBlock != nullptr );
		omgwtfBlock_ = omgwtfBlock;
		omgwtfBlock_->SetParent( this );
	}
	
	FunkshunBlock::FunkshunBlock( LiteralIdentifier *id ) : 
			funkshunId_( id ), funkshunBody_( nullptr ) {
		assert( funkshunId_ != nullptr );
		funkshunId_->SetParent( this );
	}
	
	FunkshunBlock::~FunkshunBlock() {
		delete funkshunId_;
		
		while( !params_.empty() ) {
			delete params_.back();
			params_.pop_back();
		}
		
		delete funkshunBody_;
	}
	
	void FunkshunBlock::AddParameter( LiteralIdentifier *param ) {
		assert( param != nullptr );
		params_.push_back( param );
		param->SetParent( this );
	}
	
	void FunkshunBlock::SetBody( StatementBlock *body ) {
		assert( body != nullptr );
		funkshunBody_ = body;
		funkshunBody_->SetParent( this );
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
	
	void RangeLoopBlock::SetBukkitRef( Expression *bukkitRef ) {
		assert( bukkitRef != nullptr );
		bukkitRef_ = bukkitRef;
		bukkitRef_->SetParent( this );
	}
	
	PlzBlock::PlzBlock( StatementBlock *tryBlock ) : 
			tryBlock_( tryBlock ), wellBlock_( nullptr ) {
		assert( tryBlock_ != nullptr );
		tryBlock_->SetParent( this );
	}
	
	PlzBlock::~PlzBlock() {
		delete tryBlock_;
		
		while( !exceptions_.empty() ) {
			assert( !noesBlocks_.empty() );
			delete exceptions_.back();
			delete noesBlocks_.back();
			exceptions_.pop_back();
			noesBlocks_.pop_back();
		}
		assert( noesBlocks_.empty() );
		
		delete wellBlock_;
	}
	
	void PlzBlock::AddNoesBlock( Expression *exception, StatementBlock *noesBlock ) {
		assert( exception != nullptr );
		assert( noesBlock != nullptr );
		exceptions_.push_back( exception );
		noesBlocks_.push_back( noesBlock );
		exception->SetParent( this );
		noesBlock->SetParent( this );
	}
	
	void PlzBlock::SetWellBlock( StatementBlock *wellBlock ) {
		assert( wellBlock != nullptr );
		wellBlock_ = wellBlock;
		wellBlock_->SetParent( this );
	}
}