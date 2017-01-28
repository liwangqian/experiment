//util.h

#include <utility>
#include <cstddef>


namespace snipts {


class tokenizer
{
public:
    typedef std::pair<const char*, const char*> token;
    static const token invalid_token;

public:
    ~tokenizer();
    tokenizer(const char* content = NULL, const bool copy = true);

    void reset(const char* content, const bool copy = true);
    token next(const char delim = ' ');

    template<typename Delimiter>
    token next(const Delimiter& delim);

private:
    void strip(const char delim);

    template<typename Delimiter>
    void strip(const Delimiter& delim)
    {
        const char* end = _content + _size;
        while ( _pc != end )
        {
            if ( !delim(*_pc) )
            {
                break;
            }

            ++_pc; //skip the delimiter.
        }
    }

    //disable copy/assign constructor
    tokenizer(const tokenizer&);
    tokenizer& operator=(const tokenizer&);

private:
    const char* _content;
    const char* _pc;
    std::size_t _size;
          char* _inner_buffer;
};

class delimiter
{
public:
    ~delimiter();
    delimiter(const char* content = NULL, const bool copy = true);

    void reset(const char* content = NULL, const bool copy = true);
    bool operator()(const char c) const;

private:
    //disable copy/assign constructor
    delimiter(const delimiter&);
    delimiter& operator=(const delimiter&);

private:
    const char* _content;
    std::size_t _size;
          char* _inner_buffer;
};





/**
 *  implement for template functions
 *
 **/

template<typename Delimiter>
tokenizer::token tokenizer::next(const Delimiter& delim)
{
    const char* end = _content + _size;
    token atoken = invalid_token;

    strip(delim);

    if ( _pc == end )
    {
        return atoken;
    }

    atoken.first = _pc;

    while( _pc != end )
    {
        if ( delim(*_pc) )
        {
            break;
        }

        ++_pc;
    }

    atoken.second = _pc;

    return atoken;
}


} //namespace snipts


