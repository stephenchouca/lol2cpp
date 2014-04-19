#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

#include "token.h"
#include "tokenizer.h"

Tokenizer::Tokenizer() :
	inMultiLineComment_( false ), 
	continueCurrentLine_( false ),
	addLineDelimiter_( false ) {}

Tokenizer::CharType Tokenizer::GetCharType( char ch ) {
	if( ( ch >= 'A' && ch <= 'Z' ) || ( ch >= 'a' && ch <= 'z' ) ) {
		return CharType::LETTER;
	}

	if( ch >= '0' && ch <= '9' ) {
		return CharType::DIGIT;
	}

	switch( ch ) {
		case '.':
			return CharType::PERIOD;
		case '"':
			return CharType::DOUBLE_QUOTE;
		case '!':
			return CharType::EXCLAMATION;
		case '?':
			return CharType::QUESTION;
		case ' ':
		case '\t':
			return CharType::SPACE;
		case ':':
			return CharType::YARN_ESCAPE;
		case ')':
			return CharType::YARN_NEWLINE;
		case '>':
			return CharType::YARN_TAB;
		case ',':
			return CharType::LINE_DELIMITER;
		case '-':
			return CharType::MINUS;
		default:
			break;
	}

	return CharType::UNKNOWN;
}

TokenType Tokenizer::GetTokenType( std::string str ) {
	if( str == "YARN" ) {
		return TokenType::YARN;
	} else if( str == "NUMBR" ) {
		return TokenType::NUMBR;
	} else if( str == "NUMBAR" ) {
		return TokenType::NUMBAR;
	} else if( str == "TROOF" ) {
		return TokenType::TROOF;
	} else if( str == "BUKKIT" ) {
		return TokenType::BUKKIT;
	} else if( str == "NOOB" ) {
		return TokenType::NOOB;
	} else if( str == "WIN" ) {
		return TokenType::WIN;
	} else if( str == "FAIL" ) {
		return TokenType::FAIL;
	} else if( str == "BTW" ) {
		return TokenType::BTW;
	} else if( str == "I" ) {
		return TokenType::I;
	} else if( str == "HAS" ) {
		return TokenType::HAS;
	} else if( str == "A" ) {
		return TokenType::A;
	} else if( str == "ITZ" ) {
		return TokenType::ITZ;
	} else if( str == "OBTW" ) {
		return TokenType::OBTW;
	} else if( str == "TLDR" ) {
		return TokenType::TLDR;
	} else if( str == "HAI" ) {
		return TokenType::HAI;
	} else if( str == "KTHXBYE" ) {
		return TokenType::KTHXBYE;
	} else if( str == "R" ) {
		return TokenType::R;
	} else if( str == "MKAY" ) {
		return TokenType::MKAY;
	} else if( str == "AN" ) {
		return TokenType::AN;
	} else if( str == "SUM" ) {
		return TokenType::SUM;
	} else if( str == "DIFF" ) {
		return TokenType::DIFF;
	} else if( str == "PRODUKT" ) {
		return TokenType::PRODUKT;
	} else if( str == "QUOSHUNT" ) {
		return TokenType::QUOSHUNT;
	} else if( str == "MOD" ) {
		return TokenType::MOD;
	} else if( str == "BIGGR" ) {
		return TokenType::BIGGR;
	} else if( str == "SMALLR" ) {
		return TokenType::SMALLR;
	} else if( str == "OF" ) {
		return TokenType::OF;
	} else if( str == "BOTH" ) {
		return TokenType::BOTH;
	} else if( str == "EITHER" ) {
		return TokenType::EITHER;
	} else if( str == "WON" ) {
		return TokenType::WON;
	} else if( str == "NOT" ) {
		return TokenType::NOT;
	} else if( str == "ALL" ) {
		return TokenType::ALL;
	} else if( str == "ANY" ) {
		return TokenType::ANY;
	} else if( str == "SAEM" ) {
		return TokenType::SAEM;
	} else if( str == "DIFFRINT" ) {
		return TokenType::DIFFRINT;
	} else if( str == "IT" ) {
		return TokenType::IT;
	} else if( str == "SMOOSH" ) {
		return TokenType::SMOOSH;
	} else if( str == "MAEK" ) {
		return TokenType::MAEK;
	} else if( str == "IS" ) {
		return TokenType::IS;
	} else if( str == "NOW" ) {
		return TokenType::NOW;
	} else if( str == "VISIBLE" ) {
		return TokenType::VISIBLE;
	} else if( str == "GIMMEH" ) {
		return TokenType::GIMMEH;
	} else if( str == "LONG" ) {
		return TokenType::LONG;
	} else if( str == "O" ) {
		return TokenType::O;
	} else if( str == "RLY" ) {
		return TokenType::RLY;
	} else if( str == "YA" ) {
		return TokenType::YA;
	} else if( str == "NO" ) {
		return TokenType::NO;
	} else if( str == "WAI" ) {
		return TokenType::WAI;
	} else if( str == "OIC" ) {
		return TokenType::OIC;
	} else if( str == "MEBBE" ) {
		return TokenType::MEBBE;
	} else if( str == "WTF" ) {
		return TokenType::WTF;
	} else if( str == "OMG" ) {
		return TokenType::OMG;
	} else if( str == "GTFO" ) {
		return TokenType::GTFO;
	} else if( str == "OMGWTF" ) {
		return TokenType::OMGWTF;
	} else if( str == "IM" ) {
		return TokenType::IM;
	} else if( str == "IN" ) {
		return TokenType::IN;
	} else if( str == "OUTTA" ) {
		return TokenType::OUTTA;
	} else if( str == "YR" ) {
		return TokenType::YR;
	} else if( str == "FROM" ) {
		return TokenType::FROM;
	} else if( str == "TIL" ) {
		return TokenType::TIL;
	} else if( str == "WILE" ) {
		return TokenType::WILE;
	} else if( str == "UPPIN" ) {
		return TokenType::UPPIN;
	} else if( str == "NERFIN" ) {
		return TokenType::NERFIN;
	} else if( str == "WITH" ) {
		return TokenType::WITH;
	} else if( str == "WATCHIN" ) {
		return TokenType::WATCHIN;
	} else if( str == "HOW" ) {
		return TokenType::HOW;
	} else if( str == "IZ" ) {
		return TokenType::IZ;
	} else if( str == "DAT" ) {
		return TokenType::DAT;
	} else if( str == "IF" ) {
		return TokenType::IF;
	} else if( str == "U" ) {
		return TokenType::U;
	} else if( str == "SAY" ) {
		return TokenType::SAY;
	} else if( str == "SO" ) {
		return TokenType::SO;
	} else if( str == "FOUND" ) {
		return TokenType::FOUND;
	} else if( str == "PLZ" ) {
		return TokenType::PLZ;
	} else if( str == "NOES" ) {
		return TokenType::NOES;
	} else if( str == "WEL" ) {
		return TokenType::WEL;
	} else if( str == "KTHX" ) {
		return TokenType::KTHX;
	} else if( str == "SRS" ) {
		return TokenType::SRS;
	} else if( str == "SLOT" ) {
		return TokenType::SLOT;
	}

	return TokenType::IDENTIFIER;
}

