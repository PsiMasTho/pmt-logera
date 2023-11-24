// Generated by Bisonc++ V6.06.00 on Fri, 24 Nov 2023 01:50:05 +0100

// base/comment

// $insert class.ih
#include "log_parser.h"

// The FIRST element of SR arrays shown below uses `d_type', defining the
// state's type, and `d_lastIdx' containing the last element's index. If
// d_lastIdx contains the REQ_TOKEN bitflag (see below) then the state needs
// a token: if in this state d_token is Reserved_::UNDETERMINED_, nextToken() will be
// called

// The LAST element of SR arrays uses `d_token' containing the last retrieved
// token to speed up the (linear) seach.  Except for the first element of SR
// arrays, the field `d_action' is used to determine what to do next. If
// positive, it represents the next state (used with SHIFT); if zero, it
// indicates `ACCEPT', if negative, -d_action represents the number of the
// rule to reduce to.

// `lookup()' tries to find d_token in the current SR array. If it fails, and
// there is no default reduction UNEXPECTED_TOKEN_ is thrown, which is then
// caught by the error-recovery function.

// The error-recovery function will pop elements off the stack until a state
// having bit flag ERR_ITEM is found. This state has a transition on errTok_
// which is applied. In this errTok_ state, while the current token is not a
// proper continuation, new tokens are obtained by nextToken(). If such a
// token is found, error recovery is successful and the token is
// handled according to the error state's SR table and parsing continues.
// During error recovery semantic actions are ignored.

// A state flagged with the DEF_RED flag will perform a default
// reduction if no other continuations are available for the current token.

// The ACCEPT STATE never shows a default reduction: when it is reached the
// parser returns ACCEPT(). During the grammar
// analysis phase a default reduction may have been defined, but it is
// removed during the state-definition phase.

// So:
//      s_x[] = 
//      {
//                  [_field_1_]         [_field_2_]
//
// First element:   {state-type,        idx of last element},
// Other elements:  {required token,    action to perform},
//                                      ( < 0: reduce, 
//                                          0: ACCEPT,
//                                        > 0: next state)
//      }

// base/declarations

namespace // anonymous
{
    char const author[] = "Frank B. Brokken (f.b.brokken@rug.nl)";

    enum Reserved_
    {
        UNDETERMINED_   = -2,
        EOF_            = -1,
        errTok_         = 256
    };
    enum StateType       // modify statetype/data.cc when this enum changes
    {
        NORMAL,
        ERR_ITEM,
        REQ_TOKEN,
        ERR_REQ,    // ERR_ITEM | REQ_TOKEN
        DEF_RED,    // state having default reduction
        ERR_DEF,    // ERR_ITEM | DEF_RED
        REQ_DEF,    // REQ_TOKEN | DEF_RED
        ERR_REQ_DEF // ERR_ITEM | REQ_TOKEN | DEF_RED
    };    
    inline bool operator&(StateType lhs, StateType rhs)
    {
        return (static_cast<int>(lhs) & rhs) != 0;
    }
    enum StateTransition
    {
        ACCEPT_   = 0,     // `ACCEPT' TRANSITION
    };

    struct PI_     // Production Info
    {
        size_t d_nonTerm; // identification number of this production's
                            // non-terminal 
        size_t d_size;    // number of elements in this production 
    };

    struct SR_     // Shift Reduce info, see its description above
    {
        union
        {
            int _field_1_;      // initializer, allowing initializations 
                                // of the SR s_[] arrays
            StateType d_type;
            int       d_token;
        };
        union
        {
            int _field_2_;

            int d_lastIdx;          // if negative, the state uses SHIFT
            int d_action;           // may be negative (reduce), 
                                    // postive (shift), or 0 (accept)
        };
    };

    // $insert staticdata
    
