
/* #line 1 "../src/grammar_lex/header_lexer.rl" */
#include "header_lexer.h"
#include "tokens.h"


/* #line 60 "../src/grammar_lex/header_lexer.rl" */



/* #line 7 "../src/lexer//header_lexer.cc" */
static const int header_lexer_start = 10;
static const int header_lexer_first_final = 10;
static const int header_lexer_error = -1;

static const int header_lexer_en_decl_attr_initial = 14;
static const int header_lexer_en_decl_attr_regex_seq = 18;
static const int header_lexer_en_decl_var_initial = 22;
static const int header_lexer_en_decl_var_value_seq = 26;
static const int header_lexer_en_main = 10;


/* #line 63 "../src/grammar_lex/header_lexer.rl" */

void header_lexer::init()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    
/* #line 21 "../src/lexer//header_lexer.cc" */
	{
	cs = header_lexer_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 68 "../src/grammar_lex/header_lexer.rl" */
}

void header_lexer::exec()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    
/* #line 32 "../src/lexer//header_lexer.cc" */
	{
	if ( p == pe )
		goto _test_eof;
	goto _resume;

_again:
	switch ( cs ) {
		case 10: goto st10;
		case 11: goto st11;
		case 12: goto st12;
		case 0: goto st0;
		case 13: goto st13;
		case 1: goto st1;
		case 2: goto st2;
		case 3: goto st3;
		case 4: goto st4;
		case 5: goto st5;
		case 14: goto st14;
		case 15: goto st15;
		case 16: goto st16;
		case 6: goto st6;
		case 17: goto st17;
		case 18: goto st18;
		case 19: goto st19;
		case 20: goto st20;
		case 21: goto st21;
		case 7: goto st7;
		case 22: goto st22;
		case 23: goto st23;
		case 24: goto st24;
		case 8: goto st8;
		case 25: goto st25;
		case 26: goto st26;
		case 27: goto st27;
		case 28: goto st28;
		case 9: goto st9;
		case 29: goto st29;
	default: break;
	}

	if ( ++p == pe )
		goto _test_eof;
_resume:
	switch ( cs )
	{
tr0:
/* #line 58 "../src/grammar_lex/header_lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st10;
tr1:
/* #line 56 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);}}
	goto st10;
tr5:
	cs = 10;
/* #line 54 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::DECL_ATTR); cs = 14;}}
	goto _again;
tr7:
	cs = 10;
/* #line 53 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::DECL_VAR);  cs = 22;}}
	goto _again;
tr20:
/* #line 58 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st10;
tr21:
/* #line 57 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ }}
	goto st10;
tr22:
/* #line 55 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);}}
	goto st10;
tr26:
/* #line 58 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st10;
st10:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof10;
case 10:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 109 "../src/lexer//header_lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr22;
		case 13: goto st11;
		case 32: goto tr21;
		case 35: goto tr24;
		case 62: goto tr25;
	}
	if ( 9 <= (*p) && (*p) <= 12 )
		goto tr21;
	goto tr20;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 10 )
		goto tr22;
	goto tr26;
tr24:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
/* #line 133 "../src/lexer//header_lexer.cc" */
	if ( (*p) == 10 )
		goto tr1;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st0;
	goto tr26;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
	if ( (*p) == 10 )
		goto tr1;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st0;
	goto tr0;
tr25:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
/* #line 154 "../src/lexer//header_lexer.cc" */
	switch( (*p) ) {
		case 97: goto st1;
		case 118: goto st4;
	}
	goto tr26;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) == 116 )
		goto st2;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 116 )
		goto st3;
	goto tr0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 114 )
		goto tr5;
	goto tr0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 97 )
		goto st5;
	goto tr0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 114 )
		goto tr7;
	goto tr0;
tr8:
/* #line 24 "../src/grammar_lex/header_lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st14;
tr9:
	cs = 14;
/* #line 20 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr29:
/* #line 24 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st14;
tr30:
/* #line 23 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ }}
	goto st14;
tr31:
	cs = 14;
/* #line 19 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr34:
	cs = 14;
/* #line 22 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(';');                      cs = 18;}}
	goto _again;
tr36:
/* #line 24 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st14;
tr37:
/* #line 21 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(header_tokens::IDENT);}}
	goto st14;
st14:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof14;
case 14:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 227 "../src/lexer//header_lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr31;
		case 13: goto st15;
		case 32: goto tr30;
		case 35: goto tr33;
		case 59: goto tr34;
		case 95: goto st17;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr30;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st17;
	} else
		goto st17;
	goto tr29;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 10 )
		goto tr31;
	goto tr36;
tr33:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 258 "../src/lexer//header_lexer.cc" */
	if ( (*p) == 10 )
		goto tr9;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st6;
	goto tr36;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 10 )
		goto tr9;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st6;
	goto tr8;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 46: goto st17;
		case 95: goto st17;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st17;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st17;
	} else
		goto st17;
	goto tr37;
