/**
 *   @file util.hpp   utility class/functions
 *   @date 2017-02-05-13.29
 *
 *   COPYRIGHT
 *
 *   All contributions by liwangqian
 *   Copyright (c) 2017, liwangqian
 *   All rights reserved.
 *
 **/

#include "util.h"
#include <cstring>
#include <algorithm>
#include <iostream>

namespace snipts {




/**
 * tokenizer
 *
 **/

const tokenizer::token tokenizer::invalid_token =
                std::make_pair<const char*, const char*>(NULL, NULL);

tokenizer::~tokenizer()
{
    //nothing todo
}

tokenizer::tokenizer(const char* content, const bool copy)
: _buf(content, copy), _pc(NULL)
{
    _pc = _buf._content;
}

void tokenizer::reset(const char* content, const bool copy)
{
    if ( content == NULL )
    {
        _pc = _buf._content;
        return;
    }

    _buf.reset(content, copy);

    _pc = _buf._content;
}

tokenizer::token tokenizer::next(const char delim)
{
    const char* end = _buf._content + _buf.size();
    token atoken = invalid_token;

    strip(delim); // skip the delimiters

    if ( _pc == end )
    {
        return atoken;
    }

    atoken.first = _pc;

    while( _pc != end )
    {
        if ( *_pc == delim )
        {
            break;
        }

        ++_pc;
    }

    atoken.second = _pc;

    return atoken;
}

void tokenizer::strip(const char delim)
{
    const char* end = _buf._content + _buf.size();
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
{ }

delimiter::delimiter(const char* content, const bool copy)
: _buf(content, copy)
{ }

void delimiter::reset(const char* content, const bool copy)
{
    _buf.reset(content, copy);
}

bool delimiter::operator()(const char c) const
{
    for (std::size_t i = 0; i < _buf.size(); ++i)
    {
        if ( _buf._content[i] == c )
        {
            return true;
        }
    }

    return false;
}


formatter::~formatter()
{
    for (std::size_t i = 0; i < _args.size(); ++i)
    {
        if ( _args[i] )
            delete _args[i];
    }
}

formatter::formatter(const char* format, const bool copy)
: _buf(format, copy)
{ }

void formatter::reset(const char* format, const bool copy)
{
    _buf.reset(format, copy);
}

std::string formatter::parse_format_str()
{
    const std::size_t nargs = _args.size();
    std::size_t         cnt = 0;
    int                 idx = 0;
    std::string result;

    for (std::size_t i = 0; i < _buf.size() - 1; ++i)
    {
        idx = -1;
        if ( _buf._content[i] == '%' && isdigit(_buf._content[i+1]))
        {
            cnt++;
            idx = atoi(_buf._content+i+1);
            while ( isdigit(_buf._content[++i]) ) /*nop*/ ;

            --i;
        }

        if ( idx >= 1 && idx <= nargs )
        {
 //           std::cout << _args[idx-1]->to_string() << std::endl;
            result.append(_args[idx-1]->to_string());
        }
        else
        {
            result.push_back(_buf._content[i]);
        }

    }

    return result;
}

std::string formatter::str()
{
    if ( _result.empty() )
        _result = parse_format_str();

    return _result;
}

const char* formatter::c_str()
{
    if ( _result.empty() )
        _result = parse_format_str();

    return _result.c_str();
}

} //namespace snipts
