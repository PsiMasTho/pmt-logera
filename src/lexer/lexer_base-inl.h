// clang-format off
#ifdef __INTELLISENSE__
    #include "lexer_base.h"
#endif
// clang-format on

template <typename derived>
lexer_base<derived>::lexer_base(buffer_t buffer)
    : m_result(std::move(buffer))
{
    m_p = m_result.get_buffer();
    m_pe = m_p + m_result.get_buffer_size();
    m_eof = m_pe;
    static_cast<derived*>(this)->init();
}

template <typename derived>
auto lexer_base<derived>::lex() -> i32
{
    i32 ret_code = 0;
    while(true)
    {
        static_cast<derived*>(this)->exec();

        if(m_cs == -1) // error state
        {
            ret_code = -1;
            break;
        }

        if(m_p == m_pe) // end of file (successful exit)
        {
            ret_code = 0;
            break;
        }
    }

    // push NULL token manually
    m_result.push_token_record('\0', m_result.get_buffer_size(), 1);
    return ret_code;
}

template <typename derived>
auto lexer_base<derived>::release_result() -> lexed_buffer
{
    return std::move(m_result);
}

template <typename derived>
void lexer_base<derived>::push_token(auto tok)
{
    push_token(tok, m_ts, m_te);
}

template <typename derived>
void lexer_base<derived>::push_token(auto tok, char* start, char* end)
{
    u32 const offset = static_cast<u32>(start - m_result.get_buffer());
    u16 const len = static_cast<u16>(end - start);
    tok_t const type = static_cast<tok_t>(tok);

    m_result.push_token_record(type, offset, len);
}