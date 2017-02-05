/**
 *   @file functional.hpp
 *   @date 2017-01-29-10.33
 *
 *   COPYRIGHT
 *
 *   All contributions by liwangqian
 *   Copyright (c) 2017, liwangqian
 *   All rights reserved.
 *
 **/

#ifndef FUNCTIONAL_HPP_INCLUDED
#define FUNCTIONAL_HPP_INCLUDED

namespace snipts {


template <typename T, T t1, T... tn>
struct product
{
    static constexpr T value = t1 * product<T, tn...>::value;
};

template <typename T, T t1>
struct product<T, t1>
{
    static constexpr T value = t1;
};

}// namespace snipts

#endif // FUNCTIONAL_HPP_INCLUDED
