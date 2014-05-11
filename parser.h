#ifndef PARSER_H
#define PARSER_H

#include "tokenlist.h"
#include "token.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"

/*************************************************************************************
 Program := ProgramBegin ProgramStatementBlock ProgramEnd
 ProgramBegin := 'HAI' [VersionNumber] LineDelimiter
 ProgramEnd := 'KTHXBYE' LineDelimiter*
 VersionNumber := '1.2'
 ProgramStatementBlock := (Statement | ORlyBlock | WtfBlock | 
 						  LoopBlock | FunkshunBlock | PlzBlock)*
 StatementBlock := (Statement | ORlyBlock | WtfBlock | LoopBlock | PlzBlock)*
 Statement := [VarDeclare | VarAssign | VarCast | FunkshunReturn | 
 			  VisibleStmt | GimmehStmt | 'GTFO' | Expression] LineDelimiter
 FunkshunBlock := FunkshunDeclare StatementBlock FunkshunEnd
 FunkshunDeclare := 'HOW' 'IZ' 'I' LiteralIdentifier 
 				    [('YR' | 'DAT') LiteralIdentifier 
 				    ('AN' ('YR' | 'DAT') LiteralIdentifier)*] LineDelimiter
 FunkshunEnd := 'IF' 'U' 'SAY' 'SO' LineDelimiter
 FunkshunReturn := 'FOUND' 'YR' Expression
 FunkshunCall := 'I' 'IZ' LiteralIdentifier ['YR' Expression 
				 ('AN' 'YR' Expression)*] ('MKAY' | LineDelimiter)
 ORlyBlock := ORlyStmt YaStmt StatementBlock (MeebeStmt StatementBlock)* 
 			  [NoStmt StatementBlock] EndConditionalBlock
 ORlyStmt := 'O' 'RLY' '?' LineDelimiter
 YaStmt := 'YA' 'RLY' LineDelimiter
 MebbeStmt := 'MEBBE' Expression LineDelimiter
 NoStmt := 'NO' 'WAI' LineDelimiter
 EndConditionalBlock := 'OIC' LineDelimiter
 WtfBlock := WtfStmt OmgBlock+ [OmgwtfBlock] EndConditionalBlock
 WtfStmt := 'WTF' '?' LineDelimiter
 OmgBlock := 'OMG' Literal LineDelimiter StatementBlock
 OmgwtfBlock := 'OMGWTF' LineDelimiter StatementBlock
 LoopBlock := LoopBegin StatementBlock LoopEnd
 LoopBegin := 'IM' 'IN' 'YR' LiteralIdentifier 
 			  [ForLoopInit | RangeLoopInit] LineDelimiter
 ForLoopInit := ('UPPIN' | 'NERFIN' | 'WITH' | UnaryOperator | LiteralIdentifier) 
				['YR'] LoopVarIdentifier ['FROM' Expression] 
				[('TIL' | 'WILE') Expression]
 RangeLoopInit := 'WATCHIN' ['YR'] LoopVarIdentifier 'FROM' BukkitRef
 LoopEnd := 'IM' 'OUTTA' 'YR' LiteralIdentifier LineDelimiter
 PlzBlock := PlzStmt StatementBlock ONoesBlock* [OWellBlock] KThxStmt
 PlzStmt := 'PLZ' LineDelimiter
 ONoesBlock := 'O' 'NOES' Expression LineDelimiter StatementBlock
 OWellBlock := 'O' 'WELL' LineDelimiter StatementBlock
 KThxStmt := 'KTHX' LineDelimiter
 VarDeclare := 'I' 'HAS' 'A' ExplicitIdentifier ['ITZ' (Expression | 'A' Type)]
 Type := 'NOOB' | 'TROOF' | 'NUMBR' | 'NUMBAR' | 'YARN' | 'BUKKIT'
 VarAssign := ExplicitIdentifier 'R' Expression
 VarCast := ExplicitIdentifier 'IS' 'NOW' 'A' Type
 VisibleStmt : = 'VISIBLE' Expression (Expression)* ['!']
 GimmehStmt := 'GIMMEH' ['LONG'] ExplicitIdentifier
 Expression := UnaryExpr | BinaryExpr | NaryExpr | Identifier | 
 			   Literal | FunkshunCall | CastExpr
 BukkitRef := Identifier | FunkshunCall | CastExpr
 FilezRef := Identifier | FunkshunCall | CastExpr
 CastExpr := 'MAEK' Expression ['A'] Type
 UnaryExpr := UnaryOperator Expression
 BinaryExpr := BinaryOperator Expression ['AN'] Expression
 NaryExpr := NaryOperator Expression (['AN'] Expression)* ('MKAY' | LineDelimiter)
 Identifier := LiteralIdentifier | SrsIdentifier | ItIdentifier | SlotIdentifier
 ExplicitIdentifier := LiteralIdentifier | SrsIdentifier | SlotIdentifier
 LoopVarIdentifier := LiteralIdentifier | SrsIdentifier
 LiteralIdentifier := ('A'-'Z' | 'a'-'z') . ('A'-'Z' | 'a'-'z' | '0'-'9' | '_')*
 SrsIdentifier := 'SRS' Expression
 ItIdentifier := 'IT'
 SlotIdentifier := 'SLOT' Expression 'IN' BukkitRef
 Literal := TroofLiteral | NumbrLiteral | NumbarLiteral | YarnLiteral | NoobLiteral
 TroofLiteral := 'WIN' | 'FAIL'
 NumbrLiteral := ['-'] ('0'-'9')+
 NumbarLiteral := ['-'] (('0'-'9')+ . '.' . ('0'-'9')* | 
				  ('0'-'9')* . '.' . ('0'-'9')+)
 YarnLiteral := '"' (<character>)* '"'
 NoobLiteral := 'NOOB'
 LineDelimiter := ',' | '\n'
 UnaryOperator := 'NOT'
 BinaryOperator := ('SUM' 'OF' | 'DIFF' 'OF' | 'PRODUKT' 'OF' | 'QUOSHUNT' 'OF' | 
 				   'MOD' 'OF' | 'BIGGR' 'OF' | 'SMALLR' 'OF' | 'BOTH' 'OF' | 
 				   'EITHER' 'OF' | 'WON' 'OF' | 'BOTH' 'SAEM' | 'DIFFRINT')
 NaryOperator := 'ALL' 'OF' | 'ANY' 'OF' | 'SMOOSH'
 
 ======================================= TODO =======================================
 - Objects ("TING")
 - Indirection function call through variables (map YARN to functions?)

 ==== File I/O ====
 VisibleStmt : = 'VISIBLE' Expression (Expression)* ['!'] ['TO' FilezRef]
 GimmehStmt := 'GIMMEH' ['LONG'] ExplicitIdentifier ['FROM' FilezRef]
 Type := 'NOOB' | 'TROOF' | 'NUMBR' | 'NUMBAR' | 'YARN' | 'BUKKIT' | 'FILEZ'
 OpenFilezStmt := 'OPENIN' 'FILEZ' Expression ['FOR' ('GIMMEH' | 'VISIBLE' 
				  ['AT' 'BYE'])] 'AS' ExplicitIdentifier
 CloseFilezStmt := 'CLOSIN' 'FILEZ' FilezRef

 ==== Global variables ====
 VarDeclare := 'I' 'HAS' 'A' ['SHARD'] ExplicitIdentifier 
			   ['ITZ' (Expression | 'A' Type)]
*************************************************************************************/