void Tokenizer::AddToken( Token newToken ) {
	if( inMultiLineComment_ ) {
		return;
	}

	tokens_.AddToken( newToken );

	continueCurrentLine_ = false;
	addLineDelimiter_ = ( newToken.type != TokenType::LINE_DELIMITER );
}

void Tokenizer::AddSimpleToken( TokenType newTokenType, bool overrideGuard ) {
	if( inMultiLineComment_ && !overrideGuard ) {
		return;
	}

	Token newToken;
	newToken.type = newTokenType;
	tokens_.AddToken( newToken );

	continueCurrentLine_ = false;
	addLineDelimiter_ = ( newTokenType != TokenType::LINE_DELIMITER );
}

bool Tokenizer::Tokenize( std::string srcFile ) {
	std::ifstream inFile(srcFile);
	std::string line;

	TokenizeState state = TokenizeState::START;
	
	while( std::getline( inFile, line ) ) {
		unsigned int i = 0;
		unsigned int tokenStart = 0;

		if( continueCurrentLine_ && line == "" ) {
			return false;
		}

		line += ' ';
		std::string yarnLiteral = "";

		bool inSingleLineComment = false;
		bool yarnEscaped = false;
		continueCurrentLine_ = false;

		while( i < line.length() && !inSingleLineComment ) {
			char ch = line.at( i );
			switch( state ) {
				case TokenizeState::START:
					tokenStart = i;
					switch( GetCharType( ch ) ) {
						case CharType::LETTER:
							state = TokenizeState::READ_KEYWORD_OR_IDENT;
							break;
						case CharType::DIGIT:
							state = TokenizeState::READ_NUMBR_LITERAL;
							break;
						case CharType::PERIOD:
							state = TokenizeState::READ_LINE_CONTINUE;
							break;
						case CharType::DOUBLE_QUOTE:
							if( !inSingleLineComment && !inMultiLineComment_ ) {
								state = TokenizeState::READ_YARN_LITERAL;
							}
							break;
						case CharType::MINUS:
							AddSimpleToken( TokenType::MINUS_SIGN );
							break;
						case CharType::EXCLAMATION:
							AddSimpleToken( TokenType::EXCLAMATION );
							break;
						case CharType::QUESTION:
							AddSimpleToken( TokenType::QUESTION );
							break;
						case CharType::LINE_DELIMITER:
							AddSimpleToken( TokenType::LINE_DELIMITER );
							break;
						case CharType::SPACE:
							break;
						default:
							if( !inSingleLineComment && !inMultiLineComment_ ) {
								return false;
							}
							break;
					}
					++i;
					break;
				case TokenizeState::READ_LINE_CONTINUE:
					switch( i - tokenStart ) {
						case 1:
							switch( Tokenizer::GetCharType( ch ) ) {
								case CharType::DIGIT:
									state = TokenizeState::READ_NUMBAR_LITERAL;
								case CharType::PERIOD:
									break;
								default:
									if( inSingleLineComment || inMultiLineComment_ ) {
										break;
									}
									return false;
							}
							++i;
							break;
						case 2:
							if( Tokenizer::GetCharType( ch ) != CharType::PERIOD && 
								!inSingleLineComment && !inMultiLineComment_ ) {
								return false;
							}
							++i;
							break;
						case 3:
							continueCurrentLine_ = 
								( !inMultiLineComment_ || continueCurrentLine_ );
							state = TokenizeState::START;
							break;
						default:
							if( !inSingleLineComment && !inMultiLineComment_ ) {
								return false;
							}
							++i;
							break;
					}
					break;
				case TokenizeState::READ_KEYWORD_OR_IDENT:
					switch( Tokenizer::GetCharType( ch ) ) {
						case CharType::LETTER:
						case CharType::DIGIT:
						case CharType::UNDERSCORE:
							++i;
							break;
						case CharType::EXCLAMATION:
						case CharType::QUESTION:
						case CharType::SPACE:
						case CharType::LINE_DELIMITER:
							{
								Token newToken;
								std::string tokenStr = 
									line.substr( tokenStart, i - tokenStart );
								newToken.type = GetTokenType( tokenStr );
								switch( newToken.type ) {
									case TokenType::BTW:
										inSingleLineComment = true;
										state = TokenizeState::START;
										break;
									case TokenType::OBTW:
										inMultiLineComment_ = true;
										state = TokenizeState::START;
										break;
									case TokenType::TLDR:
										inMultiLineComment_ = false;
										state = TokenizeState::START;
										break;
									case TokenType::IDENTIFIER:
										newToken.string = tokenStr;
									default:
										AddToken( newToken );
										state = TokenizeState::START;
										break;
								}
							}
							break;
						default:
							if( !inSingleLineComment && !inMultiLineComment_ ) {
								return false;
							}
							++i;
							break;
					}
					break;
				case TokenizeState::READ_YARN_LITERAL:
					if( yarnEscaped ) {
						yarnEscaped = false;
						switch( Tokenizer::GetCharType( ch ) ) {
							case CharType::YARN_NEWLINE:
								yarnLiteral += "\n";
								break;
							case CharType::YARN_TAB:
								yarnLiteral += "\t";
								break;
							case CharType::LETTER:
								if( ch == 'o' ) {
									yarnLiteral += "\a";
									break;
								}
							case CharType::DOUBLE_QUOTE:
							case CharType::YARN_ESCAPE:
							default:
								yarnLiteral += ch;
								break;
						}
					} else {
						switch( Tokenizer::GetCharType( ch ) ) {
							case CharType::DOUBLE_QUOTE:
								{
									Token newToken;
									newToken.type = TokenType::YARN_LITERAL;
									newToken.string = yarnLiteral;
									AddToken( newToken );

									state = TokenizeState::START;
									yarnLiteral = "";
								}
								break;
							case CharType::YARN_ESCAPE:
								yarnEscaped = true;
								break;
							default:
								yarnLiteral += ch;
								break;
						}
					}
					++i;
					break;
				case TokenizeState::READ_NUMBR_LITERAL:
				case TokenizeState::READ_NUMBAR_LITERAL:
					switch( Tokenizer::GetCharType( ch ) ) {
						case CharType::DIGIT:
							++i;
							break;
						case CharType::PERIOD:
							if( state == TokenizeState::READ_NUMBR_LITERAL ) {
								state = TokenizeState::READ_NUMBAR_LITERAL;
								++i;
								break;
							}
						case CharType::EXCLAMATION:
						case CharType::QUESTION:
						case CharType::SPACE:
						case CharType::LINE_DELIMITER:
							{
								Token newToken;
								newToken.type = 
									( state == TokenizeState::READ_NUMBAR_LITERAL ) ? 
									TokenType::NUMBAR_LITERAL : 
									TokenType::NUMBR_LITERAL;
								newToken.string = 
									line.substr( tokenStart, i - tokenStart );
								AddToken( newToken );

								state = TokenizeState::START;
							}
							break;
						default:
							if( !inSingleLineComment && !inMultiLineComment_ ) {
								return false;
							}
							++i;
							break;
					}
					break;
				default:
					assert( false );
					return false;
			}
		}
		if( state == TokenizeState::READ_YARN_LITERAL &&
			!inSingleLineComment && !inMultiLineComment_ ) {
			return false;
		}
		if( !continueCurrentLine_ && addLineDelimiter_ ) {
			AddSimpleToken( TokenType::LINE_DELIMITER, true );
		}
	}

	return true;
}

