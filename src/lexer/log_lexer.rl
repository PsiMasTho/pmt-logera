#include "log_lexer.h"
#include "tokens.h"

%%{
    machine log_lexer;

    alpha_u    = alpha | '_';
    alnum_u    = alnum | '_';

    ws               = space - ['\n''\r'];
    ident            = alpha_u ((alnum_u | '.') - '#')*;
    nl               = ('\r'?'\n') | 'EOF';
    comment          = '#' + (print - ['\n''\r'])* nl?;
    blank_line       = ws* (comment | nl);
    last_colon       = ':' blank_line;
    date             = digit{4} '-' digit{2} '-' digit{2};
    attr_value       = (print - (space | ';' | '#'))+;
    ident_value_pair = ident ws + attr_value;

    log_file_initial := |*
        (nl)              { push_token(log_tokens::NEWLINE);};
        (comment)         { push_token(log_tokens::NEWLINE);};
        (ident)           { push_token(log_tokens::IDENT);};
        (last_colon)      { push_token(':'); fnext logfile_attr_val_seq;};
        (ws)              { };
        (any)             { push_token(*ts);};
    *|;

    logfile_attr_val_seq := |*
        (nl)              { push_token(log_tokens::NEWLINE); ++n_seq_nl; if (n_seq_nl>1) { n_seq_nl = 0; fnext log_file_initial;}};
        (comment)         { push_token(log_tokens::NEWLINE); ++n_seq_nl; if (n_seq_nl>1) { n_seq_nl = 0; fnext log_file_initial;}};
        (ident_value_pair){ push_token(log_tokens::IDENT_VALUE_PAIR); n_seq_nl = 0;};
        (';')             { push_token(';'); n_seq_nl = 0;};
        (ws)              { };
        (any)             { push_token(*ts); n_seq_nl = 0;};
    *|;
 
    main := |*
        (date)            { push_token(log_tokens::DATE); fnext log_file_initial;};
        (nl)              { push_token(log_tokens::NEWLINE);};
        (comment)         { push_token(log_tokens::NEWLINE);};
        (ws)              { };
        (any)             { push_token(*ts);};
    *|;
}%%

%% write data;

void log_lexer::init()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write init;
}

void log_lexer::exec()
{
    static int n_seq_nl = 0;
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write exec;
}