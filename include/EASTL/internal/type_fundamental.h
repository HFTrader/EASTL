/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_INTERNAL_TYPE_FUNDAMENTAL_H
#define EASTL_INTERNAL_TYPE_FUNDAMENTAL_H


#include <EABase/eabase.h>
#include <EABase/nullptr.h>

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

namespace eastl
{


	///////////////////////////////////////////////////////////////////////
	// is_void
	//
	// is_void<T>::value == true if and only if T is one of the following types:
	//    [const][volatile] void
	//
	///////////////////////////////////////////////////////////////////////

	#define EASTL_TYPE_TRAIT_is_void_CONFORMANCE 1    // is_void is conforming.

	template <typename T> struct is_void : public false_type{};

	template <> struct is_void<void>                : public true_type{};
	template <> struct is_void<void const>          : public true_type{};
	template <> struct is_void<void volatile>       : public true_type{};
	template <> struct is_void<void const volatile> : public true_type{};



	///////////////////////////////////////////////////////////////////////
	// has_void_arg
	//
	// utility which identifies if any of the given template arguments is void.
	///////////////////////////////////////////////////////////////////////

	#if defined(EA_COMPILER_NO_VARIADIC_TEMPLATES)

		template <typename A0, typename A1 = int, typename A2 = int, typename A3 = int, typename A4 = int, typename A5 = int> 
		struct has_void_arg
			{ static const bool value = eastl::is_void<A0>::value ||    // If we add more arguments then change this 
										eastl::is_void<A1>::value ||    // to be a recursive template.
										eastl::is_void<A2>::value ||
										eastl::is_void<A3>::value ||
										eastl::is_void<A4>::value ||
										eastl::is_void<A5>::value;
			};
	#else
		template <typename ...Args> 
		struct has_void_arg;

		template <> 
		struct has_void_arg<> 
			: public eastl::false_type {};

		template <typename A0, typename ...Args>
		struct has_void_arg<A0, Args...>
			{ static const bool value = (eastl::is_void<A0>::value || eastl::has_void_arg<Args...>::value); };
	#endif


	///////////////////////////////////////////////////////////////////////
	// is_null_pointer
	//
	// C++14 type trait. Refers only to nullptr_t and not NULL (0).
	// eastl::is_null_pointer<nullptr>::value == true
	// eastl::is_null_pointer<std::nullptr_t>::value == true
	// eastl::is_null_pointer<void*>::value == false
	// eastl::is_null_pointer<NULL>::value == [cannot compile]
	//
	///////////////////////////////////////////////////////////////////////

	#if defined(EA_COMPILER_CPP11_ENABLED) && !defined(EA_COMPILER_NO_DECLTYPE) && !defined(_MSC_VER) // VC++'s handling of decltype(nullptr) is broken.
		#define EASTL_TYPE_TRAIT_is_null_pointer_CONFORMANCE 1

		template <typename T> 
		struct is_null_pointer : public eastl::is_same<typename eastl::remove_cv<T>::type, decltype(nullptr)> {}; // A C++11 compiler defines nullptr, but you need a C++11 standard library to declare std::nullptr_t. So it's safer to compare against decltype(nullptr) than to use std::nullptr_t, because we may have a C++11 compiler but C++98 library (happens with Apple frequently).
	#else
		#define EASTL_TYPE_TRAIT_is_null_pointer_CONFORMANCE 1

		template <typename T> 
		struct is_null_pointer : public eastl::is_same<typename eastl::remove_cv<T>::type, std::nullptr_t> {};
	#endif



	///////////////////////////////////////////////////////////////////////
	// is_integral
	//
	// is_integral<T>::value == true if and only if T  is one of the following types:
	//    [const] [volatile] bool
	//    [const] [volatile] char
	//    [const] [volatile] signed char
	//    [const] [volatile] unsigned char
	//    [const] [volatile] wchar_t
	//    [const] [volatile] short
	//    [const] [volatile] int
	//    [const] [volatile] long
	//    [const] [volatile] long long
	//    [const] [volatile] unsigned short
	//    [const] [volatile] unsigned int
	//    [const] [volatile] unsigned long
	//    [const] [volatile] unsigned long long
	//
	///////////////////////////////////////////////////////////////////////

