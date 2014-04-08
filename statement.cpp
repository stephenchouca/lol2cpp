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
		for( StatementListIterator it = stmts_.begin();
			 it != stmts_.end(); ++it, newline = true ) {
			if( newline ) {
				out << std::endl;
			}
			Statement *stmt = *it;
			assert( stmt != nullptr );
			out << *stmt;
		}
	}

	void StatementBlock::AddStatement( Statement *stmt ) {
		assert( stmt != nullptr );
		stmts_.push_back( stmt );
		stmt->SetParent( this );
	}
	
	void StatementBlock::SwapStatement( StatementListIterator &it, Statement *stmt ) {
		if( it == stmts_.end() ) {
			return;
		}
		
		if( stmt != nullptr ) {
			stmts_.insert( it, stmt );
			stmt->SetParent( this );
		}
		
		it = stmts_.erase( it );
	}
		
	ORlyBlock::ORlyBlock( StatementBlock *yaRlyBlock ) :
			yaRlyBlock_( yaRlyBlock ), noWaiBlock_( nullptr ) {
		assert( yaRlyBlock_ != nullptr );
		yaRlyBlock_->SetParent( this );
	}
	
	ORlyBlock::~ORlyBlock() {
		delete yaRlyBlock_;
		
		while( !mebbeBlocks_.empty() ) {
			delete mebbeBlocks_.back().first;
			delete mebbeBlocks_.back().second;
			mebbeBlocks_.pop_back();
		}
		
		delete noWaiBlock_;
	}
	
	void ORlyBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "O RLY:";
		
		if( yaRlyBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "YA RLY:" 
				<< std::endl << DebugIndent( 2 ) << "Body:"
				<< std::endl << *yaRlyBlock_;
		}
		
		for( MebbeBlockListIterator mebbeBlockIt = mebbeBlocks_.begin();
			 mebbeBlockIt != mebbeBlocks_.end(); ++mebbeBlockIt ) {
			out << std::endl << DebugIndent( 1 ) << "MEBBE:" 
				<< std::endl << DebugIndent( 2 ) << "Condition:"
				<< std::endl << *(mebbeBlockIt->first) 
				<< std::endl << DebugIndent( 2 ) << "Body:"
				<< std::endl << *(mebbeBlockIt->second);
		}
		
		if( noWaiBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "NO WAI:" 
				<< std::endl << DebugIndent( 2 ) << "Body:"
				<< std::endl << *noWaiBlock_;
		}
	}
	
	void ORlyBlock::AddMebbeBlock( Expression *mebbeCond, 
								   StatementBlock *mebbeBody ) {
		assert( mebbeCond != nullptr );
		assert( mebbeBody != nullptr );
		mebbeBlocks_.push_back( MebbeBlock( mebbeCond, mebbeBody ) );
		mebbeCond->SetParent( this );
		mebbeBody->SetParent( this );
	}
	
	void ORlyBlock::SetNoWaiBlock( StatementBlock *noWaiBlock ) {
		assert( noWaiBlock != nullptr );
		noWaiBlock_ = noWaiBlock;
		noWaiBlock_->SetParent( this );
	}
	
	WtfBlock::~WtfBlock() {
		while( !omgBlocks_.empty() ) {
			delete omgBlocks_.back().first;
			delete omgBlocks_.back().second;
			omgBlocks_.pop_back();
		}
		
		delete omgwtfBlock_;
	}
	
	void WtfBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "WTF:";
		
		for( OmgBlockListIterator omgBlockIt = omgBlocks_.begin(); 
			 omgBlockIt != omgBlocks_.end(); ++omgBlockIt ) {
			out << std::endl << DebugIndent( 1 ) << "OMG:" 
				<< std::endl << DebugIndent( 2 ) << "Label:"
				<< std::endl << *(omgBlockIt->first) 
				<< std::endl << DebugIndent( 2 ) << "Body:"
				<< std::endl << *(omgBlockIt->second);
		}
		
		if( omgwtfBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "OMGWTF:"
				<< std::endl << DebugIndent( 2 ) << "Body:"
				<< std::endl << *omgwtfBlock_;
		}
	}
	
	void WtfBlock::AddOmgBlock( Literal *omgLabel, StatementBlock *omgBody ) {
		assert( omgLabel != nullptr );
		assert( omgBody != nullptr );
		omgBlocks_.push_back( OmgBlock( omgLabel, omgBody ) );
		omgLabel->SetParent( this );
		omgBody->SetParent( this );
	}
	
	void WtfBlock::SetOmgwtfBlock( StatementBlock *omgwtfBlock ) {
		assert( omgwtfBlock != nullptr );
		omgwtfBlock_ = omgwtfBlock;
		omgwtfBlock_->SetParent( this );
	}
	
	LoopBlock::LoopBlock( LiteralIdentifier *label ) : label_( label ), 
			body_( nullptr ), loopVar_( nullptr ), loopVarIsLocal_( false ) {
		assert( label_ != nullptr );
		label_->SetParent( this );
	}
	
	LoopBlock::~LoopBlock() {
		delete label_;
		delete body_;
		delete loopVar_;
	}
	
	void LoopBlock::Print( std::ostream &out ) {
		out << DebugIndent() << GetLoopType() << ":" << std::endl 
			<< DebugIndent( 1 ) << "Label: " << label_->GetIdentifier();
		
		if( loopVar_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) 
				<< ( loopVarIsLocal_ ? "Local " : "" ) << "Loop Var:" 
				<< std::endl << *loopVar_;
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
		loopVar_->SetParent( this );
		loopVarIsLocal_ = isLocal;
	}
	
	ForLoopBlock::ForLoopBlock( LiteralIdentifier *label ) : LoopBlock( label ), 
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
	
	RangeLoopBlock::RangeLoopBlock( LiteralIdentifier *label ) : 
		LoopBlock( label ), bukkitRef_( nullptr ) {}
	
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
	
	FunkshunBlock::FunkshunBlock( LiteralIdentifier *name ) : 
			name_( name ), body_( nullptr ) {
		assert( name_ != nullptr );
		name_->SetParent( this );
	}
	
	FunkshunBlock::~FunkshunBlock() {
		delete name_;
		
		while( !params_.empty() ) {
			delete params_.back().first;
			params_.pop_back();
		}
		
		delete body_;
	}
	
	void FunkshunBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "FUNKSHUN: " << name_->GetIdentifier();
		
		if( !params_.empty() ) {
			for( FunkshunParamListIterator paramIt = params_.begin();
				 paramIt != params_.end(); ++paramIt ) {
				out << std::endl << DebugIndent( 1 ) << "Parameter:"
					<< std::endl << *(paramIt->first);
				if( paramIt->second ) {
					out << std::endl << DebugIndent( 2 ) << "isPassByRef";
				}
			}
		}
		
		if( body_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Body:" << std::endl << *body_;
		}
	}
	
	void FunkshunBlock::AddParameter( LiteralIdentifier *param, bool isPassByRef ) {
		assert( param != nullptr );
		params_.push_back( FunkshunParam( param, isPassByRef ) );
		param->SetParent( this );
	}
	
	void FunkshunBlock::SetBody( StatementBlock *body ) {
		assert( body != nullptr );
		body_ = body;
		body_->SetParent( this );
	}
	
	FunkshunDeclare::FunkshunDeclare( LiteralIdentifier *funkshunName ) : 
			funkshunName_( funkshunName ) {
		assert( funkshunName_ != nullptr );
		funkshunName_->SetParent( this );
	}
	
	FunkshunDeclare::~FunkshunDeclare() {
		delete funkshunName_;
	}
	
	void FunkshunDeclare::Print( std::ostream &out ) {
		out << DebugIndent() << "FUNKSHUN DECLARE: " << funkshunName_->GetIdentifier();
	}
	
	PlzBlock::PlzBlock( StatementBlock *tryBlock ) : 
			tryBlock_( tryBlock ), welBlock_( nullptr ) {
		assert( tryBlock_ != nullptr );
		tryBlock_->SetParent( this );
	}
	
	PlzBlock::~PlzBlock() {
		delete tryBlock_;
		
		while( !noesBlocks_.empty() ) {
			delete noesBlocks_.back().first;
			delete noesBlocks_.back().second;
			noesBlocks_.pop_back();
		}
		
		delete welBlock_;
	}
	
	void PlzBlock::Print( std::ostream &out ) {
		out << DebugIndent() << "PLZ:";
		
		if( tryBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Try:" 
				<< std::endl << DebugIndent( 2 ) << "Body:" 
				<< std::endl << *tryBlock_;
		}
		
		for( NoesBlockListIterator noesBlockIt = noesBlocks_.begin();
			 noesBlockIt != noesBlocks_.end(); ++noesBlockIt ) {
			out << std::endl << DebugIndent( 1 ) << "NOES:" 
				<< std::endl << DebugIndent( 2 ) << "Exception:"
				<< std::endl << *(noesBlockIt->first) 
				<< std::endl << DebugIndent( 2 ) << "Handler:"
				<< std::endl << *(noesBlockIt->second);
		}
		
		if( welBlock_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "WEL:" 
				<< std::endl << DebugIndent( 2 ) << "Body:"
				<< std::endl << *welBlock_;
		}
	}
	
	void PlzBlock::AddNoesBlock( Expression *exception, StatementBlock *handler ) {
		assert( exception != nullptr );
		assert( handler != nullptr );
		noesBlocks_.push_back( NoesBlock( exception, handler ) );
		exception->SetParent( this );
		handler->SetParent( this );
	}
	
	void PlzBlock::SetWelBlock( StatementBlock *welBlock ) {
		assert( welBlock != nullptr );
		welBlock_ = welBlock;
		welBlock_->SetParent( this );
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
	
	VisibleStatement::VisibleStatement( Expression *expr ) : 
			suppressNewline_( false ) {
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
		
		for( ExpressionListIterator it = exprs_.begin();
			 it != exprs_.end(); ++it ) {
			Expression *expr = *it;
			assert( expr != nullptr );
			out << std::endl << *expr;
		}
		
		if( !suppressNewline_ ) {
			out << std::endl << DebugIndent( 1 ) << "NEWLINE";
		}
	}
	
	void VisibleStatement::AddExpression( Expression *expr ) {
		assert( expr != nullptr );
		exprs_.push_back( expr );
		expr->SetParent( this );
	}
	
	GimmehStatement::GimmehStatement( Identifier *targetVar, bool isLong ) :
			targetVar_( targetVar ), isLong_( isLong ) {
		assert( targetVar_ != nullptr );
		targetVar_->SetParent( this );
	}
	
	GimmehStatement::~GimmehStatement() {
		delete targetVar_;
	}
	
	void GimmehStatement::Print( std::ostream &out ) {
		out << DebugIndent() << "GIMMEH";
		if( isLong_ ) {
			out << " LONG";
		}
		out << ":";
		
		if( targetVar_ != nullptr ) {
			out << std::endl << *targetVar_;
		}
	}
	
	Program::Program( StatementBlock *body ) : 
			globals_( new StatementBlock( 
				AST::StatementBlock::Type::PROGRAM_GLOBALS ) ), 
			body_( body ) {
		assert( body_ != nullptr );
		body_->SetParent( this );
		globals_->SetParent( this );
	}
	
	void Program::Print( std::ostream &out ) {
		out << DebugIndent() << "PROGRAM:";
		
		assert( globals_ != nullptr );
		out << std::endl << DebugIndent( 1 ) << "Globals:" << std::endl << *globals_;
		
		if( body_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Body:" << std::endl << *body_;
		}
	}
}