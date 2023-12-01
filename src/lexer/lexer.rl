#include "lexer.h"
#include "tokens.h"

#include <algorithm>

%%{
    machine lexer;

    alpha_u          = alpha | '_';
    alnum_u          = alnum | '_';
    ws               = space - ['\n''\r'];
    nl               = ('\r'?'\n') | 'EOF';
    comment          = '#' + (print - ['\n''\r'])* nl?;
    kw_var           = "var";
    kw_attr          = "attr";
    ident            = alpha_u (alnum_u | '.') *;
    regex            = (print - (space | ';' | '#'| '|' | '&'))+;
    date             = digit{4} '-' digit{2} '-' digit{2};
    attr_value       = (print - (space | ';' | '#' | '|' | '&'))+;
    ident_value_pair = ident ws + attr_value;

    decl_attr_initial := |*
        (ident)      { push_token(tokens::IDENT); fnext decl_attr_regex_seq;};
        (';')        { push_token(';');           fnext main;};
        (nl)         { };
        (comment)    { };
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;

    decl_attr_regex_seq := |*
        (regex)      { push_token(tokens::REGEX);};
        ('&')        { push_token('&');};
        ('|')        { push_token('|');};
        (';')        { push_token(';'); fnext main;};
        (nl)         { };
        (comment)    { };
        (ws)         { };
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;

    decl_var_value_seq := |*
        (ident)      { push_token(tokens::IDENT);};
        ('&')        { push_token('&');};
        ('|')        { push_token('|');};
        (';')        { push_token(';'); fnext main;};
        (nl)         { };
        (comment)    { };
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;

    attr_val_seq := |*
        (ident_value_pair){ push_ident_value_pair(m_ts, m_te);};
        ('&')             { push_token('&');};
        ('|')             { push_token('|');};
        (';')             { push_token(';'); fnext main;};
        (nl)              { };
        (comment)         { };
        (ws)              { };
        (any)             { push_token(*ts);};
    *|;
 
    main := |*
        (date)       { push_token(tokens::DATE); };
        ('>' kw_var) { auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::DECL_VAR);  --ts; fnext decl_var_value_seq;};
        ('>' kw_attr){ auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::DECL_ATTR); --ts; fnext decl_attr_initial;};
        ('>' ident)  { auto tmp = te; te = ts + 1; push_token('>'); ++ts; te = tmp; push_token(tokens::IDENT);     --ts; fnext attr_val_seq;};
        (nl)         { };
        (comment)    { };
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;
}%%

%% write data;

void lexer::init()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write init;
}

void lexer::exec()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write exec;
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
