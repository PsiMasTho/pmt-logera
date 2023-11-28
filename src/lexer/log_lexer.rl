#include "log_lexer.h"
#include "tokens.h"

#include <algorithm>

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
        (nl)              { push_token(log_tokens::NEWLINE); ++m_n_seq_nl; if (m_n_seq_nl>1) { m_n_seq_nl = 0; fnext log_file_initial;}};
        (comment)         { push_token(log_tokens::NEWLINE); ++m_n_seq_nl; if (m_n_seq_nl>1) { m_n_seq_nl = 0; fnext log_file_initial;}};
        (ident_value_pair){ push_ident_value_pair(m_ts, m_te); m_n_seq_nl = 0;};
        (';')             { push_token(';'); m_n_seq_nl = 0;};
        (ws)              { };
        (any)             { push_token(*ts); m_n_seq_nl = 0;};
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
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write exec;
}

void log_lexer::push_ident_value_pair(char* start, char* end)
{
    // find first space
    auto space = std::find(start, end, ' ');
    
    // push ident
    push_token(log_tokens::IDENT, start, space);

    // skip any consecutive spaces
    auto next_char = std::find_if_not(space, end, [](char c) { return c == ' '; });
    
    // push attr_value
    push_token(log_tokens::ATTR_VALUE, next_char, end);
}