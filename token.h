#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum class TokenType {
	YARN,
	NUMBR,
	NUMBAR,
	TROOF,
	NOOB,
	YARN_LITERAL,
	NUMBR_LITERAL,
	NUMBAR_LITERAL,
	NEGATIVE,
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
	O,
	RLY,
	YA,
	NO,
	WAI,
	OIC,
	MEEBE,
	WTF,
	OMG,
	GTFO,
	OMGWTF,
	IM,
	IN,
	OUTTA,
	YR,
	TIL,
	WILE,
	UPPIN,
	NERFIN,
	HOW,
	IZ,
	IF,
	U,
	SAY,
	SO,
	FOUND,
	LINE_DELIMITER,
	END_OF_FILE
};

std::string TokenTypeToString(TokenType);

struct Token {
	TokenType type;
	std::string string;

	friend std::ostream &operator <<( std::ostream &, Token & );
};

#endif
