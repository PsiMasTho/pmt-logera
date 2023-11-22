#include "header_lexer.h"
#include "header_tokens.h"

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
        (nl)         { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (comment)    { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (ident)      { token(header_tokens::IDENT);                              fbreak;};
        (';')        { token(';');             fnext decl_attr_regex_seq; fbreak;};
        (ws)         { };
        (any)        { token(*ts);                                        fbreak;};
    *|;

    decl_attr_regex_seq := |*
        (nl)         { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (comment)    { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (regex)      { token(header_tokens::REGEX);                              fbreak;};
        (ws)         { };
        (any)        { token(*ts);                                        fbreak;};
    *|;

    decl_var_initial := |*
        (nl)         { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (comment)    { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (ident)      { token(header_tokens::IDENT);                              fbreak;};
        (';')        { token(';');             fnext decl_var_value_seq;  fbreak;};
        (ws)         { };
        (any)        { token(*ts);                                        fbreak;};
    *|;

    decl_var_value_seq := |*
        (nl)         { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (comment)    { token(header_tokens::NEWLINE); fnext main;                fbreak;};
        (ident)      { token(header_tokens::IDENT);                              fbreak;};
        (ws)         { };
        (any)        { token(*ts);                                        fbreak;};
    *|;
 
    main := |*
        (decl_var)   { token(header_tokens::DECL_VAR);  fnext decl_var_initial;  fbreak;};
        (decl_attr)  { token(header_tokens::DECL_ATTR); fnext decl_attr_initial; fbreak;};
        (nl)         { token(header_tokens::NEWLINE);                            fbreak;};
        (comment)    { token(header_tokens::NEWLINE);                            fbreak;};
        (ws)         { };
        (any)        { token(*ts);                                        fbreak;};
    *|;
}%%

%% write data;

void header_lexer::init(ragel_state& state)
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write init;
}

void header_lexer::exec(ragel_state& state)
{
    MAKE_RAGEL_STATE_AVAILABLE;
    %% write exec;
}
