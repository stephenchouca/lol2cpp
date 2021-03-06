#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum class TokenType {
	YARN,
	NUMBR,
	NUMBAR,
	TROOF,
	BUKKIT,
	NOOB,
	YARN_LITERAL,
	NUMBR_LITERAL,
	NUMBAR_LITERAL,
	MINUS_SIGN,
	WIN,
	FAIL,
	EXCLAMATION,
	IDENTIFIER,
	BTW,
	I,
	HAS,
	A,
	ITZ,
	OBTW,
	TLDR,
	HAI,
	KTHXBYE,
	R,
	MKAY,
	AN,
	SUM,
	DIFF,
	PRODUKT,
	QUOSHUNT,
	MOD,
	BIGGR,
	SMALLR,
	OF,
	BOTH,
	EITHER,
	WON,
	NOT,
	ALL,
	ANY,
	SAEM,
	DIFFRINT,
	IT,
	SMOOSH,
	MAEK,
	IS,
	NOW,
	VISIBLE,
	GIMMEH,
	LONG,
	O,
	RLY,
	QUESTION,
	YA,
	NO,
	WAI,
	OIC,
	MEBBE,
	WTF,
	OMG,
	GTFO,
	WHATEVR,
	OMGWTF,
	IM,
	IN,
	OUTTA,
	YR,
	FROM,
	TIL,
	WILE,
	UPPIN,
	NERFIN,
	WITH,
	WATCHIN,
	HOW,
	IZ,
	DAT,
	IF,
	U,
	SAY,
	SO,
	FOUND,
	PLZ,
	NOES,
	WEL,
	KTHX,
	SRS,
	SLOT,
	LINE_DELIMITER,
	END_OF_FILE
};

std::string TokenTypeToString( TokenType );

struct Token {
	TokenType type;
	std::string string;

	friend std::ostream &operator <<( std::ostream &, Token );
};

#endif