class Parser {
	private:
		enum class LolcodeVersion { V12 };
		
	public:
		Parser() : tokens_( nullptr ) {}

		AST::Program *Parse( TokenList * );

	private:
		enum class StatementBlockType { 
			PROGRAM_BODY,
			PROGRAM_GLOBALS,
			FUNKSHUN_BODY,
			ORLY_YA,
			ORLY_MEBBE,
			ORLY_NO,
			WTF_OMG,
			WTF_OMGWTF,
			LOOP_BODY,
			PLZ_BODY,
			PLZ_ONOES,
			PLZ_OWEL
		};
			
		AST::StatementBlock *ParseStatementBlock( const StatementBlockType );
		AST::StatementBlock *ParseProgramBody() {
			return ParseStatementBlock( StatementBlockType::PROGRAM_BODY );
		}
		AST::StatementBlock *ParseProgramGlobals() {
			return ParseStatementBlock( StatementBlockType::PROGRAM_GLOBALS );
		}
		AST::StatementBlock *ParseFunkshunBody();
		AST::StatementBlock *ParseORlyYaBody() {
			return ParseStatementBlock( StatementBlockType::ORLY_YA );
		}
		AST::StatementBlock *ParseORlyMebbeBody() {
			return ParseStatementBlock( StatementBlockType::ORLY_MEBBE );
		}
		AST::StatementBlock *ParseORlyNoBody() {
			return ParseStatementBlock( StatementBlockType::ORLY_NO );
		}
		AST::StatementBlock *ParseWtfOmgBody() {
			return ParseStatementBlock( StatementBlockType::WTF_OMG );
		}
		AST::StatementBlock *ParseWtfOmgwtfBody() {
			return ParseStatementBlock( StatementBlockType::WTF_OMGWTF );
		}
		AST::StatementBlock *ParseLoopBody() {
			return ParseStatementBlock( StatementBlockType::LOOP_BODY );
		}
		AST::StatementBlock *ParsePlzBody() {
			return ParseStatementBlock( StatementBlockType::PLZ_BODY );
		}
		AST::StatementBlock *ParsePlzONoesBody() {
			return ParseStatementBlock( StatementBlockType::PLZ_ONOES );
		}
		AST::StatementBlock *ParsePlzOWelBody() {
			return ParseStatementBlock( StatementBlockType::PLZ_OWEL );
		}