tr11:
/* #line 32 "../src/grammar_lex/header_lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st18;
tr12:
	cs = 18;
/* #line 29 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr38:
/* #line 32 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st18;
tr39:
/* #line 31 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ }}
	goto st18;
tr40:
	cs = 18;
/* #line 28 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr44:
/* #line 32 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st18;
tr45:
/* #line 30 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(header_tokens::REGEX);}}
	goto st18;
st18:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 318 "../src/lexer//header_lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr40;
		case 13: goto st19;
		case 32: goto tr39;
		case 35: goto tr43;
	}
	if ( (*p) < 33 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr39;
	} else if ( (*p) > 58 ) {
		if ( 60 <= (*p) && (*p) <= 126 )
			goto st20;
	} else
		goto st20;
	goto tr38;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 10 )
		goto tr40;
	goto tr44;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) < 36 ) {
		if ( 33 <= (*p) && (*p) <= 34 )
			goto st20;
	} else if ( (*p) > 58 ) {
		if ( 60 <= (*p) && (*p) <= 126 )
			goto st20;
	} else
		goto st20;
	goto tr45;
tr43:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
/* #line 360 "../src/lexer//header_lexer.cc" */
	if ( (*p) == 10 )
		goto tr12;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st7;
	goto tr44;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 10 )
		goto tr12;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st7;
	goto tr11;
tr14:
/* #line 41 "../src/grammar_lex/header_lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st22;
tr15:
	cs = 22;
/* #line 37 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr46:
/* #line 41 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st22;
tr47:
/* #line 40 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ }}
	goto st22;
tr48:
	cs = 22;
/* #line 36 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr51:
	cs = 22;
/* #line 39 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(';');                      cs = 26;}}
	goto _again;
tr53:
/* #line 41 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st22;
tr54:
/* #line 38 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(header_tokens::IDENT);}}
	goto st22;
st22:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 407 "../src/lexer//header_lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr48;
		case 13: goto st23;
		case 32: goto tr47;
		case 35: goto tr50;
		case 59: goto tr51;
		case 95: goto st25;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr47;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st25;
	} else
		goto st25;
	goto tr46;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 10 )
		goto tr48;
	goto tr53;
tr50:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
/* #line 438 "../src/lexer//header_lexer.cc" */
	if ( (*p) == 10 )
		goto tr15;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st8;
	goto tr53;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 10 )
		goto tr15;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st8;
	goto tr14;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 46: goto st25;
		case 95: goto st25;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st25;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st25;
	} else
		goto st25;
	goto tr54;
tr17:
/* #line 49 "../src/grammar_lex/header_lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st26;
tr18:
	cs = 26;
/* #line 46 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr55:
/* #line 49 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st26;
tr56:
/* #line 48 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ }}
	goto st26;
tr57:
	cs = 26;
/* #line 45 "../src/grammar_lex/header_lexer.rl" */
	{te = p+1;{ push_token(header_tokens::NEWLINE);   cs = 10;}}
	goto _again;
tr61:
/* #line 49 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st26;
tr62:
/* #line 47 "../src/grammar_lex/header_lexer.rl" */
	{te = p;p--;{ push_token(header_tokens::IDENT);}}
	goto st26;
st26:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof26;
case 26:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 498 "../src/lexer//header_lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr57;
		case 13: goto st27;
		case 32: goto tr56;
		case 35: goto tr59;
		case 95: goto st29;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr56;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st29;
	} else
		goto st29;
	goto tr55;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 10 )
		goto tr57;
	goto tr61;
tr59:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 528 "../src/lexer//header_lexer.cc" */
	if ( (*p) == 10 )
		goto tr18;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st9;
	goto tr61;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 10 )
		goto tr18;
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st9;
	goto tr17;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 46: goto st29;
		case 95: goto st29;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st29;
	} else
		goto st29;
	goto tr62;
	}
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 11: goto tr26;
	case 12: goto tr26;
	case 0: goto tr0;
	case 13: goto tr26;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 4: goto tr0;
	case 5: goto tr0;
	case 15: goto tr36;
	case 16: goto tr36;
	case 6: goto tr8;
	case 17: goto tr37;
	case 19: goto tr44;
	case 20: goto tr45;
	case 21: goto tr44;
	case 7: goto tr11;
	case 23: goto tr53;
	case 24: goto tr53;
	case 8: goto tr14;
	case 25: goto tr54;
	case 27: goto tr61;
	case 28: goto tr61;
	case 9: goto tr17;
	case 29: goto tr62;
	}
	}

	}

/* #line 74 "../src/grammar_lex/header_lexer.rl" */
}