    enum                        // size to expand the state-stack with when
    {                           // full
        STACK_EXPANSION_ = 10
    };

// Productions Info Records:
PI_ const s_productionInfo[] = 
{
     {0, 0}, // not used: reduction values are negative
     {267, 0}, // 1: input ->  <empty>
     {267, 2}, // 2: input ->  input statement
     {268, 2}, // 3: statement (NEWLINE) ->  date NEWLINE
     {268, 2}, // 4: statement (EOF_) ->  date EOF_
     {268, 1}, // 5: statement ->  variable
     {268, 2}, // 6: statement (NEWLINE) ->  ident_value_pair_list NEWLINE
     {268, 2}, // 7: statement (EOF_) ->  ident_value_pair_list EOF_
     {268, 1}, // 8: statement (NEWLINE) ->  NEWLINE
     {268, 1}, // 9: statement (EOF_) ->  EOF_
     {269, 2}, // 10: ident_value_pair_list (';') ->  ident_value_pair ';'
     {269, 3}, // 11: ident_value_pair_list (';') ->  ident_value_pair_list ident_value_pair ';'
     {270, 2}, // 12: variable (':') ->  identifier ':'
     {271, 1}, // 13: identifier (IDENT) ->  IDENT
     {272, 1}, // 14: date (DATE) ->  DATE
     {273, 1}, // 15: ident_value_pair (IDENT_VALUE_PAIR) ->  IDENT_VALUE_PAIR
     {274, 1}, // 16: input_$ ->  input
};

// State info and SR_ transitions for each state.


SR_ s_0[] =
{
    { { DEF_RED}, {  2} },         
    { {     267}, {  1} }, // input
    { {       0}, { -1} },         
};

SR_ s_1[] =
{
    { { REQ_TOKEN}, {      13} },                         
    { {       268}, {       2} }, // statement            
    { {       272}, {       3} }, // date                 
    { {       270}, {       4} }, // variable             
    { {       269}, {       5} }, // ident_value_pair_list
    { {       258}, {       6} }, // NEWLINE              
    { {      EOF_}, {       7} }, // EOF_                 
    { {       259}, {       8} }, // DATE                 
    { {       271}, {       9} }, // identifier           
    { {       273}, {      10} }, // ident_value_pair     
    { {       257}, {      11} }, // IDENT                
    { {       260}, {      12} }, // IDENT_VALUE_PAIR     
    { {      EOF_}, { ACCEPT_} },                         
    { {         0}, {       0} },                         
};

SR_ s_2[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -2} }, 
};

SR_ s_3[] =
{
    { { REQ_TOKEN}, {  3} },           
    { {       258}, { 13} }, // NEWLINE
    { {      EOF_}, { 14} }, // EOF_   
    { {         0}, {  0} },           
};

SR_ s_4[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -5} }, 
};

SR_ s_5[] =
{
    { { REQ_TOKEN}, {  5} },                    
    { {       258}, { 15} }, // NEWLINE         
    { {      EOF_}, { 16} }, // EOF_            
    { {       273}, { 17} }, // ident_value_pair
    { {       260}, { 12} }, // IDENT_VALUE_PAIR
    { {         0}, {  0} },                    
};

SR_ s_6[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -8} }, 
};

SR_ s_7[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -9} }, 
};

SR_ s_8[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -14} }, 
};

SR_ s_9[] =
{
    { { REQ_TOKEN}, {  2} },       
    { {        58}, { 18} }, // ':'
    { {         0}, {  0} },       
};

SR_ s_10[] =
{
    { { REQ_TOKEN}, {  2} },       
    { {        59}, { 19} }, // ';'
    { {         0}, {  0} },       
};

SR_ s_11[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -13} }, 
};

SR_ s_12[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -15} }, 
};

SR_ s_13[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -3} }, 
};

SR_ s_14[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -4} }, 
};

SR_ s_15[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -6} }, 
};

SR_ s_16[] =
{
    { { DEF_RED}, {  1} }, 
    { {       0}, { -7} }, 
};

SR_ s_17[] =
{
    { { REQ_TOKEN}, {  2} },       
    { {        59}, { 20} }, // ';'
    { {         0}, {  0} },       
};

SR_ s_18[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -12} }, 
};

SR_ s_19[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -10} }, 
};

SR_ s_20[] =
{
    { { DEF_RED}, {   1} }, 
    { {       0}, { -11} }, 
};


