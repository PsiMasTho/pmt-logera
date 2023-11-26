
/* #line 1 "../src/grammar_lex/log_lexer.rl" */
#include "log_lexer.h"
#include "tokens.h"

/* #line 45 "../src/grammar_lex/log_lexer.rl" */

/* #line 7 "../src/lexer//log_lexer.cc" */
static const int log_lexer_start = 19;
static const int log_lexer_first_final = 19;
static const int log_lexer_error = -1;

static const int log_lexer_en_log_file_initial = 24;
static const int log_lexer_en_logfile_attr_val_seq = 32;
static const int log_lexer_en_main = 19;

/* #line 48 "../src/grammar_lex/log_lexer.rl" */

void log_lexer::init()
{
    MAKE_RAGEL_STATE_AVAILABLE;

    /* #line 19 "../src/lexer//log_lexer.cc" */
    {
        cs = log_lexer_start;
        ts = 0;
        te = 0;
        act = 0;
    }

    /* #line 53 "../src/grammar_lex/log_lexer.rl" */
}

void log_lexer::exec()
{
    static int n_seq_nl = 0;
    MAKE_RAGEL_STATE_AVAILABLE;

    /* #line 31 "../src/lexer//log_lexer.cc" */
    {
        if(p == pe)
            goto _test_eof;
        goto _resume;

    _again:
        switch(cs)
        {
        case 19:
            goto st19;
        case 20:
            goto st20;
        case 21:
            goto st21;
        case 0:
            goto st0;
        case 22:
            goto st22;
        case 1:
            goto st1;
        case 2:
            goto st2;
        case 3:
            goto st3;
        case 4:
            goto st4;
        case 5:
            goto st5;
        case 6:
            goto st6;
        case 7:
            goto st7;
        case 8:
            goto st8;
        case 23:
            goto st23;
        case 9:
            goto st9;
        case 24:
            goto st24;
        case 25:
            goto st25;
        case 26:
            goto st26;
        case 10:
            goto st10;
        case 27:
            goto st27;
        case 11:
            goto st11;
        case 12:
            goto st12;
        case 28:
            goto st28;
        case 13:
            goto st13;
        case 14:
            goto st14;
        case 29:
            goto st29;
        case 30:
            goto st30;
        case 31:
            goto st31;
        case 32:
            goto st32;
        case 33:
            goto st33;
        case 34:
            goto st34;
        case 15:
            goto st15;
        case 35:
            goto st35;
        case 16:
            goto st16;
        case 36:
            goto st36;
        case 17:
            goto st17;
        case 37:
            goto st37;
        case 18:
            goto st18;
        default:
            break;
        }

        if(++p == pe)
            goto _test_eof;
    _resume:
        switch(cs)
        {
        tr0:
            /* #line 41 "../src/grammar_lex/log_lexer.rl" */
            {
                {
                    p = ((te)) - 1;
                }
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st19;
        tr1:
            /* #line 41 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st19;
        tr2:
            /* #line 43 "../src/grammar_lex/log_lexer.rl" */
            {
                {
                    p = ((te)) - 1;
                }
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st19;
        tr10:
            cs = 19;
            /* #line 39 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::DATE);
                    cs = 24;
                }
            }
            goto _again;
        tr11:
            /* #line 40 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st19;
        tr30:
            /* #line 43 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st19;
        tr31:
            /* #line 42 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                { }
            }
            goto st19;
        tr36:
            /* #line 43 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st19;
        tr37:
            /* #line 41 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st19;
        st19:
            /* #line 1 "NONE" */
            {
                ts = 0;
            }
            if(++p == pe)
                goto _test_eof19;
        case 19:
            /* #line 1 "NONE" */
            {
                ts = p;
            }
            /* #line 118 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr11;
            case 13:
                goto st20;
            case 32:
                goto tr31;
            case 35:
                goto tr33;
            case 69:
                goto tr35;
            }
            if((*p) > 12)
            {
                if(48 <= (*p) && (*p) <= 57)
                    goto tr34;
            }
            else if((*p) >= 9)
                goto tr31;
            goto tr30;
        st20:
            if(++p == pe)
                goto _test_eof20;
        case 20:
            if((*p) == 10)
                goto tr11;
            goto tr36;
        tr33:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            goto st21;
        st21:
            if(++p == pe)
                goto _test_eof21;
        case 21:
            /* #line 145 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr1;
            case 13:
                goto st0;
            }
            if((*p) > 38)
            {
                if(40 <= (*p) && (*p) <= 126)
                    goto tr33;
            }
            else if((*p) >= 32)
                goto tr33;
            goto tr37;
        st0:
            if(++p == pe)
                goto _test_eof0;
        case 0:
            if((*p) == 10)
                goto tr1;
            goto tr0;
        tr34:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            goto st22;
        st22:
            if(++p == pe)
                goto _test_eof22;
        case 22:
            /* #line 169 "../src/lexer//log_lexer.cc" */
            if(48 <= (*p) && (*p) <= 57)
                goto st1;
            goto tr36;
        st1:
            if(++p == pe)
                goto _test_eof1;
        case 1:
            if(48 <= (*p) && (*p) <= 57)
                goto st2;
            goto tr2;
        st2:
            if(++p == pe)
                goto _test_eof2;
        case 2:
            if(48 <= (*p) && (*p) <= 57)
                goto st3;
            goto tr2;
        st3:
            if(++p == pe)
                goto _test_eof3;
        case 3:
            if((*p) == 45)
                goto st4;
            goto tr2;
        st4:
            if(++p == pe)
                goto _test_eof4;
        case 4:
            if(48 <= (*p) && (*p) <= 57)
                goto st5;
            goto tr2;
        st5:
            if(++p == pe)
                goto _test_eof5;
        case 5:
            if(48 <= (*p) && (*p) <= 57)
                goto st6;
            goto tr2;
        st6:
            if(++p == pe)
                goto _test_eof6;
        case 6:
            if((*p) == 45)
                goto st7;
            goto tr2;
        st7:
            if(++p == pe)
                goto _test_eof7;
        case 7:
            if(48 <= (*p) && (*p) <= 57)
                goto st8;
            goto tr2;
        st8:
            if(++p == pe)
                goto _test_eof8;
        case 8:
            if(48 <= (*p) && (*p) <= 57)
                goto tr10;
            goto tr2;
        tr35:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            goto st23;
        st23:
            if(++p == pe)
                goto _test_eof23;
        case 23:
            /* #line 235 "../src/lexer//log_lexer.cc" */
            if((*p) == 79)
                goto st9;
            goto tr36;
        st9:
            if(++p == pe)
                goto _test_eof9;
        case 9:
            if((*p) == 70)
                goto tr11;
            goto tr2;
        tr12:
            /* #line 22 "../src/grammar_lex/log_lexer.rl" */
            {
                {
                    p = ((te)) - 1;
                }
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st24;
        tr13:
            /* #line 22 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st24;
        tr14:
            /* #line 26 "../src/grammar_lex/log_lexer.rl" */
            {
                {
                    p = ((te)) - 1;
                }
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st24;
        tr16:
            cs = 24;
            /* #line 24 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(':');
                    cs = 32;
                }
            }
            goto _again;
        tr20:
            cs = 24;
            /* #line 1 "NONE" */
            {
                switch(act)
                {
                case 1: {
                    {
                        p = ((te)) - 1;
                    }
                    push_token(log_tokens::NEWLINE);
                }
                break;
                case 3: {
                    {
                        p = ((te)) - 1;
                    }
                    push_token(log_tokens::IDENT);
                }
                break;
                case 4: {
                    {
                        p = ((te)) - 1;
                    }
                    push_token(':');
                    cs = 32;
                }
                break;
                case 6: {
                    {
                        p = ((te)) - 1;
                    }
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
                break;
                }
            }
            goto _again;
        tr41:
            /* #line 26 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st24;
        tr42:
            /* #line 25 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                { }
            }
            goto st24;
        tr43:
            /* #line 21 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st24;
        tr49:
            /* #line 26 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st24;
        tr50:
            /* #line 22 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    push_token(log_tokens::NEWLINE);
                }
            }
            goto st24;
        tr52:
            cs = 24;
            /* #line 24 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    push_token(':');
                    cs = 32;
                }
            }
            goto _again;
        tr53:
            /* #line 23 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    push_token(log_tokens::IDENT);
                }
            }
            goto st24;
        st24:
            /* #line 1 "NONE" */
            {
                ts = 0;
            }
            if(++p == pe)
                goto _test_eof24;
        case 24:
            /* #line 1 "NONE" */
            {
                ts = p;
            }
            /* #line 304 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr43;
            case 13:
                goto st25;
            case 32:
                goto tr42;
            case 35:
                goto tr45;
            case 58:
                goto tr46;
            case 69:
                goto st30;
            case 95:
                goto tr47;
            }
            if((*p) < 65)
            {
                if(9 <= (*p) && (*p) <= 12)
                    goto tr42;
            }
            else if((*p) > 90)
            {
                if(97 <= (*p) && (*p) <= 122)
                    goto tr47;
            }
            else
                goto tr47;
            goto tr41;
        st25:
            if(++p == pe)
                goto _test_eof25;
        case 25:
            if((*p) == 10)
                goto tr43;
            goto tr49;
        tr45:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            goto st26;
        st26:
            if(++p == pe)
                goto _test_eof26;
        case 26:
            /* #line 336 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr13;
            case 13:
                goto st10;
            }
            if((*p) > 38)
            {
                if(40 <= (*p) && (*p) <= 126)
                    goto tr45;
            }
            else if((*p) >= 32)
                goto tr45;
            goto tr50;
        st10:
            if(++p == pe)
                goto _test_eof10;
        case 10:
            if((*p) == 10)
                goto tr13;
            goto tr12;
        tr46:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 26 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 6;
            }
            goto st27;
        st27:
            if(++p == pe)
                goto _test_eof27;
        case 27:
            /* #line 361 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr16;
            case 13:
                goto st12;
            case 32:
                goto st11;
            case 35:
                goto tr18;
            case 69:
                goto st13;
            }
            if(9 <= (*p) && (*p) <= 12)
                goto st11;
            goto tr49;
        st11:
            if(++p == pe)
                goto _test_eof11;
        case 11:
            switch((*p))
            {
            case 10:
                goto tr16;
            case 13:
                goto st12;
            case 32:
                goto st11;
            case 35:
                goto tr18;
            case 69:
                goto st13;
            }
            if(9 <= (*p) && (*p) <= 12)
                goto st11;
            goto tr14;
        st12:
            if(++p == pe)
                goto _test_eof12;
        case 12:
            if((*p) == 10)
                goto tr16;
            goto tr20;
        tr18:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 24 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 4;
            }
            goto st28;
        st28:
            if(++p == pe)
                goto _test_eof28;
        case 28:
            /* #line 400 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr16;
            case 13:
                goto st12;
            }
            if((*p) > 38)
            {
                if(40 <= (*p) && (*p) <= 126)
                    goto tr18;
            }
            else if((*p) >= 32)
                goto tr18;
            goto tr52;
        st13:
            if(++p == pe)
                goto _test_eof13;
        case 13:
            if((*p) == 79)
                goto st14;
            goto tr14;
        st14:
            if(++p == pe)
                goto _test_eof14;
        case 14:
            if((*p) == 70)
                goto tr16;
            goto tr14;
        tr47:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 23 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 3;
            }
            goto st29;
        tr55:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 21 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 1;
            }
            goto st29;
        st29:
            if(++p == pe)
                goto _test_eof29;
        case 29:
            /* #line 436 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 46:
                goto tr47;
            case 95:
                goto tr47;
            }
            if((*p) < 65)
            {
                if(48 <= (*p) && (*p) <= 57)
                    goto tr47;
            }
            else if((*p) > 90)
            {
                if(97 <= (*p) && (*p) <= 122)
                    goto tr47;
            }
            else
                goto tr47;
            goto tr20;
        st30:
            if(++p == pe)
                goto _test_eof30;
        case 30:
            switch((*p))
            {
            case 46:
                goto tr47;
            case 79:
                goto st31;
            case 95:
                goto tr47;
            }
            if((*p) < 65)
            {
                if(48 <= (*p) && (*p) <= 57)
                    goto tr47;
            }
            else if((*p) > 90)
            {
                if(97 <= (*p) && (*p) <= 122)
                    goto tr47;
            }
            else
                goto tr47;
            goto tr53;
        st31:
            if(++p == pe)
                goto _test_eof31;
        case 31:
            switch((*p))
            {
            case 46:
                goto tr47;
            case 70:
                goto tr55;
            case 95:
                goto tr47;
            }
            if((*p) < 65)
            {
                if(48 <= (*p) && (*p) <= 57)
                    goto tr47;
            }
            else if((*p) > 90)
            {
                if(97 <= (*p) && (*p) <= 122)
                    goto tr47;
            }
            else
                goto tr47;
            goto tr53;
        tr22:
            cs = 32;
            /* #line 31 "../src/grammar_lex/log_lexer.rl" */
            {
                {
                    p = ((te)) - 1;
                }
                {
                    push_token(log_tokens::NEWLINE);
                    ++n_seq_nl;
                    if(n_seq_nl > 1)
                    {
                        n_seq_nl = 0;
                        cs = 24;
                    }
                }
            }
            goto _again;
        tr23:
            cs = 32;
            /* #line 31 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::NEWLINE);
                    ++n_seq_nl;
                    if(n_seq_nl > 1)
                    {
                        n_seq_nl = 0;
                        cs = 24;
                    }
                }
            }
            goto _again;
        tr24:
            cs = 32;
            /* #line 1 "NONE" */
            {
                switch(act)
                {
                case 7: {
                    {
                        p = ((te)) - 1;
                    }
                    push_token(log_tokens::NEWLINE);
                    ++n_seq_nl;
                    if(n_seq_nl > 1)
                    {
                        n_seq_nl = 0;
                        cs = 24;
                    }
                }
                break;
                case 12: {
                    {
                        p = ((te)) - 1;
                    }
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
                break;
                }
            }
            goto _again;
        tr28:
            /* #line 35 "../src/grammar_lex/log_lexer.rl" */
            {
                {
                    p = ((te)) - 1;
                }
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st32;
        tr56:
            /* #line 35 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st32;
        tr57:
            /* #line 34 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                { }
            }
            goto st32;
        tr58:
            cs = 32;
            /* #line 30 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(log_tokens::NEWLINE);
                    ++n_seq_nl;
                    if(n_seq_nl > 1)
                    {
                        n_seq_nl = 0;
                        cs = 24;
                    }
                }
            }
            goto _again;
        tr61:
            /* #line 33 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p + 1;
                {
                    push_token(';');
                    n_seq_nl = 0;
                }
            }
            goto st32;
        tr64:
            /* #line 35 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    if(te == pe - 1)
                        push_token(-1);
                    else
                        push_token(*ts);
                }
            }
            goto st32;
        tr65:
            cs = 32;
            /* #line 31 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    push_token(log_tokens::NEWLINE);
                    ++n_seq_nl;
                    if(n_seq_nl > 1)
                    {
                        n_seq_nl = 0;
                        cs = 24;
                    }
                }
            }
            goto _again;
        tr67:
            /* #line 32 "../src/grammar_lex/log_lexer.rl" */
            {
                te = p;
                p--;
                {
                    push_token(log_tokens::IDENT_VALUE_PAIR);
                    n_seq_nl = 0;
                }
            }
            goto st32;
        st32:
            /* #line 1 "NONE" */
            {
                ts = 0;
            }
            if(++p == pe)
                goto _test_eof32;
        case 32:
            /* #line 1 "NONE" */
            {
                ts = p;
            }
            /* #line 537 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr58;
            case 13:
                goto st33;
            case 32:
                goto tr57;
            case 35:
                goto tr60;
            case 59:
                goto tr61;
            case 69:
                goto tr63;
            case 95:
                goto tr62;
            }
            if((*p) < 65)
            {
                if(9 <= (*p) && (*p) <= 12)
                    goto tr57;
            }
            else if((*p) > 90)
            {
                if(97 <= (*p) && (*p) <= 122)
                    goto tr62;
            }
            else
                goto tr62;
            goto tr56;
        st33:
            if(++p == pe)
                goto _test_eof33;
        case 33:
            if((*p) == 10)
                goto tr58;
            goto tr64;
        tr60:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            goto st34;
        st34:
            if(++p == pe)
                goto _test_eof34;
        case 34:
            /* #line 569 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 10:
                goto tr23;
            case 13:
                goto st15;
            }
            if((*p) > 38)
            {
                if(40 <= (*p) && (*p) <= 126)
                    goto tr60;
            }
            else if((*p) >= 32)
                goto tr60;
            goto tr65;
        st15:
            if(++p == pe)
                goto _test_eof15;
        case 15:
            if((*p) == 10)
                goto tr23;
            goto tr22;
        tr29:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 30 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 7;
            }
            goto st35;
        tr62:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 35 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 12;
            }
            goto st35;
        st35:
            if(++p == pe)
                goto _test_eof35;
        case 35:
            /* #line 598 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 9:
                goto st16;
            case 32:
                goto st16;
            case 46:
                goto st17;
            case 95:
                goto st17;
            }
            if((*p) < 48)
            {
                if(11 <= (*p) && (*p) <= 12)
                    goto st16;
            }
            else if((*p) > 57)
            {
                if((*p) > 90)
                {
                    if(97 <= (*p) && (*p) <= 122)
                        goto st17;
                }
                else if((*p) >= 65)
                    goto st17;
            }
            else
                goto st17;
            goto tr24;
        st16:
            if(++p == pe)
                goto _test_eof16;
        case 16:
            switch((*p))
            {
            case 9:
                goto st16;
            case 32:
                goto st16;
            }
            if((*p) < 33)
            {
                if(11 <= (*p) && (*p) <= 12)
                    goto st16;
            }
            else if((*p) > 34)
            {
                if((*p) > 58)
                {
                    if(60 <= (*p) && (*p) <= 126)
                        goto st36;
                }
                else if((*p) >= 36)
                    goto st36;
            }
            else
                goto st36;
            goto tr24;
        st36:
            if(++p == pe)
                goto _test_eof36;
        case 36:
            if((*p) < 36)
            {
                if(33 <= (*p) && (*p) <= 34)
                    goto st36;
            }
            else if((*p) > 58)
            {
                if(60 <= (*p) && (*p) <= 126)
                    goto st36;
            }
            else
                goto st36;
            goto tr67;
        st17:
            if(++p == pe)
                goto _test_eof17;
        case 17:
            switch((*p))
            {
            case 9:
                goto st16;
            case 32:
                goto st16;
            case 46:
                goto st17;
            case 95:
                goto st17;
            }
            if((*p) < 48)
            {
                if(11 <= (*p) && (*p) <= 12)
                    goto st16;
            }
            else if((*p) > 57)
            {
                if((*p) > 90)
                {
                    if(97 <= (*p) && (*p) <= 122)
                        goto st17;
                }
                else if((*p) >= 65)
                    goto st17;
            }
            else
                goto st17;
            goto tr24;
        tr63:
            /* #line 1 "NONE" */
            {
                te = p + 1;
            }
            /* #line 35 "../src/grammar_lex/log_lexer.rl" */
            {
                act = 12;
            }
            goto st37;
        st37:
            if(++p == pe)
                goto _test_eof37;
        case 37:
            /* #line 679 "../src/lexer//log_lexer.cc" */
            switch((*p))
            {
            case 9:
                goto st16;
            case 32:
                goto st16;
            case 46:
                goto st17;
            case 79:
                goto st18;
            case 95:
                goto st17;
            }
            if((*p) < 48)
            {
                if(11 <= (*p) && (*p) <= 12)
                    goto st16;
            }
            else if((*p) > 57)
            {
                if((*p) > 90)
                {
                    if(97 <= (*p) && (*p) <= 122)
                        goto st17;
                }
                else if((*p) >= 65)
                    goto st17;
            }
            else
                goto st17;
            goto tr64;
        st18:
            if(++p == pe)
                goto _test_eof18;
        case 18:
            switch((*p))
            {
            case 9:
                goto st16;
            case 32:
                goto st16;
            case 46:
                goto st17;
            case 70:
                goto tr29;
            case 95:
                goto st17;
            }
            if((*p) < 48)
            {
                if(11 <= (*p) && (*p) <= 12)
                    goto st16;
            }
            else if((*p) > 57)
            {
                if((*p) > 90)
                {
                    if(97 <= (*p) && (*p) <= 122)
                        goto st17;
                }
                else if((*p) >= 65)
                    goto st17;
            }
            else
                goto st17;
            goto tr28;
        }
    _test_eof19:
        cs = 19;
        goto _test_eof;
    _test_eof20:
        cs = 20;
        goto _test_eof;
    _test_eof21:
        cs = 21;
        goto _test_eof;
    _test_eof0:
        cs = 0;
        goto _test_eof;
    _test_eof22:
        cs = 22;
        goto _test_eof;
    _test_eof1:
        cs = 1;
        goto _test_eof;
    _test_eof2:
        cs = 2;
        goto _test_eof;
    _test_eof3:
        cs = 3;
        goto _test_eof;
    _test_eof4:
        cs = 4;
        goto _test_eof;
    _test_eof5:
        cs = 5;
        goto _test_eof;
    _test_eof6:
        cs = 6;
        goto _test_eof;
    _test_eof7:
        cs = 7;
        goto _test_eof;
    _test_eof8:
        cs = 8;
        goto _test_eof;
    _test_eof23:
        cs = 23;
        goto _test_eof;
    _test_eof9:
        cs = 9;
        goto _test_eof;
    _test_eof24:
        cs = 24;
        goto _test_eof;
    _test_eof25:
        cs = 25;
        goto _test_eof;
    _test_eof26:
        cs = 26;
        goto _test_eof;
    _test_eof10:
        cs = 10;
        goto _test_eof;
    _test_eof27:
        cs = 27;
        goto _test_eof;
    _test_eof11:
        cs = 11;
        goto _test_eof;
    _test_eof12:
        cs = 12;
        goto _test_eof;
    _test_eof28:
        cs = 28;
        goto _test_eof;
    _test_eof13:
        cs = 13;
        goto _test_eof;
    _test_eof14:
        cs = 14;
        goto _test_eof;
    _test_eof29:
        cs = 29;
        goto _test_eof;
    _test_eof30:
        cs = 30;
        goto _test_eof;
    _test_eof31:
        cs = 31;
        goto _test_eof;
    _test_eof32:
        cs = 32;
        goto _test_eof;
    _test_eof33:
        cs = 33;
        goto _test_eof;
    _test_eof34:
        cs = 34;
        goto _test_eof;
    _test_eof15:
        cs = 15;
        goto _test_eof;
    _test_eof35:
        cs = 35;
        goto _test_eof;
    _test_eof16:
        cs = 16;
        goto _test_eof;
    _test_eof36:
        cs = 36;
        goto _test_eof;
    _test_eof17:
        cs = 17;
        goto _test_eof;
    _test_eof37:
        cs = 37;
        goto _test_eof;
    _test_eof18:
        cs = 18;
        goto _test_eof;

    _test_eof: { }
        if(p == eof)
        {
            switch(cs)
            {
            case 20:
                goto tr36;
            case 21:
                goto tr37;
            case 0:
                goto tr0;
            case 22:
                goto tr36;
            case 1:
                goto tr2;
            case 2:
                goto tr2;
            case 3:
                goto tr2;
            case 4:
                goto tr2;
            case 5:
                goto tr2;
            case 6:
                goto tr2;
            case 7:
                goto tr2;
            case 8:
                goto tr2;
            case 23:
                goto tr36;
            case 9:
                goto tr2;
            case 25:
                goto tr49;
            case 26:
                goto tr50;
            case 10:
                goto tr12;
            case 27:
                goto tr49;
            case 11:
                goto tr14;
            case 12:
                goto tr20;
            case 28:
                goto tr52;
            case 13:
                goto tr14;
            case 14:
                goto tr14;
            case 29:
                goto tr20;
            case 30:
                goto tr53;
            case 31:
                goto tr53;
            case 33:
                goto tr64;
            case 34:
                goto tr65;
            case 15:
                goto tr22;
            case 35:
                goto tr24;
            case 16:
                goto tr24;
            case 36:
                goto tr67;
            case 17:
                goto tr24;
            case 37:
                goto tr64;
            case 18:
                goto tr28;
            }
        }
    }

    /* #line 60 "../src/grammar_lex/log_lexer.rl" */
}