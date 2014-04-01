#include <cassert>
#include <cstdlib>

#include "expression.h"
#include "statement.h"

namespace AST {
	NumbrLiteral::NumbrLiteral( std::string &valStr ) {
		char *end;
		val_ = std::strtol( valStr.c_str(), &end, 10 );
		assert( *end == '\0' );
	}
	
	NumbarLiteral::NumbarLiteral( std::string &valStr ) {
		char *end;
		val_ = std::strtod( valStr.c_str(), &end );
		assert( *end == '\0' );
	}
	
	void TypeIdentifier::Print( std::ostream &out ) {
		out << DebugIndent() << "TYPE: ";
		switch( type_ ) {
			case Type::NUMBR:
				out << "NUMBR";
				break;
			case Type::NUMBAR:
				out << "NUMBAR";
				break;
			case Type::TROOF:
				out << "TROOF";
				break;
			case Type::YARN:
				out << "YARN";
				break;
			case Type::BUKKIT:
				out << "BUKKIT";
				break;
			case Type::NOOB:
				out << "NOOB";
				break;
			default:
				break;
		}
	}
	
	SrsIdentifier::SrsIdentifier( Expression *ref ) : ref_( ref ) {
		assert( ref_ != nullptr );
		ref_->SetParent( this );
	}
	
	void SrsIdentifier::Print( std::ostream &out ) {
		out << DebugIndent() << "SRS:";
		if( ref_ != nullptr ) {
			out << std::endl << *ref_;
		}
	}
	
	SlotIdentifier::SlotIdentifier( Expression *key ) : 
			key_( key ), bukkitRef_( nullptr ) {
		assert( key_ != nullptr );
		key_->SetParent( this );
	}
	
	void SlotIdentifier::Print( std::ostream &out ) {
		out << DebugIndent() << "SLOT:";
		if( key_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Key:" << std::endl << *key_;
		}
		if( bukkitRef_ != nullptr ) {
			out << std::endl << DebugIndent( 1 ) << "Bukkit Ref:" 
				<< std::endl << *bukkitRef_;
		}
	}
	
	SlotIdentifier *SlotIdentifier::Clone() {
		SlotIdentifier *ret = new SlotIdentifier( key_->Clone() );
		ret->bukkitRef_ = this->bukkitRef_->Clone();
		return ret;
	}
	
	void SlotIdentifier::SetBukkitReference( Expression *ref ) {
		assert( ref != nullptr );
		assert( bukkitRef_ == nullptr );
		bukkitRef_ = ref;
		bukkitRef_->SetParent( this );
	}
	
	void UnaryExpression::Print( std::ostream &out ) {
		out << DebugIndent() << GetOperatorName() << ":";
		if( operand_ != nullptr ) {
			out << std::endl << *operand_;
		}
	}

	void UnaryExpression::CloneOperand( UnaryExpression *newExpr ) {
		assert( newExpr != nullptr );
		newExpr->operand_ = ( this->operand_ == nullptr ) ? 
							nullptr : this->operand_->Clone();
	}
	
	void UnaryExpression::SetOperand( Expression *operand ) {
		assert( operand != nullptr );
		operand_ = operand;
		operand_->SetParent( this );
	}
	
	NotExpression *NotExpression::Clone() {
		NotExpression *ret = new NotExpression();
		CloneOperand( ret );
		return ret;
	}
	
	UppinExpression *UppinExpression::Clone() {
		UppinExpression *ret = new UppinExpression();
		CloneOperand( ret );
		return ret;
	}

	NerfinExpression *NerfinExpression::Clone() {
		NerfinExpression *ret = new NerfinExpression();
		CloneOperand( ret );
		return ret;
	}

	TilExpression *TilExpression::Clone() {
		TilExpression *ret = new TilExpression();
		CloneOperand( ret );
		return ret;
	}

	WileExpression *WileExpression::Clone() {
		WileExpression *ret = new WileExpression();
		CloneOperand( ret );
		return ret;
	}
	
	void BinaryExpression::Print( std::ostream &out ) {
		out << DebugIndent() << GetOperatorName() << ":";
		if( leftOperand_ != nullptr ) {
			out << std::endl << *leftOperand_;
		}
		if( rightOperand_ != nullptr ) {
			out << std::endl << *rightOperand_;
		}
	}
	
	void BinaryExpression::CloneOperands( BinaryExpression *newExpr ) {
		assert( newExpr != nullptr );
		newExpr->leftOperand_ = ( this->leftOperand_ == nullptr ) ? 
								nullptr : this->leftOperand_->Clone();
		newExpr->rightOperand_ = ( this->rightOperand_ == nullptr ) ? 
								 nullptr : this->rightOperand_->Clone();
	}
	
	void BinaryExpression::SetLeftOperand( Expression *leftOperand ) {
		assert( leftOperand != nullptr );
		leftOperand_ = leftOperand;
		leftOperand_->SetParent( this );
	}
	
