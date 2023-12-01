
/* #line 1 "../src/lexer/lexer.rl" */
#include "lexer.h"
#include "tokens.h"

#include <algorithm>


/* #line 75 "../src/lexer/lexer.rl" */



/* #line 9 "../src/lexer//lexer.cc" */
static const int lexer_start = 17;
static const int lexer_first_final = 17;
static const int lexer_error = -1;

static const int lexer_en_decl_attr_initial = 29;
static const int lexer_en_decl_attr_regex_seq = 35;
static const int lexer_en_decl_var_value_seq = 41;
static const int lexer_en_attr_val_seq = 47;
static const int lexer_en_main = 17;


/* #line 78 "../src/lexer/lexer.rl" */

void lexer::init()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    
/* #line 23 "../src/lexer//lexer.cc" */
	{
	cs = lexer_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 83 "../src/lexer/lexer.rl" */
}

void lexer::exec()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    
/* #line 34 "../src/lexer//lexer.cc" */
	{
	if ( p == pe )
		goto _test_eof;
	goto _resume;

_again:
	switch ( cs ) {
		case 17: goto st17;
		case 18: goto st18;
		case 19: goto st19;
		case 0: goto st0;
		case 20: goto st20;
		case 1: goto st1;
		case 2: goto st2;
		case 3: goto st3;
		case 4: goto st4;
		case 5: goto st5;
		case 6: goto st6;
		case 7: goto st7;
		case 8: goto st8;
		case 21: goto st21;
		case 22: goto st22;
		case 23: goto st23;
		case 24: goto st24;
		case 25: goto st25;
		case 26: goto st26;
		case 27: goto st27;
		case 28: goto st28;
		case 9: goto st9;
		case 29: goto st29;
		case 30: goto st30;
		case 31: goto st31;
		case 10: goto st10;
		case 32: goto st32;
		case 33: goto st33;
		case 34: goto st34;
		case 35: goto st35;
		case 36: goto st36;
		case 37: goto st37;
		case 38: goto st38;
		case 11: goto st11;
		case 39: goto st39;
		case 40: goto st40;
		case 41: goto st41;
		case 42: goto st42;
		case 43: goto st43;
		case 12: goto st12;
		case 44: goto st44;
		case 45: goto st45;
		case 46: goto st46;
		case 47: goto st47;
		case 48: goto st48;
		case 49: goto st49;
		case 13: goto st13;
		case 50: goto st50;
		case 14: goto st14;
		case 51: goto st51;
		case 15: goto st15;
		case 52: goto st52;
		case 16: goto st16;
	default: break;
	}

	if ( ++p == pe )
		goto _test_eof;
_resume:
	switch ( cs )
	{
tr0:
/* #line 71 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ }}
	goto st17;
tr1:
/* #line 71 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st17;
tr2:
/* #line 73 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st17;
tr10:
/* #line 66 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(tokens::DATE); }}
	goto st17;
tr11:
/* #line 70 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st17;
tr26:
/* #line 73 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st17;
tr27:
/* #line 72 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st17;
tr33:
/* #line 73 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st17;
tr34:
/* #line 71 "../src/lexer/lexer.rl" */
	{te = p;p--;{ }}
	goto st17;
tr40:
	cs = 17;
/* #line 1 "NONE" */
	{	switch( act ) {
	case 33:
	{{p = ((te))-1;} auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::DECL_VAR); --ts;  cs = 41;}
	break;
	case 34:
	{{p = ((te))-1;} auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::DECL_ATTR); --ts; cs = 29;}
	break;
	case 35:
	{{p = ((te))-1;} auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::IDENT); --ts; cs = 47;}
	break;
	}
	}
	goto _again;
tr41:
	cs = 17;
/* #line 69 "../src/lexer/lexer.rl" */
	{te = p;p--;{ auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::IDENT); --ts; cs = 47;}}
	goto _again;
st17:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 154 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr11;
		case 13: goto st18;
		case 32: goto tr27;
		case 35: goto tr29;
		case 62: goto st21;
		case 69: goto tr32;
	}
	if ( (*p) > 12 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr30;
	} else if ( (*p) >= 9 )
		goto tr27;
	goto tr26;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 10 )
		goto tr11;
	goto tr33;
