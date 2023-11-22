#include "log_lexer.h"
#include "log_tokens.h"

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
        (nl)              { token(log_tokens::NEWLINE);                             fbreak;};
        (comment)         { token(log_tokens::NEWLINE);                             fbreak;};
        (ident)           { token(log_tokens::IDENT);                               fbreak;};
        (last_colon)      { token(':');                 fnext logfile_attr_val_seq; fbreak;};
        (ws)              { };
        (any)             { token(*ts);                                             fbreak;};
    *|;

    logfile_attr_val_seq := |*
        (nl)              { token(log_tokens::NEWLINE); ++n_seq_nl; if (n_seq_nl>1) { n_seq_nl = 0; fnext log_file_initial;} fbreak;};
        (comment)         { token(log_tokens::NEWLINE); ++n_seq_nl; if (n_seq_nl>1) { n_seq_nl = 0; fnext log_file_initial;} fbreak;};
        (ident_value_pair){ token(log_tokens::IDENT_VALUE_PAIR); n_seq_nl = 0;      fbreak;};
        (';')             { token(';'); n_seq_nl = 0;                               fbreak;};
        (ws)              { };
        (any)             { token(*ts); n_seq_nl = 0;                               fbreak;};
    *|;
 
    main := |*
        (date)            { token(log_tokens::DATE);      fnext log_file_initial;  fbreak;};
        (nl)              { token(log_tokens::NEWLINE);                            fbreak;};
        (comment)         { token(log_tokens::NEWLINE);                            fbreak;};
        (ws)              { };
        (any)             { token(*ts);                                            fbreak;};
    *|;
}%%

%% write data;

void log_lexer::init(ragel_state& state)
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write init;
}

void log_lexer::exec(ragel_state& state)
{
    static int n_seq_nl = 0;
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write exec;
}