// State array:
SR_ *s_state[] =
{
  s_0,  s_1,  s_2,  s_3,  s_4,  s_5,  s_6,  s_7,  s_8,  s_9,
  s_10,  s_11,  s_12,  s_13,  s_14,  s_15,  s_16,  s_17,  s_18,  s_19,
  s_20,
};

} // anonymous namespace ends



// $insert polymorphicCode
namespace Meta_
{

size_t const *t_nErrors;
// $insert idoftag
char const *idOfTag_[] = {
    "IDENT_VALUE_PAIR_LIST_NODE",
    "LOG_FILE_NODE",
    "IDENT_VALUE_PAIR_NODE",
    "DATE_NODE",
    "VARIABLE_NODE",
    "OFFSET_T",
    "EndPolyType_"
};

size_t const *s_nErrors_;

Base::~Base()
{}

}   // namespace Meta_

// If the parsing function call (i.e., parse()' needs arguments, then provide
// an overloaded function.  The code below doesn't rely on parameters, so no
// arguments are required.  Furthermore, parse uses a function try block to
// allow us to do ACCEPT and ABORT from anywhere, even from within members
// called by actions, simply throwing the appropriate exceptions.


// base/base1
log_parser_base::log_parser_base()
:
    d_token(Reserved_::UNDETERMINED_),
    // $insert baseclasscode
    d_requiredTokens_(0)
{
    Meta_::t_nErrors = &d_nErrors_;
}

// base/clearin
void log_parser_base::clearin_()
{
    d_nErrors_ = 0;
    d_stackIdx = -1;
    d_stateStack.clear();
    d_token = Reserved_::UNDETERMINED_;
    d_next = TokenPair{ Reserved_::UNDETERMINED_, STYPE_{} };
    d_recovery = false;
    d_acceptedTokens_ = d_requiredTokens_;
    d_val_ = STYPE_{};

    push_(0);
}

// base/debugfunctions

void log_parser_base::setDebug(bool mode)
{
    d_actionCases_ = false;
    d_debug_ = mode;
}

void log_parser_base::setDebug(DebugMode_ mode)
{
    d_actionCases_ = mode & ACTIONCASES;
    d_debug_ =       mode & ON;
}

// base/lex
void log_parser_base::lex_(int token)
{
    d_token = token;

    if (d_token <= 0)
        d_token = Reserved_::EOF_;

    d_terminalToken = true;
}

// base/lookup
int log_parser_base::lookup_() const
{
    // if the final transition is negative, then we should reduce by the rule
    // given by its positive value.

    SR_ const *sr = s_state[d_state];
    SR_ const *last = sr + sr->d_lastIdx;

    for ( ; ++sr != last; )           // visit all but the last SR entries
    {
        if (sr->d_token == d_token)
            return sr->d_action;
    }

    if (sr == last)   // reached the last element
    {
        if (sr->d_action < 0)   // default reduction
        {
            return sr->d_action;                
        }

        // No default reduction, so token not found, so error.
        throw UNEXPECTED_TOKEN_;
    }

    // not at the last element: inspect the nature of the action
    // (< 0: reduce, 0: ACCEPT, > 0: shift)

    int action = sr->d_action;


    return action;
}

// base/pop
void log_parser_base::pop_(size_t count)
{
    if (d_stackIdx < static_cast<int>(count))
    {
        ABORT();
    }

    d_stackIdx -= count;
    d_state = d_stateStack[d_stackIdx].first;
    d_vsp = &d_stateStack[d_stackIdx];

}

// base/poptoken
void log_parser_base::popToken_()
{
    d_token = d_next.first;
    d_val_ = std::move(d_next.second);

    d_next.first = Reserved_::UNDETERMINED_;
}

// base/push
void log_parser_base::push_(size_t state)
{
    size_t currentSize = d_stateStack.size();
    if (stackSize_() == currentSize)
    {
        size_t newSize = currentSize + STACK_EXPANSION_;
        d_stateStack.resize(newSize);
    }

    ++d_stackIdx;
    d_stateStack[d_stackIdx] = 
                    StatePair{ d_state = state, std::move(d_val_) };

    d_vsp = &d_stateStack[d_stackIdx];

    if (d_stackIdx == 0)
    {
    }
    else
    {
    }
}

