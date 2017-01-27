//util.cpp

#include "util.h"
#include <cstring>
#include <algorithm>

namespace snipts {


/**
 * tokenizer
 *
 **/

const tokenizer::word tokenizer::invalid_word =
                std::make_pair<const char*, const char*>(NULL, NULL);

tokenizer::~tokenizer()
{
    if ( _inner_buffer )
    {
        delete[] _inner_buffer;
    }
}

tokenizer::tokenizer(const char* content, const bool copy)
: _content(NULL), _pc(NULL), _size(0), _inner_buffer(NULL)
{
    const std::size_t len = strlen(content);

    if ( copy )
    {
        _inner_buffer = new char[len];
        std::copy(content, content + len, _inner_buffer);
        _content = _inner_buffer;
    }
    else
    {
        _content = content;
    }

    _pc     = _content;
    _size   = len;
}

void tokenizer::reset(const char* content, const bool copy)
{
    const std::size_t len = strlen(content);

    if ( copy )
    {
        if ( len > _size || _inner_buffer == NULL)
        {
            delete[] _inner_buffer; //delete null is ok.
            _inner_buffer = new char[len];
            _content = _inner_buffer;
        }

        std::copy(content, content + len, _inner_buffer);
    }
    else
    {
        _content = content;
    }

    _pc     = _content;
    _size   = len;
}

tokenizer::word tokenizer::next(const char delim)
{
    const char* end = _content + _size;
    word aword = invalid_word;

    strip(delim); // skip the delimiters

    if ( _pc == end )
    {
        return aword;
    }

    aword.first = _pc;

    while( _pc != end )
    {
        if ( *_pc == delim )
        {
            break;
        }

        ++_pc;
    }

    aword.second = _pc;

    return aword;
}

void tokenizer::strip(const char delim)
{
    const char* end = _content + _size;
    while ( _pc != end )
    {
        if ( *_pc != delim )
        {
            break;
        }

        ++_pc; //skip the delimiter.
    }
}


/**
 * delimiter
 *
 **/

delimiter::~delimiter()
{
    if ( _inner_buffer )
    {
        delete[] _inner_buffer;
    }
}

delimiter::delimiter(const char* content, const bool copy)
: _content(NULL), _size(0), _inner_buffer(NULL)
{
    _size = strlen(content);

    if ( copy )
    {
        _inner_buffer = new char[_size];
        std::copy(content, content + _size, _inner_buffer);
        _content = _inner_buffer;
    }
    else
    {
        _content = content;
    }
}

void delimiter::reset(const char* content, const bool copy)
{
    const std::size_t len = strlen(content);

    if ( copy )
    {
        if ( len > _size || _inner_buffer == NULL)
        {
            delete[] _inner_buffer; //delete null is ok.
            _inner_buffer = new char[len];
            _content = _inner_buffer;
        }

        std::copy(content, content + len, _inner_buffer);
    }
    else
    {
        _content = content;
    }

    _size   = len;
}

bool delimiter::operator()(const char c) const
{
    for (std::size_t i = 0; i < _size; ++i)
    {
        if ( _content[i] == c )
        {
            return true;
        }
    }

    return false;
}


} //namespace snipts
