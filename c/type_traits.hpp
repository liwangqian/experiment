/**
 *   @file type_traits.hpp   type traits for template proc
 *   @date 2017-02-05-13.29
 *
 *   COPYRIGHT
 *
 *   All contributions by liwangqian
 *   Copyright (c) 2017, liwangqian
 *   All rights reserved.
 *
 **/

#ifndef TYPE_TRAITS_HPP_INCLUDED
#define TYPE_TRAITS_HPP_INCLUDED


#include "meta.hpp"
#include <complex>

namespace snipts {


struct true_type
{
	static const bool value = true;
};

struct false_type
{
	static const bool value = false;
};

template <typename T1, typename T2>
struct is_same : public false_type {};

template <typename T>
struct is_same<T, T> : public true_type {};

//copy from c++11 possible implement
//@ref http://en.cppreference.com/w/cpp/types/is_pointer
template< typename T > struct remove_const          { typedef T type; };
template< typename T > struct remove_const<const T> { typedef T type; };

template< typename T > struct remove_volatile             { typedef T type; };
template< typename T > struct remove_volatile<volatile T> { typedef T type; };

template< typename T > struct remove_reference      { typedef T type; };
template< typename T > struct remove_reference<T&>  { typedef T type; };
template< typename T > struct remove_reference<T&&> { typedef T type; };

template< typename T >
struct remove_cv {
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

template <typename T>
struct is_integer
{
    typedef typename remove_cv<T>::type D;
    static const bool value = is_same<D, char>::value           ||
                              is_same<D, unsigned>::value       ||
                              is_same<D, short>::value          ||
                              is_same<D, unsigned>::value       ||
                              is_same<D, int>::value            ||
                              is_same<D, unsigned int>::value   ||
                              is_same<D, long>::value           ||
                              is_same<D, unsigned long>::value  ||
                              is_same<D, long long>::value      ||
                              is_same<D, unsigned long long>::value;
};

template <typename T>
struct is_float : public false_type {};

template <> struct is_float<float> : public true_type {};
template <> struct is_float<double> : public true_type {};
template <> struct is_float<long double> : public true_type {};

template <typename T>
struct is_numeric_complex : public false_type {};

template <typename eT>
struct is_numeric_complex< std::complex<eT> > :
	public if_< or_< is_integer<eT>, is_float<eT> >::value,
                true_type,
                false_type
              >::type {};


template <typename T>
struct is_numeric
{
    static const bool value = or_< or_<is_integer<T>, is_float<T> >,
                                   is_numeric_complex<T>
                                 >::value;
};


namespace detail {

template <typename T> struct is_pointer_helper     : public false_type {};
template <typename T> struct is_pointer_helper<T*> : public true_type {};

}

template <typename T>
struct is_pointer :
    public detail::is_pointer_helper<typename remove_cv<T>::type > {};




template <typename T>
struct is_string : public false_type {};

template <>
struct is_string<std::string> : public true_type {};
template <>
struct is_string<char*> : public true_type {};
template <>
struct is_string<const char*> : public true_type {};


} //namespace snipts


#endif //TYPE_TRAITS_HPP_INCLUDED