// base/pushtoken
void log_parser_base::pushToken_(int token)
{
    d_next = TokenPair{ d_token, std::move(d_val_) };
    d_token = token;
}

// base/redotoken
void log_parser_base::redoToken_()
{
    if (d_token != Reserved_::UNDETERMINED_)
        pushToken_(d_token);
}

// base/reduce
void log_parser_base::reduce_(int rule)
{
    PI_ const &pi = s_productionInfo[rule];

    d_token = pi.d_nonTerm;
    pop_(pi.d_size);

    d_terminalToken = false;
}

// base/shift
void log_parser_base::shift_(int action)
{
    push_(action);
    popToken_();               // token processed

    if (d_recovery and d_terminalToken)
    {
        d_recovery = false;
        d_acceptedTokens_ = 0;
    }
}

// base/startrecovery
void log_parser_base::startRecovery_()
{
    int lastToken = d_token;                // give the unexpected token a
                                            // chance to be processed
                                            // again.

    pushToken_(Reserved_::errTok_);      // specify errTok_ as next token
    push_(lookup_());                     // push the error state

    d_token = lastToken;                    // reactivate the unexpected
                                            // token (we're now in an
                                            // ERROR state).

    d_recovery = true;
}

// base/top
inline size_t log_parser_base::top_() const
{
    return d_stateStack[d_stackIdx].first;
}

// derived/errorrecovery
void log_parser::errorRecovery_()
{
    // When an error has occurred, pop elements off the stack until the top
    // state has an error-item. If none is found, the default recovery
    // mode (which is to abort) is activated. 
    //
    // If EOF is encountered without being appropriate for the current state,
    // then the error recovery will fall back to the default recovery mode.
    // (i.e., parsing terminates)



    if (d_acceptedTokens_ >= d_requiredTokens_)// only generate an error-
    {                                           // message if enough tokens 
        ++d_nErrors_;                          // were accepted. Otherwise
        error();                                // simply skip input
    }

    // get the error state
    while (not (s_state[top_()][0].d_type & ERR_ITEM))
    {
        pop_();
    }

    // In the error state, looking up a token allows us to proceed.
    // Continuation may be require multiple reductions, but eventually a
    // terminal-token shift is used. See nextCycle_ for details.

    startRecovery_();
}

// derived/executeaction
void log_parser::executeAction_(int production)
try
{
    if (token_() != Reserved_::UNDETERMINED_)
        pushToken_(token_());     // save an already available token
    switch (production)
    {
        // $insert actioncases
        
        case 2:
        {
         
         if (std::holds_alternative<newline_node>(vs_(0).get<Tag_::LOG_FILE_NODE>()) || std::holds_alternative<eof_node>(vs_(0).get<Tag_::LOG_FILE_NODE>()))
         return;
         m_ast.m_nodes.push_back(vs_(0).get<Tag_::LOG_FILE_NODE>());
         }
        break;

        case 3:
        {
         log_file_node const lfn = date_node{vs_(-1).get<Tag_::DATE_NODE>()};
         d_val_ = lfn;
         }
        break;

        case 4:
        {
         log_file_node const lfn = date_node{vs_(-1).get<Tag_::DATE_NODE>()};
         d_val_ = lfn;
         }
        break;

        case 5:
        {
         log_file_node const lfn = variable_node{vs_(0).get<Tag_::OFFSET_T>()};
         d_val_ = lfn;
         }
        break;

        case 6:
        {
         log_file_node const lfn = ident_value_pair_list_node{vs_(-1).get<Tag_::IDENT_VALUE_PAIR_LIST_NODE>()};
         d_val_ = lfn;
         }
        break;

        case 7:
        {
         log_file_node const lfn = ident_value_pair_list_node{vs_(-1).get<Tag_::IDENT_VALUE_PAIR_LIST_NODE>()};
         d_val_ = lfn;
         }
        break;

        case 8:
        {
         auto const offset = m_walker.get_cur_token_record_offset();
         log_file_node const lfn = newline_node{offset};
         d_val_ = lfn;
         }
        break;

        case 9:
        {
         auto const offset = m_walker.get_cur_token_record_offset();
         log_file_node const lfn = eof_node{offset};
         d_val_ = lfn;
         ACCEPT();
         }
        break;

        case 10:
        {
         d_val_ = ident_value_pair_list_node{};
         d_val_.get<Tag_::IDENT_VALUE_PAIR_LIST_NODE>().m_pairs.push_back(vs_(-1).get<Tag_::IDENT_VALUE_PAIR_NODE>());
         }
        break;

        case 11:
        {
         d_val_ = vs_(-2).get<Tag_::IDENT_VALUE_PAIR_LIST_NODE>();
         d_val_.get<Tag_::IDENT_VALUE_PAIR_LIST_NODE>().m_pairs.push_back(vs_(-1).get<Tag_::IDENT_VALUE_PAIR_NODE>());
         }
        break;

        case 12:
        {
         d_val_ = vs_(-1).get<Tag_::OFFSET_T>();
         }
        break;

        case 13:
        {
         auto const offset = m_walker.get_cur_token_record_offset();
         d_val_ = offset;
         }
        break;

        case 14:
        {
         auto const offset = m_walker.get_cur_token_record_offset();
         d_val_ = date_node{offset};
         }
        break;

        case 15:
        {
         auto const offset = m_walker.get_cur_token_record_offset();
         d_val_ = ident_value_pair_node{offset};
         }
        break;

    }
}
catch (std::exception const &exc)
{
    exceptionHandler(exc);
}

