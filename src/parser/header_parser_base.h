// Generated by Bisonc++ V6.06.00 on Mon, 27 Nov 2023 02:10:05 +0100

// hdr/includes
#ifndef header_parser_base_h_included
#define header_parser_base_h_included

#include <exception>
#include <vector>
#include <iostream>
// $insert polyincludes
#include <memory>
// $insert preincludes
#include "header_parser_base_preinclude.h"
#include "../lexer/tokens.h"

// hdr/baseclass

namespace // anonymous
{
    struct PI_;
}


// $insert polymorphic
enum class Tag_
{
    REGEX_NODE,
    IDENTIFIER_NODE,
    DECL_ATTR_NODE,
    DECL_VAR_NODE,
    STATEMENT_NODE,
    END_TAG_
};

class EndPolyType_
{};

namespace Meta_
{

extern size_t const *t_nErrors;

extern size_t const *s_nErrors_;

template <typename Tp_>
struct TagOf;

template <Tag_ tag>
struct TypeOf;

// $insert polymorphicSpecializations

extern char const *idOfTag_[];
template <>
struct TagOf<header_regex_node>
{
    static Tag_ const tag = Tag_::REGEX_NODE;
};

template <>
struct TagOf<header_identifier_node>
{
    static Tag_ const tag = Tag_::IDENTIFIER_NODE;
};

template <>
struct TagOf<header_decl_attr_node>
{
    static Tag_ const tag = Tag_::DECL_ATTR_NODE;
};

template <>
struct TagOf<header_decl_var_node>
{
    static Tag_ const tag = Tag_::DECL_VAR_NODE;
};

template <>
struct TagOf<header_statement_node>
{
    static Tag_ const tag = Tag_::STATEMENT_NODE;
};

template <>
struct TagOf<EndPolyType_>
{
    static Tag_ const tag = Tag_::END_TAG_;
};

template <>
struct TypeOf<Tag_::REGEX_NODE>
{
    using type = header_regex_node;
};

template <>
struct TypeOf<Tag_::IDENTIFIER_NODE>
{
    using type = header_identifier_node;
};

template <>
struct TypeOf<Tag_::DECL_ATTR_NODE>
{
    using type = header_decl_attr_node;
};

template <>
struct TypeOf<Tag_::DECL_VAR_NODE>
{
    using type = header_decl_var_node;
};

template <>
struct TypeOf<Tag_::STATEMENT_NODE>
{
    using type = header_statement_node;
};

template <>
struct TypeOf<Tag_::END_TAG_>
{
    using type = EndPolyType_;
};

    // Individual semantic value classes are derived from Base, offering a
    // member returning the value's Tag_, a member cloning the object of its
    // derived Semantic<Tag_> and a member returning a pointerr to its
    // derived Semantic<Tag_> data. See also Bisonc++'s distribution file
    // README.polymorphic-techical
class Base
{
    protected:
        Tag_ d_baseTag;        // d_baseTag is assigned by Semantic.

    public:
        Base() = default;
        Base(Base const &other) = delete;

        virtual ~Base();

        Tag_ tag() const;
        Base *clone() const;
        void *data() const;        

    private:
        virtual Base *vClone() const = 0;
        virtual void *vData() const = 0;
};

inline Base *Base::clone() const
{
    return vClone();
}

inline void *Base::data() const
{
    return vData();
}

inline Tag_ Base::tag() const
{
    return d_baseTag;
}

    // The class Semantic stores a semantic value of the type matching tg_
template <Tag_ tg_>
class Semantic: public Base
{
    typename TypeOf<tg_>::type d_data;
    
    public:
        Semantic();
        Semantic(Semantic<tg_> const &other);   // req'd for cloning

            // This constructor member template forwards its arguments to
            // d_data, allowing it to be initialized using whatever
            // constructor is available for DataType
        template <typename ...Params>
        Semantic(Params &&...params);

    private:
        Base *vClone() const override;
        void *vData() const override;
};

template <Tag_ tg_>
Semantic<tg_>::Semantic()
{
    d_baseTag = tg_;                // Base's data member:
}

template <Tag_ tg_>
Semantic<tg_>::Semantic(Semantic<tg_> const &other)
:
    d_data(other.d_data)
{
    d_baseTag = other.d_baseTag;
}

template <Tag_ tg_>
template <typename ...Params>
Semantic<tg_>::Semantic(Params &&...params)
:
    d_data(std::forward<Params>(params) ...)
{
    d_baseTag = tg_;
}


template <Tag_ tg_>
Base *Semantic<tg_>::vClone() const
{
    return new Semantic<tg_>{*this};
}

template <Tag_ tg_>
void *Semantic<tg_>::vData() const 
{
    return const_cast<typename TypeOf<tg_>::type *>(&d_data);
}