tr29:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
/* #line 182 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr1;
		case 13: goto st0;
	}
	if ( (*p) > 38 ) {
		if ( 40 <= (*p) && (*p) <= 126 )
			goto tr29;
	} else if ( (*p) >= 32 )
		goto tr29;
	goto tr34;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
	if ( (*p) == 10 )
		goto tr1;
	goto tr0;
tr30:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
/* #line 206 "../src/lexer//lexer.cc" */
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st1;
	goto tr33;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st2;
	goto tr2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st3;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 45 )
		goto st4;
	goto tr2;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st5;
	goto tr2;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st6;
	goto tr2;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 45 )
		goto st7;
	goto tr2;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto tr2;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr10;
	goto tr2;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 95: goto tr37;
		case 97: goto st23;
		case 118: goto st26;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else if ( (*p) >= 65 )
		goto tr37;
	goto tr33;
tr37:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 69 "../src/lexer/lexer.rl" */
	{act = 35;}
	goto st22;
tr44:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 68 "../src/lexer/lexer.rl" */
	{act = 34;}
	goto st22;
tr46:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 67 "../src/lexer/lexer.rl" */
	{act = 33;}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
/* #line 296 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 46: goto tr37;
		case 95: goto tr37;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr37;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else
		goto tr37;
	goto tr40;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 46: goto tr37;
		case 95: goto tr37;
		case 116: goto st24;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr37;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else
		goto tr37;
	goto tr41;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 46: goto tr37;
		case 95: goto tr37;
		case 116: goto st25;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr37;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else
		goto tr37;
	goto tr41;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 46: goto tr37;
		case 95: goto tr37;
		case 114: goto tr44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr37;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else
		goto tr37;
	goto tr41;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 46: goto tr37;
		case 95: goto tr37;
		case 97: goto st27;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr37;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else
		goto tr37;
	goto tr41;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 46: goto tr37;
		case 95: goto tr37;
		case 114: goto tr46;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr37;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr37;
	} else
		goto tr37;
	goto tr41;
tr32:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
/* #line 406 "../src/lexer//lexer.cc" */
	if ( (*p) == 79 )
		goto st9;
	goto tr33;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 70 )
		goto tr11;
	goto tr2;
tr12:
/* #line 26 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ }}
	goto st29;
tr13:
/* #line 26 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st29;
tr48:
/* #line 28 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st29;
tr49:
/* #line 27 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st29;
tr50:
/* #line 25 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st29;
tr53:
	cs = 29;
/* #line 24 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(';');           cs = 17;}}
	goto _again;
tr56:
/* #line 28 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st29;
tr57:
/* #line 26 "../src/lexer/lexer.rl" */
	{te = p;p--;{ }}
	goto st29;
tr59:
	cs = 29;
/* #line 23 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(tokens::IDENT); cs = 35;}}
	goto _again;
st29:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof29;
case 29:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 451 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr50;
		case 13: goto st30;
		case 32: goto tr49;
		case 35: goto tr52;
		case 59: goto tr53;
		case 69: goto st33;
		case 95: goto st32;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr49;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st32;
	} else
		goto st32;
	goto tr48;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 10 )
		goto tr50;
	goto tr56;
tr52:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
/* #line 483 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr13;
		case 13: goto st10;
	}
	if ( (*p) > 38 ) {
		if ( 40 <= (*p) && (*p) <= 126 )
			goto tr52;
	} else if ( (*p) >= 32 )
		goto tr52;
	goto tr57;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 10 )
		goto tr13;
	goto tr12;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 46: goto st32;
		case 95: goto st32;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st32;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st32;
	} else
		goto st32;
	goto tr59;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 46: goto st32;
		case 79: goto st34;
		case 95: goto st32;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st32;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st32;
	} else
		goto st32;
	goto tr59;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 46: goto st32;
		case 95: goto st32;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st32;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st32;
	} else
		goto st32;
	goto tr59;
tr14:
/* #line 37 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ }}
	goto st35;
tr15:
/* #line 37 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st35;
tr61:
/* #line 40 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st35;
tr62:
/* #line 38 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st35;
tr63:
/* #line 36 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st35;
tr67:
/* #line 33 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token('&');}}
	goto st35;
tr68:
	cs = 35;
/* #line 35 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(';'); cs = 17;}}
	goto _again;
tr70:
/* #line 34 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token('|');}}
	goto st35;
tr71:
/* #line 40 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st35;
tr72:
/* #line 32 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(tokens::REGEX);}}
	goto st35;
tr73:
/* #line 37 "../src/lexer/lexer.rl" */
	{te = p;p--;{ }}
	goto st35;