	void BinaryExpression::SetRightOperand( Expression *rightOperand ) {
		assert( rightOperand != nullptr );
		rightOperand_ = rightOperand;
		rightOperand_->SetParent( this );
	}
	
	SumExpression *SumExpression::Clone() {
		SumExpression *ret = new SumExpression();
		CloneOperands( ret );
		return ret;
	}

	DiffExpression *DiffExpression::Clone() {
		DiffExpression *ret = new DiffExpression();
		CloneOperands( ret );
		return ret;
	}

	ProduktExpression *ProduktExpression::Clone() {
		ProduktExpression *ret = new ProduktExpression();
		CloneOperands( ret );
		return ret;
	}

	QuoshuntExpression *QuoshuntExpression::Clone() {
		QuoshuntExpression *ret = new QuoshuntExpression();
		CloneOperands( ret );
		return ret;
	}

	ModExpression *ModExpression::Clone() {
		ModExpression *ret = new ModExpression();
		CloneOperands( ret );
		return ret;
	}

	BiggrExpression *BiggrExpression::Clone() {
		BiggrExpression *ret = new BiggrExpression();
		CloneOperands( ret );
		return ret;
	}

	SmallrExpression *SmallrExpression::Clone() {
		SmallrExpression *ret = new SmallrExpression();
		CloneOperands( ret );
		return ret;
	}

	BothExpression *BothExpression::Clone() {
		BothExpression *ret = new BothExpression();
		CloneOperands( ret );
		return ret;
	}

	EitherExpression *EitherExpression::Clone() {
		EitherExpression *ret = new EitherExpression();
		CloneOperands( ret );
		return ret;
	}

	WonExpression *WonExpression::Clone() {
		WonExpression *ret = new WonExpression();
		CloneOperands( ret );
		return ret;
	}

	SaemExpression *SaemExpression::Clone() {
		SaemExpression *ret = new SaemExpression();
		CloneOperands( ret );
		return ret;
	}
	
	DiffrintExpression *DiffrintExpression::Clone() {
		DiffrintExpression *ret = new DiffrintExpression();
		CloneOperands( ret );
		return ret;
	}
	
	NaryExpression::~NaryExpression() {
		while( !operands_.empty() ) {
			delete operands_.back();
			operands_.pop_back();
		}
	}
	
	void NaryExpression::Print( std::ostream &out ) {
		out << DebugIndent() << GetOperatorName() << ":";
		
		ExpressionListIterator operandIt;
		for( operandIt = operands_.cbegin(); 
			 operandIt != operands_.cend(); ++operandIt ) {
			Expression *operand = *operandIt;
			out << std::endl << *operand;
		}
	}
	
	void NaryExpression::CloneOperands( NaryExpression *newExpr ) {
		assert( newExpr != nullptr );
		newExpr->operands_ = this->operands_;
	}
	
	AllExpression *AllExpression::Clone() {
		AllExpression *ret = new AllExpression();
		CloneOperands( ret );
		return ret;
	}
	
	AnyExpression *AnyExpression::Clone() {
		AnyExpression *ret = new AnyExpression();
		CloneOperands( ret );
		return ret;
	}
	
	SmooshExpression *SmooshExpression::Clone() {
		SmooshExpression *ret = new SmooshExpression();
		CloneOperands( ret );
		return ret;
	}
	
	FunkshunCall *FunkshunCall::Clone() {
		FunkshunCall *ret = new FunkshunCall( funkshunName_->Clone() );
		CloneOperands( ret );
		return ret;
	}
	
	void NaryExpression::AddOperand( Expression *operand ) {
		assert( operand != nullptr );
		operands_.push_back( operand );
		operand->SetParent( this );
	}
	
	FunkshunCall::FunkshunCall( LiteralIdentifier *funkshunName ) : 
			funkshunName_( funkshunName ) {
		assert( funkshunName_ != nullptr );
		funkshunName_->SetParent( this );
	}
	
	CastExpression::CastExpression( Expression *srcExpr ) :
			srcExpr_( srcExpr ), targetType_( nullptr ) {
		assert( srcExpr_ != nullptr );
		srcExpr_->SetParent( this );
	}
	
	void CastExpression::Print( std::ostream &out ) {
		out << DebugIndent() << "CAST:";
		if( srcExpr_ != nullptr ) {
			out << *srcExpr_ << std::endl;
		}
		if( targetType_ != nullptr ) {
			out << *targetType_;
		}
	}
	
	CastExpression *CastExpression::Clone() {
		CastExpression *ret = new CastExpression( srcExpr_->Clone() );
		ret->targetType_ = this->targetType_->Clone();
		return ret;
	}
	
	void CastExpression::SetCastTargetType( TypeIdentifier *targetType ) {
		assert( targetType != nullptr );
		targetType_ = targetType;
		targetType_->SetParent( this );
	}
}