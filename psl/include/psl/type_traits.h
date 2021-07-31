/**
 * @addtogroup psl
 * @{
 * @file type_traits.h Partial implementation of the standard
 * &lt;type_traits&gt; header, with psl-specific bits.
 */

#pragma once

#include <stddef.h>

namespace psl {

/**
 * Enable wrapping of an integer constant in a type. Serves as the base class
 * for many of the type traits defined here.
 */
template <typename T, T V>
struct integral_constant {
  using value_type = T;
  using type = integral_constant;

  static constexpr value_type value = V;

  constexpr operator value_type() const { return value; }
  constexpr value_type operator()() const { return value; }
};

/**
 * Utility typedef for the common case of boolean integer constants.
 */
template <bool B>
using bool_constant = integral_constant<bool, B>;

/**
 * `true` bool_constant.
 */
using true_type = bool_constant<true>;

/**
 * `false` bool_constant.
 */
using false_type = bool_constant<false>;


/**
 * Dummy type alias which always resolves to `void`. This is used to detect
 * ill-formed types in SFINAE contexts.
 */
template <typename...>
using void_t = void;


/**
 * Provide a member alias `type` equivalent to
 * `T`. Serves as the base class for many type metafunctions.
 */
template <typename T>
struct type_identity {
  using type = T;
};

/**
 * Helper alias for using type_identity.
 * @note When used as a parameter type in a function template, this alias can
 * prevent deduction of of the template parameters.
 */
template <typename T>
using type_identity_t = typename type_identity<T>::type;


/**
 * Provide a member alias `type` equivalent to `T` with the top-level `const`
 * qualifier removed (if it exists).
 */
template <typename T>
struct remove_const : type_identity<T> {};

template <typename T>
struct remove_const<const T> : type_identity<T> {};

/**
 * Helper alias for using remove_const.
 */
template <typename T>
using remove_const_t = typename remove_const<T>::type;


/**
 * Provide a member alias `type` equivalent to `T` with the top-level
 * `volatile` qualifier removed (if it exists).
 */
template <typename T>
struct remove_volatile : type_identity<T> {};

template <typename T>
struct remove_volatile<volatile T> : type_identity<T> {};

/**
 * Helper alias for using remove_volatile.
 */
template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;


/**
 * Helper alias for using remove_cv.
 */
template <typename T>
using remove_cv_t = remove_volatile_t<remove_const_t<T>>;

/**
 * Provide a member alias `type` equivalent to `T` with the top-level `const`
 * and `volatile` qualifiers removed.
 */
template <typename T>
struct remove_cv : type_identity<remove_cv_t<T>> {};


/**
 * Provide a member alias `type` equivalent to `U` with any of `T`'s
 * cv-qualifiers applied to it.
 * @note This metafunction is psl-specific.
 */
template <typename T, typename U>
struct apply_cv : type_identity<U> {};

template <typename T, typename U>
struct apply_cv<const T, U> : type_identity<const U> {};

template <typename T, typename U>
struct apply_cv<volatile T, U> : type_identity<volatile U> {};

template <typename T, typename U>
struct apply_cv<const volatile T, U> : type_identity<const volatile U> {};

/**
 * Helper alias for using apply_cv.
 */
template <typename T, typename U>
using apply_cv_t = typename apply_cv<T, U>::type;


/**
 * Provide a member alias `type` equivalent to `T` with references removed.
 */
template <typename T>
struct remove_reference : type_identity<T> {};

template <typename T>
struct remove_reference<T&> : type_identity<T> {};

template <typename T>
struct remove_reference<T&&> : type_identity<T> {};

/**
 * Helper alias for using remove_reference.
 */
template <typename T>
using remove_reference_t = typename remove_reference<T>::type;


/**
 * Helper alias for using remove_cvref.
 */
template <typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

/**
 * Provide a member alias `type` equivalent to `T` with the top-level `const`
 * and `volatile` qualifiers and any references stripped.
 */
template <typename T>
struct remove_cvref : type_identity<remove_cvref_t<T>> {};


/**
 * If the type `T&` is well-formed, provide a member alias `type` equivalent to
 * `T&`. Otherwise, `type` is `T`.
 */
template <typename T, typename = void>
struct add_lvalue_reference : type_identity<T> {};

template <typename T>
struct add_lvalue_reference<T, void_t<T&>> : type_identity<T&> {};

/**
 * Helper alias for using add_lvalue_reference.
 */
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;


/**
 * If the type `T&&` is well-formed, provide a member alias `type` equivalent to
 * `T&&`. Otherwise, `type` is `T`.
 */
template <typename T, typename = void>
struct add_rvalue_reference : type_identity<T> {};

template <typename T>
struct add_rvalue_reference<T, void_t<T&&>> : type_identity<T&&> {};

/**
 * Helper alias for using add_rvalue_reference.
 */
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;


/**
 * Helper which generates a value of type `T&&` for use with `decltype`.
 * @note This function may only be used in unevaluated contexts.
 */
template <typename T>
add_rvalue_reference_t<T> declval();


/**
 * If all of the types `Ts...` are convertible to some type `U`, provide a
 * member alias `type` equivalent to `U`. Otherwise, `type` is not provided.
 * @note The current implementation is somewhat simplified and is not completely
 * conformant. Notably, it does not decay its argument types.
 */
template <typename... Ts>
struct common_type {};

/**
 * Helper alias for using common_type.
 */
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


/**
 * If `E` is true, provide a member alias `type` equivalent to `T`. Otherwise,
 * no member is provided.
 */
template <bool E, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  using type = T;
};