st35:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof35;
case 35:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 592 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr63;
		case 13: goto st36;
		case 32: goto tr62;
		case 35: goto tr66;
		case 38: goto tr67;
		case 59: goto tr68;
		case 69: goto st39;
		case 124: goto tr70;
	}
	if ( (*p) > 12 ) {
		if ( 33 <= (*p) && (*p) <= 126 )
			goto st37;
	} else if ( (*p) >= 9 )
		goto tr62;
	goto tr61;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 10 )
		goto tr63;
	goto tr71;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) < 39 ) {
		if ( (*p) > 34 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st37;
		} else if ( (*p) >= 33 )
			goto st37;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st37;
		} else if ( (*p) >= 60 )
			goto st37;
	} else
		goto st37;
	goto tr72;
tr66:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
/* #line 641 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr15;
		case 13: goto st11;
	}
	if ( (*p) > 38 ) {
		if ( 40 <= (*p) && (*p) <= 126 )
			goto tr66;
	} else if ( (*p) >= 32 )
		goto tr66;
	goto tr73;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 10 )
		goto tr15;
	goto tr14;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 79 )
		goto st40;
	if ( (*p) < 39 ) {
		if ( (*p) > 34 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st37;
		} else if ( (*p) >= 33 )
			goto st37;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st37;
		} else if ( (*p) >= 60 )
			goto st37;
	} else
		goto st37;
	goto tr72;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) < 39 ) {
		if ( (*p) > 34 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st37;
		} else if ( (*p) >= 33 )
			goto st37;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st37;
		} else if ( (*p) >= 60 )
			goto st37;
	} else
		goto st37;
	goto tr72;
tr16:
/* #line 49 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ }}
	goto st41;
tr17:
/* #line 49 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st41;
tr76:
/* #line 51 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st41;
tr77:
/* #line 50 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st41;
tr78:
/* #line 48 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st41;
tr81:
/* #line 45 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token('&');}}
	goto st41;
tr82:
	cs = 41;
/* #line 47 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(';'); cs = 17;}}
	goto _again;
tr85:
/* #line 46 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token('|');}}
	goto st41;
tr86:
/* #line 51 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st41;
tr87:
/* #line 49 "../src/lexer/lexer.rl" */
	{te = p;p--;{ }}
	goto st41;
tr89:
/* #line 44 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(tokens::IDENT);}}
	goto st41;
st41:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof41;
case 41:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 738 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr78;
		case 13: goto st42;
		case 32: goto tr77;
		case 35: goto tr80;
		case 38: goto tr81;
		case 59: goto tr82;
		case 69: goto st45;
		case 95: goto st44;
		case 124: goto tr85;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr77;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st44;
	} else
		goto st44;
	goto tr76;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 10 )
		goto tr78;
	goto tr86;
tr80:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
/* #line 772 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr17;
		case 13: goto st12;
	}
	if ( (*p) > 38 ) {
		if ( 40 <= (*p) && (*p) <= 126 )
			goto tr80;
	} else if ( (*p) >= 32 )
		goto tr80;
	goto tr87;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 10 )
		goto tr17;
	goto tr16;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 46: goto st44;
		case 95: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st44;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st44;
	} else
		goto st44;
	goto tr89;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 46: goto st44;
		case 79: goto st46;
		case 95: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st44;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st44;
	} else
		goto st44;
	goto tr89;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 46: goto st44;
		case 95: goto st44;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st44;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st44;
	} else
		goto st44;
	goto tr89;
tr18:
/* #line 60 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ }}
	goto st47;
tr19:
/* #line 60 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st47;
tr20:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 28:
	{{p = ((te))-1;} }
	break;
	case 31:
	{{p = ((te))-1;} push_token(*ts);}
	break;
	}
	}
	goto st47;
tr24:
/* #line 62 "../src/lexer/lexer.rl" */
	{{p = ((te))-1;}{ push_token(*ts);}}
	goto st47;
tr91:
/* #line 62 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(*ts);}}
	goto st47;
tr92:
/* #line 61 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st47;
tr93:
/* #line 59 "../src/lexer/lexer.rl" */
	{te = p+1;{ }}
	goto st47;
tr96:
/* #line 56 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token('&');}}
	goto st47;
tr97:
	cs = 47;
/* #line 58 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token(';'); cs = 17;}}
	goto _again;
tr100:
/* #line 57 "../src/lexer/lexer.rl" */
	{te = p+1;{ push_token('|');}}
	goto st47;
tr101:
/* #line 62 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_token(*ts);}}
	goto st47;
tr102:
/* #line 60 "../src/lexer/lexer.rl" */
	{te = p;p--;{ }}
	goto st47;
