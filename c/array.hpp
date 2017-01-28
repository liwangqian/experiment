/**
 *   @file array.hpp   abstract data type for c++11
 *   @date 2017-01-28-20.21
 *
 *   COPYRIGHT
 *
 *   All contributions by liwangqian
 *   Copyright (c) 2017, liwangqian
 *   All rights reserved.
 *
 **/

#ifndef ARRAY_HPP_INCLUDED
#define ARRAY_HPP_INCLUDED

#include <cstddef>
#include <iomanip>
#include <random>

namespace snipts {

/**
    @class Array    static multidimensional array
 */
template <typename T, std::size_t D0, std::size_t... Dn>
class Array
{
public:
    typedef Array<T, Dn...> array_type;
    typedef T               value_type;

public:
    ~Array() = default;
    Array() = default;

    inline array_type& operator[](const std::size_t i)
    {
        return _d[i];
    }

    inline array_type operator[](const std::size_t i) const
    {
        return _d[i];
    }

    inline array_type& at(const std::size_t i) throw(std::out_of_range)
    {
        if (i >= D0)
        {
            throw std::out_of_range("Array::at -> Index out of range");
        }

        return _d[i];
    }

    inline array_type at(const std::size_t i) const throw(std::out_of_range)
    {
        if (i >= D0)
        {
            throw std::out_of_range("Array::at -> Index out of range");
        }

        return _d[i];
    }

    template <typename ... Idx>
    inline value_type& operator()(const std::size_t i, Idx... j)
    {
        static_assert(sizeof...(j) == sizeof...(Dn),
                      "Array::operator()(i,j,...) -> Index is mismatch with Array dimension");

        return _d[i](j...);
    }

    template <typename ... Idx>
    inline value_type operator()(const std::size_t i, Idx... j) const
    {
        static_assert(sizeof...(j) == sizeof...(Dn),
                      "Array::operator()(i,j,...) -> Index is mismatch with Array dimension");

        return _d[i](j...);
    }

    inline std::size_t size(const std::size_t dim = 1) const
    {
        return dim == 1 ? D0 : _d[0].size(dim - 1);
    }

    inline void fill(const value_type x)
    {
        for (std::size_t i = 0; i < D0; ++i)
        {
            _d[i].fill(x);
        }
    }


private:
    array_type _d[D0];
};

// specialize for one dimension array
template <typename T, std::size_t D0>
class Array<T, D0>
{
public:
    typedef T   value_type;

public:
    ~Array() = default;
    Array() = default;

    inline value_type& operator[](const std::size_t i)
    {
        return _d[i];
    }

    inline value_type operator[](const std::size_t i) const
    {
        return _d[i];
    }

    inline value_type& at(const std::size_t i) throw(std::out_of_range)
    {
        if (i >= D0)
        {
            throw std::out_of_range("Array::at -> Index out of range");
        }

        return _d[i];
    }

    inline value_type at(const std::size_t i) const throw(std::out_of_range)
    {
        if (i >= D0)
        {
            throw std::out_of_range("Array::at -> Index out of range");
        }

        return _d[i];
    }

    inline value_type& operator()(const std::size_t i)
    {
        return _d[i];
    }

    inline value_type operator()(const std::size_t i) const
    {
        return _d[i];
    }

    inline std::size_t size(const std::size_t dim = 1) const
    {
        return dim == 1 ? D0 : 0;
    }

    inline void fill(const value_type x)
    {
        for (std::size_t i = 0; i < D0; ++i)
        {
            _d[i] = x;
        }
    }

private:
    value_type _d[D0];
};

template <typename OS, typename T, std::size_t D0, std::size_t... Dn>
OS& operator << (OS& os, const Array<T, D0, Dn...>& m)
{
    const std::size_t ndim = sizeof...(Dn);

    //高维数组不支持打印
    static_assert(ndim <= 2, "Array::operator<<() -> "
                  "print array which dimension larger than 3 is not supported");

    for (std::size_t i = 0; i < m.size(1); ++i)
    {
        if (ndim == 2)
        {
            os << "\nArray(" << i << ",:,:) = \n";
        }
        os << std::setw(12) << m[i];
    }

    return os;
}

template <typename OS, typename T, std::size_t D0, std::size_t D1>
OS& operator << (OS& os, const Array<T, D0, D1>& m)
{
    for (std::size_t i = 0; i < m.size(1); ++i)
    {
        for (std::size_t j = 0; j < m.size(2); ++j)
        {
            os << std::setw(12) << m[i][j];
        }
        os << "\n";
    }

    return os;
}

template <typename T, std::size_t N>
inline void eye(Array<T, N, N>& m)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        m(i,i) = T(1);
    }
}


} //namespace snipts

#endif // ARRAY_HPP_INCLUDED