    // The class SType wraps a pointer to Base.  It becomes the polymorphic
    // STYPE_ type. It also defines get members, allowing constructions like
    // $$.get<INT> to be used.  
class SType: private std::unique_ptr<Base>
{
    using BasePtr = std::unique_ptr<Base>;

    public:
        SType();
        SType(SType const &other);
        SType(SType &&tmp);

        ~SType() = default;

            // Specific overloads are needed for SType = SType assignments
        SType &operator=(SType const &rhs);
        SType &operator=(SType &rhs);           // required so it is used
                                                // instead of the template op=
        SType &operator=(SType &&tmp);

// $insert polymorphicOpAssignDecl
        SType &operator=(header_regex_node const &value);
        SType &operator=(header_regex_node &&tmp);

        SType &operator=(header_identifier_node const &value);
        SType &operator=(header_identifier_node &&tmp);

        SType &operator=(header_decl_attr_node const &value);
        SType &operator=(header_decl_attr_node &&tmp);

        SType &operator=(header_decl_var_node const &value);
        SType &operator=(header_decl_var_node &&tmp);

        SType &operator=(header_statement_node const &value);
        SType &operator=(header_statement_node &&tmp);

        SType &operator=(EndPolyType_ const &value);
        SType &operator=(EndPolyType_ &&tmp);

        template <Tag_ tagParam, typename ...Args>
        void assign(Args &&...args);
    
            // By default the get()-members check whether the specified <tag>
            // matches the tag returned by SType::tag (d_data's tag). If they
            // don't match a run-time fatal error results.
        template <Tag_ tag>
        typename TypeOf<tag>::type &get();

        template <Tag_ tag>
        typename TypeOf<tag>::type const &get() const;

        Tag_ tag() const;
};

inline SType::SType()
:
    std::unique_ptr<Base>(new Semantic<Tag_::END_TAG_>{})
{}

inline SType::SType(SType const &other)
:
    BasePtr{ other->clone() }
{}

inline SType::SType(SType &&tmp)
:
    BasePtr{ std::move(tmp) }
{}

inline SType &SType::operator=(SType const &rhs)
{
    reset(rhs->clone());
    return *this;
}

inline SType &SType::operator=(SType &rhs)
{
    reset(rhs->clone());
    return *this;
}

inline SType &SType::operator=(SType &&tmp)
{
    BasePtr::operator=(std::move(tmp));
    return *this;
}

// $insert polymorphicOpAssignImpl
inline SType &SType::operator=(header_regex_node const &value)
{
    assign< Tag_::REGEX_NODE >(value);
    return *this;
}
inline SType &SType::operator=(header_regex_node &&tmp)
{
    assign< Tag_::REGEX_NODE >(std::move(tmp));
    return *this;
}
inline SType &SType::operator=(header_identifier_node const &value)
{
    assign< Tag_::IDENTIFIER_NODE >(value);
    return *this;
}
inline SType &SType::operator=(header_identifier_node &&tmp)
{
    assign< Tag_::IDENTIFIER_NODE >(std::move(tmp));
    return *this;
}
inline SType &SType::operator=(header_decl_attr_node const &value)
{
    assign< Tag_::DECL_ATTR_NODE >(value);
    return *this;
}
inline SType &SType::operator=(header_decl_attr_node &&tmp)
{
    assign< Tag_::DECL_ATTR_NODE >(std::move(tmp));
    return *this;
}
inline SType &SType::operator=(header_decl_var_node const &value)
{
    assign< Tag_::DECL_VAR_NODE >(value);
    return *this;
}
inline SType &SType::operator=(header_decl_var_node &&tmp)
{
    assign< Tag_::DECL_VAR_NODE >(std::move(tmp));
    return *this;
}
inline SType &SType::operator=(header_statement_node const &value)
{
    assign< Tag_::STATEMENT_NODE >(value);
    return *this;
}
inline SType &SType::operator=(header_statement_node &&tmp)
{
    assign< Tag_::STATEMENT_NODE >(std::move(tmp));
    return *this;
}
inline SType &SType::operator=(EndPolyType_ const &value)
{
    assign< Tag_::END_TAG_ >(value);
    return *this;
}
inline SType &SType::operator=(EndPolyType_ &&tmp)
{
    assign< Tag_::END_TAG_ >(std::move(tmp));
    return *this;
}

template <Tag_ tagParam, typename ...Args>
void SType::assign(Args &&...args)
{
    reset(new Semantic<tagParam>(std::forward<Args>(args) ...));
}

template <Tag_ tg>
typename TypeOf<tg>::type &SType::get()
{
// $insert warnTagMismatches

    if (tag() != tg)
    {
        if (*t_nErrors != 0)
            const_cast<SType *>(this)->assign<tg>();
        else
        {
            std::cerr << "[Fatal] calling `.get<Tag_::" << 
                idOfTag_[static_cast<int>(tg)] << 
                ">()', but Tag " <<
                idOfTag_[static_cast<int>(tag())] << " is encountered. Try "
                "option --debug and call setDebug(Parser::ACTIONCASES)\n";
            throw 1;        // ABORTs
        }
    }

    return *static_cast<typename TypeOf<tg>::type *>( (*this)->data() );
}

template <Tag_ tg>
typename TypeOf<tg>::type const &SType::get() const
{
// $insert warnTagMismatches

    if (tag() != tg)
    {
        if (*t_nErrors != 0)
            const_cast<SType *>(this)->assign<tg>();
        else
        {
            std::cerr << "[Fatal] calling `.get<Tag_::" << 
                idOfTag_[static_cast<int>(tg)] << 
                ">()', but Tag " <<
                idOfTag_[static_cast<int>(tag())] << " is encountered. Try "
                "option --debug and call setDebug(Parser::ACTIONCASES)\n";
            throw 1;        // ABORTs
        }
    }

    return *static_cast<typename TypeOf<tg>::type *>( (*this)->data() );
}

inline Tag_ SType::tag() const
{
    return (*this)->tag();
}

}  // namespace Meta_