// derived/nextcycle
void log_parser::nextCycle_()
try
{
    if (s_state[state_()]->d_type & REQ_TOKEN)
        nextToken_();              // obtain next token


    int action = lookup_();        // lookup d_token in d_state

    if (action > 0)                 // SHIFT: push a new state
    {
        shift_(action);
        return;
    }

    if (action < 0)            // REDUCE: execute and pop.
    {

        if (recovery_())
            redoToken_();
        else
            executeAction_(-action);
                                            // next token is the rule's LHS
        reduce_(-action); 
        return;
    }

    if (recovery_())
        ABORT();
    else 
        ACCEPT();
}
catch (ErrorRecovery_)
{
    if (not recovery_())
        errorRecovery_();
    else
    {
        if (token_() == Reserved_::EOF_)
            ABORT();
        popToken_();               // skip the failing token
    }
}


// derived/nexttoken
void log_parser::nextToken_()
{ 
    // If d_token is Reserved_::UNDETERMINED_ then if savedToken_() is
    // Reserved_::UNDETERMINED_ another token is obtained from lex(). Then
    // savedToken_() is assigned to d_token.

                                    // no need for a token: got one already
    if (token_() != Reserved_::UNDETERMINED_) 
    {
        return;                             
    }

    if (savedToken_() != Reserved_::UNDETERMINED_)
    {
        popToken_();               // consume pending token
    }
    else
    {
        ++d_acceptedTokens_;       // accept another token (see
                                    // errorRecover())
        lex_(lex());
        print_();
    }
    print();
}

// derived/print
void log_parser::print_()
{
// $insert print
}

// derived/parse
int log_parser::parse()
try 
{
    // The parsing algorithm:
    // Initially, state 0 is pushed on the stack, and all relevant variables
    // are initialized by Base::clearin_.
    //
    // Then, in an eternal loop:
    //
    //  1. If a state is a REQ_TOKEN type, then the next token is obtained
    //     from nextToken().  This may very well be the currently available
    //     token. When retrieving a terminal token d_terminal is set to true.
    //
    //  2. lookup() is called, d_token is looked up in the current state's
    //     SR_ array.
    //
    //  4. Depending on the result of the lookup() function the next state is
    //     shifted on the parser's stack, a reduction by some rule is applied,
    //     or the parsing function returns ACCEPT(). When a reduction is
    //     called for, any action that may have been defined for that
    //     reduction is executed.
    //
    //  5. An error occurs if d_token is not found, and the state has no
    //     default reduction.

    clearin_();                            // initialize, push(0)

    while (true)
    {
// $insert prompt
        nextCycle_();
    }
}
catch (Return_ retValue)
{
    return retValue or d_nErrors_;
}


// derived/tail


