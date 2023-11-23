// clang-format off
#ifdef __INTELLISENSE__
    #include "lexer_base.h"
#endif
// clang-format on

template <typename derived>
lexer_base<derived>::lexer_base(char const* infile)
:   m_result(infile)
{
    m_p = m_result.get_buffer();
    m_pe = m_p + m_result.get_buffer_size();
    m_eof = m_pe;
    static_cast<derived*>(this)->init();
}

template <typename derived>
auto lexer_base<derived>::lex() -> int
{
    while (true)
    {
        static_cast<derived*>(this)->exec();

        if (m_cs == -1) // error state
            return 1;

        if (m_p == m_pe) // end of file (successful exit)
            return 0;
    }
}

template <typename derived>
auto lexer_base<derived>::release_result() -> lexed_file
{
    return std::move(m_result);
}

template <typename derived>
void lexer_base<derived>::push_token(auto tok)
{
    uint32_t const offset = static_cast<uint32_t>(m_ts - m_result.get_buffer());
    uint16_t const len = static_cast<uint16_t>(m_te - m_ts);
    underlying_token_t const type = static_cast<underlying_token_t>(tok);

    m_result.push_token(type, offset, len);
}