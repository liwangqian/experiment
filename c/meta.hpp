/**
 *   @file meta.hpp   meta programming facility
 *   @date 2017-02-05-14.36
 *
 *   COPYRIGHT
 *
 *   All contributions by liwangqian
 *   Copyright (c) 2017, liwangqian
 *   All rights reserved.
 *
 **/

#ifndef META_HPP_INCLUDED
#define META_HPP_INCLUDED

namespace snipts {


template <bool cond, typename T1, typename T2>
struct if_
{
	typedef T2	type;
};

template <typename T1, typename T2>
struct if_<true, T1, T2>
{
	typedef T1	type;
};

//and is keyword of the c++
template <typename T1, typename T2>
struct and_
{
	static const bool value = T1::value && T2::value;
};

//or is keyword of the c++
template <typename T1, typename T2>
struct or_
{
	static const bool value = T1::value || T2::value;
};


template <bool, typename T> struct enable_if {};

template <typename T> struct enable_if<true, T> { typedef T type; };


} //namespace snipts

#endif //META_HPP_INCLUDED