/**
 * Helper alias for using enable_if.
 */
template <bool E, typename T = void>
using enable_if_t = typename enable_if<E, T>::type;


/**
 * `true` iff `T` and `U` name the same type.
 */
template <typename T, typename U>
constexpr bool is_same_v = false;

template <typename T>
constexpr bool is_same_v<T, T> = true;

/**
 * Derives from `true_type` iff `T` and `U` name the same type. Otherwise,
 * derives from `false_type`.
 */
template <typename T, typename U>
struct is_same : bool_constant<is_same_v<T, U>> {};


/**
 * `true` iff `T` is (cv-qualified) void.
 */
template <typename T>
constexpr bool is_void_v = is_same_v<remove_cv_t<T>, void>;

/**
 * Derives from `true_type` iff `T` is (cv-qualified) void.
 * Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_void : bool_constant<is_void_v<T>> {};


/**
 * `true` iff `T` is a (bounded or unbounded) array type.
 */
template <typename T>
constexpr bool is_array_v = false;

template <typename T>
constexpr bool is_array_v<T[]> = true;

template <typename T, size_t N>
constexpr bool is_array_v<T[N]> = true;

/**
 * Derives from `true_type` iff `T` is a (bounded or unbounded) array type.
 * Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_array : bool_constant<is_array_v<T>> {};


/**
 * `true` iff `T` is an lvalue or rvalue reference type.
 */
template <typename T>
constexpr bool is_reference_v = false;

template <typename T>
constexpr bool is_reference_v<T&> = true;

template <typename T>
constexpr bool is_reference_v<T&&> = true;

/**
 * Derives from `true_type` iff `T` is an lvalue or rvalue reference type.
 * Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_reference : bool_constant<is_reference_v<T>> {};


/**
 * `true` iff `T` is a const-qualified type.
 */
template <typename T>
constexpr bool is_const_v = false;

template <typename T>
constexpr bool is_const_v<const T> = true;

/**
 * Derives from `true_type` iff `T` is an const-qualified type.
 * Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_const : bool_constant<is_const_v<T>> {};


/**
 * `true` iff `T` is a volatile-qualified type.
 */
template <typename T>
constexpr bool is_volatile_v = false;

template <typename T>
constexpr bool is_volatile_v<volatile T> = true;

/**
 * Derives from `true_type` iff `T` is an volatile-qualified type.
 * Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_volatile : bool_constant<is_volatile_v<T>> {};


/**
 * `true` iff `T` is a function type.
 */
template <typename T>
constexpr bool is_function_v = !is_reference_v<T> && !is_const_v<const T>;

/**
 * Derives from `true_type` iff `T` is a function type.
 * Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_function : bool_constant<is_function_v<T>> {};


namespace impl {

template <typename To>
void try_convert(To);

template <typename From, typename To, typename = void>
constexpr bool is_convertible_test_v = false;

template <typename From, typename To>
constexpr bool is_convertible_test_v<
    From, To, decltype(::psl::impl::try_convert<To>(declval<From>()))> = true;

} // namespace impl


/**
 * `true` iff `From` is implicitly convertible to `To`.
 */
