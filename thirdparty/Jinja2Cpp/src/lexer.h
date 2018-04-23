#ifndef LEXER_H
#define LEXER_H

#include "lexertk.h"
#include "value.h"

#include <functional>

namespace jinja2
{
struct CharRange
{
    size_t startOffset;
    size_t endOffset;
    auto size() const {return endOffset - startOffset;}
};

struct Token
{
    enum Type
    {
        Unknown,
        Eof = 256,

        // General
        Identifier,
        IntegerNum,
        FloatNum,
        String,

        // Operators
        Equal,
        NotEqual,
        LessEqual,
        GreaterEqual,
        StarStar,
        DashDash,
        LogicalOr,
        LogicalAnd,
        LogicalNot,
        MulMul,
        DivDiv,
        True,
        False,
        None,
        In,
        Is,

        // Keywords
        For,
        Endfor,
        If,
        Else,
        ElIf,
        EndIf,
        Block,
        EndBlock,
        Extends,
        Macro,
        EndMacro,
        Call,
        EndCall,
        Filter,
        EndFilter,
        Set,
        EndSet,
        Include,
        Import,
    };

    Type type;
    CharRange range;
    Value value;

    bool IsEof() const
    {
        return type == Eof;
    }

    bool operator == (char ch) const
    {
        return type == ch;
    }

    bool operator == (Type t) const
    {
        return type == t;
    }

    template<typename T>
    bool operator != (T v) const
    {
        return !(*this == v);
    }
};

struct LexerHelper
{
    virtual std::string GetAsString(const CharRange& range) = 0;
    virtual Value GetAsValue(const CharRange& range, Token::Type type) = 0;
    virtual Token::Type GetKeyword(const CharRange& range) = 0;
    virtual char GetCharAt(size_t pos) = 0;
};

class Lexer
{
public:
    using TokensList = std::vector<Token>;
    Lexer(std::function<lexertk::token ()> tokenizer, LexerHelper* helper)
        : m_tokenizer(std::move(tokenizer))
        , m_helper(helper)
    {
    }

    bool Preprocess();
    const TokensList& GetTokens() const
    {
        return m_tokens;
    }

private:
    bool ProcessNumber(const lexertk::token& token, Token& newToken);
    bool ProcessSymbolOrKeyword(const lexertk::token& token, Token& newToken);
    bool ProcessString(const lexertk::token& token, Token& newToken);
private:
    std::function<lexertk::token ()> m_tokenizer;
    TokensList m_tokens;
    LexerHelper* m_helper;
};

class LexScanner
{
public:
    struct State
    {
        Lexer::TokensList::const_iterator m_begin;
        Lexer::TokensList::const_iterator m_end;
        Lexer::TokensList::const_iterator m_cur;
    };

    struct StateSaver
    {
        StateSaver(LexScanner& scanner)
            : m_state(scanner.m_state)
            , m_scanner(scanner)
        {
        }

        ~StateSaver()
        {
            if (!m_commited)
                m_scanner.m_state = m_state;
        }

        void Commit()
        {
            m_commited = true;
        }

        State m_state;
        LexScanner& m_scanner;
        bool m_commited = false;
    };

    LexScanner(const Lexer& lexer)
    {
        m_state.m_begin = lexer.GetTokens().begin();
        m_state.m_end = lexer.GetTokens().end();
        Reset();
    }

    void Reset()
    {
        m_state.m_cur = m_state.m_begin;
    }

    auto GetState() const
    {
        return m_state;
    }

    void RestoreState(const State& state)
    {
        m_state = state;
    }

    const Token& NextToken()
    {
        if (m_state.m_cur == m_state.m_end)
            return EofToken();

        return *m_state.m_cur ++;
    }

    void EatToken()
    {
        if (m_state.m_cur != m_state.m_end)
            ++ m_state.m_cur;
    }

    void ReturnToken()
    {
        if (m_state.m_cur != m_state.m_begin)
            -- m_state.m_cur;
    }

    const Token& PeekNextToken() const
    {
        if (m_state.m_cur == m_state.m_end)
            return EofToken();

        return *m_state.m_cur;
    }

private:
    State m_state;
    static const Token& EofToken()
    {
        static Token eof;
        eof.type = Token::Eof;
        return eof;
    }
};

} // jinja2

#endif // LEXER_H