tr104:
/* #line 55 "../src/lexer/lexer.rl" */
	{te = p;p--;{ push_ident_value_pair(m_ts, m_te);}}
	goto st47;
st47:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof47;
case 47:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 895 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr93;
		case 13: goto st48;
		case 32: goto tr92;
		case 35: goto tr95;
		case 38: goto tr96;
		case 59: goto tr97;
		case 69: goto tr99;
		case 95: goto tr98;
		case 124: goto tr100;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 12 )
			goto tr92;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr98;
	} else
		goto tr98;
	goto tr91;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 10 )
		goto tr93;
	goto tr101;
tr95:
/* #line 1 "NONE" */
	{te = p+1;}
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
/* #line 929 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 10: goto tr19;
		case 13: goto st13;
	}
	if ( (*p) > 38 ) {
		if ( 40 <= (*p) && (*p) <= 126 )
			goto tr95;
	} else if ( (*p) >= 32 )
		goto tr95;
	goto tr102;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 10 )
		goto tr19;
	goto tr18;
tr25:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 59 "../src/lexer/lexer.rl" */
	{act = 28;}
	goto st50;
tr98:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 62 "../src/lexer/lexer.rl" */
	{act = 31;}
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
/* #line 958 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
		case 46: goto st15;
		case 95: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto st14;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto tr20;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
	}
	if ( (*p) < 36 ) {
		if ( (*p) > 12 ) {
			if ( 33 <= (*p) && (*p) <= 34 )
				goto st51;
		} else if ( (*p) >= 11 )
			goto st14;
	} else if ( (*p) > 37 ) {
		if ( (*p) < 60 ) {
			if ( 39 <= (*p) && (*p) <= 58 )
				goto st51;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st51;
		} else
			goto st51;
	} else
		goto st51;
	goto tr20;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) < 39 ) {
		if ( (*p) > 34 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st51;
		} else if ( (*p) >= 33 )
			goto st51;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st51;
		} else if ( (*p) >= 60 )
			goto st51;
	} else
		goto st51;
	goto tr104;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
		case 46: goto st15;
		case 95: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto st14;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto tr20;
tr99:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 62 "../src/lexer/lexer.rl" */
	{act = 31;}
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
/* #line 1051 "../src/lexer//lexer.cc" */
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
		case 46: goto st15;
		case 79: goto st16;
		case 95: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto st14;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto tr101;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 9: goto st14;
		case 32: goto st14;
		case 46: goto st15;
		case 70: goto tr25;
		case 95: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto st14;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto tr24;
	}
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 18: goto tr33;
	case 19: goto tr34;
	case 0: goto tr0;
	case 20: goto tr33;
	case 1: goto tr2;
	case 2: goto tr2;
	case 3: goto tr2;
	case 4: goto tr2;
	case 5: goto tr2;
	case 6: goto tr2;
	case 7: goto tr2;
	case 8: goto tr2;
	case 21: goto tr33;
	case 22: goto tr40;
	case 23: goto tr41;
	case 24: goto tr41;
	case 25: goto tr41;
	case 26: goto tr41;
	case 27: goto tr41;
	case 28: goto tr33;
	case 9: goto tr2;
	case 30: goto tr56;
	case 31: goto tr57;
	case 10: goto tr12;
	case 32: goto tr59;
	case 33: goto tr59;
	case 34: goto tr59;
	case 36: goto tr71;
	case 37: goto tr72;
	case 38: goto tr73;
	case 11: goto tr14;
	case 39: goto tr72;
	case 40: goto tr72;
	case 42: goto tr86;
	case 43: goto tr87;
	case 12: goto tr16;
	case 44: goto tr89;
	case 45: goto tr89;
	case 46: goto tr89;
	case 48: goto tr101;
	case 49: goto tr102;
	case 13: goto tr18;
	case 50: goto tr20;
	case 14: goto tr20;
	case 51: goto tr104;
	case 15: goto tr20;
	case 52: goto tr101;
	case 16: goto tr24;
	}
	}

	}

/* #line 89 "../src/lexer/lexer.rl" */
}

void lexer::push_ident_value_pair(char* start, char* end)
{
    // find first space
    auto space = std::find(start, end, ' ');
    
    // push ident
    push_token(tokens::IDENT, start, space);

    // skip any consecutive spaces
    auto next_char = std::find_if_not(space, end, [](char c) { return c == ' '; });
    
    // push attr_value
    push_token(tokens::ATTR_VALUE, next_char, end);
}