	#define EASTL_TYPE_TRAIT_is_integral_CONFORMANCE 1    // is_integral is conforming.

	template <typename T> struct is_integral_helper : public false_type{};

	template <> struct is_integral_helper<unsigned char>      : public true_type{};
	template <> struct is_integral_helper<unsigned short>     : public true_type{};
	template <> struct is_integral_helper<unsigned int>       : public true_type{};
	template <> struct is_integral_helper<unsigned long>      : public true_type{};
	template <> struct is_integral_helper<unsigned long long> : public true_type{};

	template <> struct is_integral_helper<signed char>        : public true_type{};
	template <> struct is_integral_helper<signed short>       : public true_type{};
	template <> struct is_integral_helper<signed int>         : public true_type{};
	template <> struct is_integral_helper<signed long>        : public true_type{};
	template <> struct is_integral_helper<signed long long>   : public true_type{};

	template <> struct is_integral_helper<bool>               : public true_type{};
	template <> struct is_integral_helper<char>               : public true_type{};
	#ifndef EA_WCHAR_T_NON_NATIVE // If wchar_t is a native type instead of simply a define to an existing type which is already handled above...
		template <> struct is_integral_helper<wchar_t>        : public true_type{};
	#endif

	template <typename T>
	struct is_integral : public eastl::is_integral_helper<typename eastl::remove_cv<T>::type>{};

	#define EASTL_DECLARE_INTEGRAL(T)                                             \
	namespace eastl{                                                              \
		template <> struct is_integral<T>                : public true_type{};    \
		template <> struct is_integral<const T>          : public true_type{};    \
		template <> struct is_integral<volatile T>       : public true_type{};    \
		template <> struct is_integral<const volatile T> : public true_type{};    \
	}



	///////////////////////////////////////////////////////////////////////
	// is_floating_point
	//
	// is_floating_point<T>::value == true if and only if T is one of the following types:
	//    [const] [volatile] float
	//    [const] [volatile] double
	//    [const] [volatile] long double
	//
	///////////////////////////////////////////////////////////////////////

	#define EASTL_TYPE_TRAIT_is_floating_point_CONFORMANCE 1    // is_floating_point is conforming.

	template <typename T> struct is_floating_point_helper : public false_type{};

	template <> struct is_floating_point_helper<float>       : public true_type{};
	template <> struct is_floating_point_helper<double>      : public true_type{};
	template <> struct is_floating_point_helper<long double> : public true_type{};

	template <typename T>
	struct is_floating_point : public eastl::is_floating_point_helper<typename eastl::remove_cv<T>::type>{};

	#define EASTL_DECLARE_FLOATING_POINT(T)                                             \
	namespace eastl{                                                                    \
		template <> struct is_floating_point<T>                : public true_type{};    \
		template <> struct is_floating_point<const T>          : public true_type{};    \
		template <> struct is_floating_point<volatile T>       : public true_type{};    \
		template <> struct is_floating_point<const volatile T> : public true_type{};    \
	}



	///////////////////////////////////////////////////////////////////////
	// is_arithmetic
	//
	// is_arithmetic<T>::value == true if and only if:
	//    is_floating_point<T>::value == true, or
	//    is_integral<T>::value == true
	//
	///////////////////////////////////////////////////////////////////////

	#define EASTL_TYPE_TRAIT_is_arithmetic_CONFORMANCE 1    // is_arithmetic is conforming.

	template <typename T> 
	struct is_arithmetic : public integral_constant<bool,
		is_integral<T>::value || is_floating_point<T>::value
	>{};



	///////////////////////////////////////////////////////////////////////
	// is_fundamental
	//
	// is_fundamental<T>::value == true if and only if:
	//    is_floating_point<T>::value == true, or
	//    is_integral<T>::value == true, or
	//    is_void<T>::value == true
	//    is_null_pointer<T>::value == true
	///////////////////////////////////////////////////////////////////////

	#define EASTL_TYPE_TRAIT_is_fundamental_CONFORMANCE 1    // is_fundamental is conforming.

	template <typename T> 
	struct is_fundamental : public integral_constant<bool,
		is_void<T>::value || is_integral<T>::value || is_floating_point<T>::value || is_null_pointer<T>::value
	>{};


} // namespace eastl


#endif // Header include guard





