// $insert parserbase
class header_parser_base: public header_tokens
{
    public:
        enum DebugMode_
        {
            OFF           = 0,
            ON            = 1 << 0,
            ACTIONCASES   = 1 << 1
        };

// $insert tokens
// $insert STYPE
    using STYPE_ = Meta_::SType;


    private:
                        // state  semval
        using StatePair = std::pair<size_t, STYPE_>;
                       // token   semval
        using TokenPair = std::pair<int,    STYPE_>;

        int d_stackIdx = -1;
        std::vector<StatePair> d_stateStack;
        StatePair  *d_vsp = 0;       // points to the topmost value stack
        size_t      d_state = 0;

        TokenPair   d_next;
        int         d_token;

        bool        d_terminalToken = false;
        bool        d_recovery = false;


    protected:
        enum Return_
        {
            PARSE_ACCEPT_ = 0,   // values used as parse()'s return values
            PARSE_ABORT_  = 1
        };
        enum ErrorRecovery_
        {
            UNEXPECTED_TOKEN_,
        };

        bool        d_actionCases_ = false;    // set by options/directives
        bool        d_debug_ = true;
        size_t      d_requiredTokens_;
        size_t      d_nErrors_;                // initialized by clearin()
        size_t      d_acceptedTokens_;
        STYPE_     d_val_;


        header_parser_base();

        void ABORT() const;
        void ACCEPT() const;
        void ERROR() const;

        STYPE_ &vs_(int idx);             // value stack element idx 
        int  lookup_() const;
        int  savedToken_() const;
        int  token_() const;
        size_t stackSize_() const;
        size_t state_() const;
        size_t top_() const;
        void clearin_();
        void errorVerbose_();
        void lex_(int token);
        void popToken_();
        void pop_(size_t count = 1);
        void pushToken_(int token);
        void push_(size_t nextState);
        void redoToken_();
        bool recovery_() const;
        void reduce_(int rule);
        void shift_(int state);
        void startRecovery_();

    public:
        void setDebug(bool mode);
        void setDebug(DebugMode_ mode);
}; 

// hdr/abort
inline void header_parser_base::ABORT() const
{
    throw PARSE_ABORT_;
}

// hdr/accept
inline void header_parser_base::ACCEPT() const
{
    throw PARSE_ACCEPT_;
}


// hdr/error
inline void header_parser_base::ERROR() const
{
    throw UNEXPECTED_TOKEN_;
}

// hdr/savedtoken
inline int header_parser_base::savedToken_() const
{
    return d_next.first;
}

// hdr/opbitand
inline header_parser_base::DebugMode_ operator&(header_parser_base::DebugMode_ lhs,
                                     header_parser_base::DebugMode_ rhs)
{
    return static_cast<header_parser_base::DebugMode_>(
            static_cast<int>(lhs) & rhs);
}

// hdr/opbitor
inline header_parser_base::DebugMode_ operator|(header_parser_base::DebugMode_ lhs, 
                                     header_parser_base::DebugMode_ rhs)
{
    return static_cast<header_parser_base::DebugMode_>(static_cast<int>(lhs) | rhs);
};

// hdr/recovery
inline bool header_parser_base::recovery_() const
{
    return d_recovery;
}

// hdr/stacksize
inline size_t header_parser_base::stackSize_() const
{
    return d_stackIdx + 1;
}

// hdr/state
inline size_t header_parser_base::state_() const
{
    return d_state;
}

// hdr/token
inline int header_parser_base::token_() const
{
    return d_token;
}

// hdr/vs
inline header_parser_base::STYPE_ &header_parser_base::vs_(int idx) 
{
    return (d_vsp + idx)->second;
}



#endif



