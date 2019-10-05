/**
 * @addtogroup psl
 * @{
 * @file type_traits.h
 */

#pragma once

namespace psl {

template <typename T, T V>
struct integral_constant {
  using value_type = T;
  using type = integral_constant;

  static constexpr value_type value = V;

  constexpr operator value_type() const { return value; }
  constexpr value_type operator()() const { return value; }
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;


template <typename...>
using void_t = void;


template <typename T>
struct type_identity {
  using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;


template <typename T>
struct remove_const : type_identity<T> {};

template <typename T>
struct remove_const<const T> : type_identity<T> {};

template <typename T>
using remove_const_t = typename remove_const<T>::type;


template <typename T>
struct remove_volatile : type_identity<T> {};

template <typename T>
struct remove_volatile<volatile T> : type_identity<T> {};

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;


template <typename T>
using remove_cv_t = remove_volatile_t<remove_const_t<T>>;

template <typename T>
struct remove_cv : type_identity<remove_cv_t<T>> {};


template <typename T>
struct remove_reference : type_identity<T> {};

template <typename T>
struct remove_reference<T&> : type_identity<T> {};

template <typename T>
struct remove_reference<T&&> : type_identity<T> {};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;


template <typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

template <typename T>
struct remove_cvref : type_identity<remove_cvref_t<T>> {};


template <typename T, typename = void>
struct add_lvalue_reference : type_identity<T> {};

template <typename T>
struct add_lvalue_reference<T, void_t<T&>> : type_identity<T&> {};

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;


template <typename T, typename = void>
struct add_rvalue_reference : type_identity<T> {};

template <typename T>
struct add_rvalue_reference<T, void_t<T&&>> : type_identity<T&&> {};

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;


template <typename T>
add_rvalue_reference_t<T> declval();


template <typename... Ts>
struct common_type {};

template <typename... Ts>
using common_type_t = typename common_type<Ts...>::type;

namespace impl {

template <typename T1, typename T2>
using cond_op_t =
    remove_cvref_t<decltype(false ? declval<T1>() : declval<T2>())>;

template <typename T1, typename T2, typename = void>
struct common_type2 {};

template <typename T1, typename T2>
struct common_type2<T1, T2, void_t<cond_op_t<T1, T2>>> {
  using type = cond_op_t<T1, T2>;
};

template <typename, typename T1, typename T2, typename... Rest>
struct common_type_rest {};

template <typename T1, typename T2, typename... Rest>
struct common_type_rest<void_t<common_type_t<T1, T2>>, T1, T2, Rest...>
    : common_type<common_type_t<T1, T2>, Rest...> {};

} // namespace impl

template <typename T>
struct common_type<T> : common_type<T, T> {};

template <typename T1, typename T2>
struct common_type<T1, T2> : impl::common_type2<T1, T2> {};

template <typename T1, typename T2, typename... Rest>
struct common_type<T1, T2, Rest...>
    : impl::common_type_rest<void, T1, T2, Rest...> {};


template <bool E, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  using type = T;
};

template <bool E, typename T = void>
using enable_if_t = typename enable_if<E, T>::type;


template <typename T, typename U>
constexpr bool is_same_v = false;

template <typename T>
constexpr bool is_same_v<T, T> = true;

template <typename T, typename U>
struct is_same : bool_constant<is_same_v<T, U>> {};


namespace impl {

template <typename T>
struct is_integral : false_type {};

template <>
struct is_integral<bool> : true_type {};
template <>
struct is_integral<char> : true_type {};
template <>
struct is_integral<signed char> : true_type {};
template <>
struct is_integral<unsigned char> : true_type {};
template <>
struct is_integral<char16_t> : true_type {};
template <>
struct is_integral<char32_t> : true_type {};
template <>
struct is_integral<wchar_t> : true_type {};
template <>
struct is_integral<short> : true_type {};
template <>
struct is_integral<unsigned short> : true_type {};
template <>
struct is_integral<int> : true_type {};
template <>
struct is_integral<unsigned int> : true_type {};
template <>
struct is_integral<long> : true_type {};
template <>
struct is_integral<unsigned long> : true_type {};
template <>
struct is_integral<long long> : true_type {};
template <>
struct is_integral<unsigned long long> : true_type {};

} // namespace impl

template <typename T>
struct is_integral : impl::is_integral<remove_cv_t<T>> {};

template <typename T>
constexpr bool is_integral_v = is_integral<T>::value;

} // namespace psl

/** @} */
