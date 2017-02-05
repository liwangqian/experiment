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

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <utility>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <complex>

#include "type_traits.hpp"


namespace snipts {

namespace aux {

/**
 *  @class char_buffer    simple, shareable character buffer.
 *
 **/
template <const bool ReadOnly = true>
class char_buffer
{
public:
    typedef typename if_<ReadOnly, const char*, char*>::type pointer;

public:
    ~char_buffer()
    {
        if ( _inner_buffer )
        {
            delete[] _inner_buffer;
        }
    }

    char_buffer(pointer content, const bool copy)
    : _content(NULL), _inner_buffer(NULL), _size(0)
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

        _size = len;
    }

    void reset(pointer content, const bool copy)
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

        _size = len;
    }

    std::size_t size() const
    {
        return _size;
    }

    pointer _content;   //make public

private:
          char* _inner_buffer;
    std::size_t _size;
};

} //namespace aux

typedef aux::char_buffer<true>  readonly_buffer;
typedef aux::char_buffer<false> readwrite_buffer;

/**
 *  @class tokenizer    simple, text tokenize functionality
 *
 **/
class tokenizer
{
public:
    typedef std::pair<const char*, const char*> token;
    static const token invalid_token;

public:
    ~tokenizer();
    tokenizer(const char* content = NULL, const bool copy = true);

    void reset(const char* content = NULL, const bool copy = true);
    token next(const char delim = ' ');

    template<typename Delimiter>
    token next(const Delimiter& delim);

private:
    void strip(const char delim);

    template<typename Delimiter>
    void strip(const Delimiter& delim)
    {
        const char* end = _buf._content + _buf.size();
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
    readonly_buffer _buf;
    const char*     _pc;
};


/**
 *  @class delimiter    a class holding the delimiters used by tokenizer
 *
 **/
class delimiter
{
public:
    ~delimiter();
    delimiter(const char* content = NULL, const bool copy = true);

    void reset(const char* content, const bool copy = true);
    bool operator()(const char c) const;

private:
    //disable copy/assign constructor
    delimiter(const delimiter&);
    delimiter& operator=(const delimiter&);

private:
    readonly_buffer _buf;
};


/**
 *  @class formatter    a class help formatting strings
 *      cout << formatter("Write %1 %3 %2 %1").arg(12.3, "%2.3").arg("-").arg(10);
 **/
class formatter
{

private:
    struct arg_base
    {
        virtual const char* to_string() const = 0;
        virtual std::size_t size() const = 0;
        virtual ~arg_base() {}
    };

    template <typename NumericType>
    struct numeric_arg : public arg_base
    {
        explicit numeric_arg(const NumericType v, const char* format = NULL);
        explicit numeric_arg(const std::complex<NumericType>& v, const char* format = NULL);

        const char* to_string() const
        {
            return _str;
        }

        std::size_t size() const
        {
            return strlen(_str);
        }

    private:
        const char* guess_format(const NumericType v);
        const char* guess_format(const std::complex<NumericType>& v);

        char _str[64];
    };

    struct character_arg : public arg_base
    {
        //format is ignore
        explicit character_arg(const char* _content, const char* = NULL)
        : _str(_content)
        { }

        //format is ignore
        explicit character_arg(const std::string& _content, const char* = NULL)
        : _str(_content)
        { }

        const char* to_string() const
        {
            return _str.c_str();
        }

        std::size_t size() const
        {
            return _str.size();
        }

    private:
        std::string _str;
    };

    typedef std::vector<arg_base*> arg_holder;

public:
    ~formatter();
    explicit formatter(const char* format, const bool copy = false);

    void reset(const char* format = NULL, const bool copy = false);

    template <typename T>
    formatter& arg(T value, const char* format = NULL);

    const char* c_str();
    std::string str();

private:
    std::string parse_format_str();

    template <typename T>
    void arg_impl(T value, const char* format, true_type);

    template <typename T>
    void arg_impl(const std::complex<T>& value, const char* format, true_type);

    template <typename T>
    void arg_impl(T value, const char* format, false_type);

    readonly_buffer _buf;
    arg_holder      _args;
    std::string     _result;
};

template <typename OS>
OS& operator << (OS& os, const formatter& f)
{
    os << const_cast<formatter&>(f).c_str();
}

/**
 *  implement for template functions
 *
 **/

template<typename Delimiter>
tokenizer::token tokenizer::next(const Delimiter& delim)
{
    const char* end = _buf._content + _buf.size();
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



template <typename NumericType>
formatter::numeric_arg<NumericType>::numeric_arg(const NumericType v, const char* format)
{
    if ( format == NULL )
    {
        format = guess_format(v);
    }

    //!todo: validate format

    (void)snprintf(_str, sizeof(_str), format, v);
}

template <typename NumericType>
formatter::numeric_arg<NumericType>::numeric_arg(const std::complex<NumericType>& v, const char* format)
{
    if ( format == NULL )
    {
        format = guess_format(v);
    }

    //!todo: validate format

    (void)snprintf(_str, sizeof(_str), format, v.real(), v.imag());
}


template <typename NumericType>
const char* formatter::numeric_arg<NumericType>::guess_format(const NumericType v)
{
    const char* format = 0;

    if ( is_integer<NumericType>::value )
    {
        format = "%d";
    }
    else if ( is_float<NumericType>::value )
    {
        format = "%.6g";
    }
    else //pointer
    {
        format = "%p";
    }

    return format;
}

template <typename NumericType>
const char* formatter::numeric_arg<NumericType>::guess_format(const std::complex<NumericType>& v)
{
    const char* format = 0;

    if ( is_integer<NumericType>::value )
    {
        format = "(%d,%d)";
    }
    else
    {
        format = "(%.6g,%.6g)";
    }


    return format;
}

template <typename T>
formatter& formatter::arg(T value, const char* format)
{
    arg_impl(value, format,
             typename if_< is_string<T>::value,
                           false_type,
                           true_type
                         >::type()
             );

    return *this;
}

template <typename T>
void formatter::arg_impl(T value, const char* format, true_type)
{
    _args.push_back(new numeric_arg<T>(value, format));
}

template <typename T>
void formatter::arg_impl(const std::complex<T>& value, const char* format, true_type)
{
    _args.push_back(new numeric_arg<T>(value, format));
}

template <typename T>
void formatter::arg_impl(T value, const char* format, false_type)
{
    _args.push_back(new character_arg(value, format));
}

} //namespace snipts


#endif //UTIL_H_INCLUDED
