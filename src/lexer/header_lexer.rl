#include "header_lexer.h"
#include "tokens.h"

%%{
    machine header_lexer;

    alpha_u    = alpha | '_';
    alnum_u    = alnum | '_';

    ws         = space - ['\n''\r'];
    comment    = '#' + (print - '\n')* '\n';
    decl_var   = ">var";
    decl_attr  = ">attr";
    ident      = alpha_u (alnum_u | '.') *;
    nl         = '\r'?'\n';
    regex      = (print - (space | ';' | '#'))+;

    decl_attr_initial := |*
        (nl)         { push_token(header_tokens::NEWLINE);   fnext main;};
        (comment)    { push_token(header_tokens::NEWLINE);   fnext main;};
        (ident)      { push_token(header_tokens::IDENT);};
        (';')        { push_token(';');                      fnext decl_attr_regex_seq;};
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;

    decl_attr_regex_seq := |*
        (nl)         { push_token(header_tokens::NEWLINE);   fnext main;};
        (comment)    { push_token(header_tokens::NEWLINE);   fnext main;};
        (regex)      { push_token(header_tokens::REGEX);};
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;

    decl_var_initial := |*
        (nl)         { push_token(header_tokens::NEWLINE);   fnext main;};
        (comment)    { push_token(header_tokens::NEWLINE);   fnext main;};
        (ident)      { push_token(header_tokens::IDENT);};
        (';')        { push_token(';');                      fnext decl_var_value_seq;};
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;

    decl_var_value_seq := |*
        (nl)         { push_token(header_tokens::NEWLINE);   fnext main;};
        (comment)    { push_token(header_tokens::NEWLINE);   fnext main;};
        (ident)      { push_token(header_tokens::IDENT);};
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;
 
    main := |*
        (decl_var)   { push_token(header_tokens::DECL_VAR);  fnext decl_var_initial;};
        (decl_attr)  { push_token(header_tokens::DECL_ATTR); fnext decl_attr_initial;};
        (nl)         { push_token(header_tokens::NEWLINE);};
        (comment)    { push_token(header_tokens::NEWLINE);};
        (ws)         { };
        (any)        { push_token(*ts);};
    *|;
}%%

%% write data;

void header_lexer::init()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write init;
}

void header_lexer::exec()
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write exec;
}