template <typename From, typename To,
          bool = is_void_v<From> || is_array_v<To> || is_function_v<To>>
constexpr bool is_convertible_v = is_void_v<To>;

template <typename From, typename To>
constexpr bool is_convertible_v<From, To, false> =
    impl::is_convertible_test_v<From, To>;

/**
 * Derives from `true_type` iff `From` is implicitly convertible to `To`.
 * Otherwise, derives from `false_type`.
 */
template <typename From, typename To>
struct is_convertible : bool_constant<is_convertible_v<From, To>> {};


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

/**
 * Derives from `true_type` iff `T` names a (possibly cv-qualified) integer
 * type. Otherwise, derives from `false_type`.
 */
template <typename T>
struct is_integral : impl::is_integral<remove_cv_t<T>> {};

/**
 * `true` iff `T` names a (possibly cv-qualified) integer
 * type.
 */
template <typename T>
constexpr bool is_integral_v = is_integral<T>::value;


/**
 * `true` iff `T` is a signed integer type.
 */
template <typename T, bool = is_integral_v<T>>
constexpr bool is_signed_v = T(-1) < T(0);

template <typename T>
constexpr bool is_signed_v<T, false> = false;

/**
 * Derives from `true_type` iff `T` is a signed integer type. Otherwise, derives
 * from `false_type`.
 */
template <typename T>
struct is_signed : bool_constant<is_signed_v<T>> {};


/**
 * `true` iff `T` is an unsigned integer type.
 */
template <typename T, bool = is_integral_v<T>>
constexpr bool is_unsigned_v = T(-1) > T(0);

template <typename T>
constexpr bool is_unsigned_v<T, false> = false;

/**
 * Derives from `true_type` iff `T` is an unsigned integer type.
 */
template <typename T>
struct is_unsigned : bool_constant<is_unsigned_v<T>> {};


namespace impl {

template <typename T>
constexpr bool is_nonbool_integral_v =
    is_integral_v<T> && !is_same_v<remove_cv_t<T>, bool>;


template <typename T>
struct make_signed : type_identity<T> {};

template <>
struct make_signed<char> : type_identity<signed char> {};
template <>
struct make_signed<unsigned char> : type_identity<signed char> {};
template <>
struct make_signed<unsigned short> : type_identity<short> {};
template <>
struct make_signed<unsigned int> : type_identity<int> {};
template <>
struct make_signed<unsigned long> : type_identity<long> {};
template <>
struct make_signed<unsigned long long> : type_identity<long long> {};

template <typename T>
using make_signed_t = typename make_signed<T>::type;


template <typename T>
struct make_unsigned : type_identity<T> {};

template <>
struct make_unsigned<char> : type_identity<unsigned char> {};
template <>
struct make_unsigned<signed char> : type_identity<unsigned char> {};
template <>
struct make_unsigned<short> : type_identity<unsigned short> {};
template <>
struct make_unsigned<int> : type_identity<unsigned int> {};
template <>
struct make_unsigned<long> : type_identity<unsigned long> {};
template <>
struct make_unsigned<long long> : type_identity<unsigned long long> {};

template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

} // namespace impl


/**
 * If `T` is an integer type that is not `bool`, provide a member alias `type`
 * corresponding to the signed variant of `T`. `T`'s cv-qualifiers are
 * preserved.
 */
template <typename T, bool = impl::is_nonbool_integral_v<T>>
struct make_signed {};

template <typename T>
struct make_signed<T, true>
    : type_identity<apply_cv_t<T, impl::make_signed_t<T>>> {};

/**
 * Helper alias for using make_unsigned.
 */
template <typename T>
using make_signed_t = typename make_signed<T>::type;


/**
 * If `T` is an integer type that is not `bool`, provide a member alias `type`
 * corresponding to the unsigned variant of `T`. `T`'s cv-qualifiers are
 * preserved.
 */
template <typename T, bool = impl::is_nonbool_integral_v<T>>
struct make_unsigned {};

template <typename T>
struct make_unsigned<T, true>
    : type_identity<apply_cv_t<T, impl::make_unsigned_t<T>>> {};

/**
 * Helper alias for using make_unsigned.
 */
template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

} // namespace psl

/** @} */