		AST::ORlyBlock *ParseORlyBlock();
		AST::WtfBlock *ParseWtfBlock();
		AST::LoopBlock *ParseLoopBlock();
		AST::FunkshunBlock *ParseFunkshunBlock();
		AST::PlzBlock *ParsePlzBlock();
		AST::Statement *ParseStatement();

		AST::VarDeclare *ParseVarDeclare();
		AST::VarAssign *ParseVarAssign( AST::Identifier * );
		AST::VarAssign *ParseVarCast( AST::Identifier * );
		AST::VisibleStatement *ParseVisibleStatement();
		AST::GimmehStatement *ParseGimmehStatement();
		AST::FunkshunReturn *ParseFunkshunReturn();
		
		AST::Expression *ParseExpression();
		AST::FunkshunCall *ParseFunkshunCall();
		AST::MaekExpression *ParseMaekExpression();
		AST::UnaryExpression *ParseUnaryOperator();
		AST::BinaryExpression *ParseBinaryOperator();
		AST::NaryExpression *ParseNaryOperator();
		AST::Expression *ParseBukkitReference();
		
		AST::Literal *ParseLiteral();
		AST::TroofLiteral *ParseTroofLiteral();
		AST::NumbrLiteral *ParseNumbrLiteral();
		AST::NumbarLiteral *ParseNumbarLiteral();
		AST::YarnLiteral *ParseYarnLiteral();
		
		AST::Identifier *ParseIdentifier();
		AST::Identifier *ParseExplicitIdentifier();
		AST::Identifier *ParseLoopVarIdentifier();
		AST::LiteralIdentifier *ParseLiteralIdentifier();
		AST::SrsIdentifier *ParseSrsIdentifier();
		AST::ItIdentifier *ParseItIdentifier();
		AST::SlotIdentifier *ParseSlotIdentifier();
		AST::TypeIdentifier *ParseTypeIdentifier();
		
		bool AcceptToken( TokenType, Token * = nullptr );
	
	private:
		TokenList *tokens_;
		bool lineGood_;
		
		LolcodeVersion lolcodeVersion_;
		
		AST::FunkshunList funkshuns_;
};

#endif

