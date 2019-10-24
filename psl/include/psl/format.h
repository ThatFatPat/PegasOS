/**
 * @addtogroup psl
 * @{
 * @file format.h String formatting library. For more details, see the @ref
 * psl_fmt "formatting documentation".
 */

#pragma once

#include <limits.h>
#include <psl/algorithm.h>
#include <psl/charconv.h>
#include <psl/string_view.h>
#include <psl/type_traits.h>
#include <psl/util.h>
#include <stddef.h>
#include <stdint.h>

namespace psl {

/**
 * Class template that can be specialized to provide formatting capabilities for
 * custom types. The second template argument defaults to void and can be used
 * for SFINAE in partial specializations. For more details, see the @ref
 * psl_fmt_formatter "formatter documentation".
 */
template <typename T, typename = void>
struct formatter;


namespace impl {

template <typename O>
class format_args_ref;

template <typename O>
struct format_arg {
  template <typename T>
  constexpr format_arg(const T& val)
      : obj(::psl::addressof(val)),
        output_func([](O& output_sink, const void* obj, string_view spec) {
          formatter<T>::format(output_sink, *static_cast<const T*>(obj), spec);
        }) {}

  const void* obj;
  void (*output_func)(O&, const void*, string_view);
};

template <typename O, size_t N>
class format_arg_store {
public:
  template <typename... Args>
  constexpr format_arg_store(const Args&... args) : args_{{args}...} {
    static_assert(sizeof...(Args) == N);
  }

private:
  friend class format_args_ref<O>;

  format_arg<O> args_[N];
};


template <typename O>
class format_args_ref {
public:
  template <size_t N>
  constexpr format_args_ref(const format_arg_store<O, N>& args)
      : args_(args.args_), size_(N) {}

private:
  template <typename T>
  friend void output_format_arg(T&, const format_args_ref<T>&, size_t&,
                                string_view);

  const impl::format_arg<O>* args_;
  size_t size_;
};


inline string_view extract_prefix(string_view& str, size_t count) {
  auto ret = str.substr(0, count);
  str.remove_prefix(count);
  return ret;
}

template <typename O>
bool handle_escaped(O& output_sink, string_view& str, char c) {
  if (!str.empty() && str.front() == c) {
    str.remove_prefix(1);
    output_sink(string_view{&c, 1});
    return true;
  }
  return false;
}

template <typename O>
void output_format_arg(O& output_sink, const format_args_ref<O>& args,
                       size_t& curr_arg, string_view spec) {
  if (curr_arg >= args.size_) {
    return;
  }

  auto user_delim_pos = find(spec.begin(), spec.end(), ':');
  string_view user_spec = user_delim_pos == spec.end()
                              ? ""
                              : spec.substr(user_delim_pos - spec.begin() + 1);

  const auto& arg = args.args_[curr_arg++];
  arg.output_func(output_sink, arg.obj, user_spec);
}

} // namespace impl


/**
 * Opaque type which can be used to reference type-erased formatting arguments
 * created by @ref make_format_args().
 * @note To prevent dangling references, objects of this type should not outlive
 * the formatting argument objects (created by @ref make_format_args()) to
 * which they refer.
 */
template <typename O>
using format_args_ref_t = impl::format_args_ref<type_identity_t<O>>;

/**
 * Create a type-erased object storing references to formatting arguments in a
 * format suitable for output through @ref psl_fmt_output_sink "output sink"
 * `O`.
 * @note The exact return type of this function is unspecified, but is
 * guaranteed to be convertible to `psl::format_args_ref_t<O>`.
 */
template <typename O, typename... Args>
constexpr impl::format_arg_store<O, sizeof...(Args)> make_format_args(
    const Args&... args) {
  return {args...};
}


/**
 * Format the @ref psl_fmt_str "format string" `fmt` with the type-erased
 * arguments stored in `args`, reporting output to `output_sink`.
 * @param output_sink The @ref psl_fmt_output_sink "output sink" to use.
 * @param fmt The @ref psl_fmt_str "format string" to use.
 * @param args A reference to the object holding type-erased formatting
 * arguments. Use @ref make_format_args() to create such an object.
 * @note If the format string is invalid, or if there are more @ref
 * psl_fmt_str_subst "substitutions" in the format string than formatting
 * arguments, the output of this function is unspecified.
 */
template <typename O>
void vformat(O& output_sink, string_view fmt, format_args_ref_t<O> args) {
  size_t curr_arg = 0;

  while (!fmt.empty()) {
    auto brace_pos = find_if(fmt.begin(), fmt.end(),
                             [](char c) { return c == '{' || c == '}'; });
    auto brace_off = brace_pos - fmt.begin();
    output_sink(impl::extract_prefix(fmt, brace_off));

    if (fmt.empty()) {
      // Nothing more to do
      break;
    }

    fmt.remove_prefix(1); // Skip brace character

    switch (*brace_pos) {
    case '{':
      if (impl::handle_escaped(output_sink, fmt, '{')) {
        break;
      }

      {
        auto close_brace_pos = find(fmt.begin(), fmt.end(), '}');
        if (close_brace_pos == fmt.end()) {
          // Invalid format string
          return;
        }

        auto spec = impl::extract_prefix(fmt, close_brace_pos - fmt.begin());
        fmt.remove_prefix(1); // Skip '}'
        impl::output_format_arg(output_sink, args, curr_arg, spec);
      }

      break;

    case '}':
      if (!impl::handle_escaped(output_sink, fmt, '}')) {
        // Invalid format string
        return;
      }
      break;
    }
  }
}

/**
 * Format the @ref psl_fmt_str "format string" `fmt` with the arguments `args`,
 * reporting output to `output_sink`.
 * @param output_sink The @ref psl_fmt_output_sink "output sink" to use.
 * @param fmt The @ref psl_fmt_str "format string" to use.
 * @param args The formatting arguments to use.
 * @note If the format string is invalid, or if there are more @ref
 * psl_fmt_str_subst "substitutions" in the format string than formatting
 * arguments, the output of this function is unspecified.
 */
template <typename O, typename... Args>
void format(O& output_sink, string_view fmt, const Args&... args) {
  vformat(output_sink, fmt, make_format_args<O>(args...));
}


// Built-in formatters

/**
 * @ref psl_fmt_formatter_builtin "Built-in formatter" for string types.
 */
template <typename T>
struct formatter<T, enable_if_t<is_convertible_v<T, string_view>>> {
  template <typename O>
  static void format(O& output_sink, string_view val, string_view) {
    output_sink(val);
  }
};

/**
 * @ref psl_fmt_formatter_builtin "Built-in formatter" for characters.
 */
template <>
struct formatter<char> {
  template <typename O>
  static void format(O& output_sink, char c, string_view) {
    output_sink(string_view{&c, 1});
  }
};

/**
 * @ref psl_fmt_formatter_builtin "Built-in formatter" for integer types.
 */
template <typename I>
struct formatter<I, enable_if_t<is_integral_v<I>>> {
  template <typename O>
  static void format(O& output_sink, I val, string_view spec) {
    char buf[sizeof(I) * CHAR_BIT];
    int base = spec == "x" ? 16 : spec == "b" ? 2 : 10;

    char* str_end = to_chars(begin(buf), end(buf), val, base);
    if (str_end) {
      size_t str_size = str_end - begin(buf);
      output_sink(string_view{buf, str_size});
    }
  }
};

/**
 * @ref psl_fmt_formatter_builtin "Built-in formatter" for booleans.
 */
template <>
struct formatter<bool> {
  template <typename O>
  static void format(O& output_sink, bool val, string_view) {
    using namespace literals;
    output_sink(val ? "true"_sv : "false"_sv);
  }
};

/**
 * @ref psl_fmt_formatter_builtin "Built-in formatter" for `nullptr_t`.
 */
template <>
struct formatter<nullptr_t> {
  template <typename O>
  static void format(O& output_sink, nullptr_t, string_view) {
    using namespace literals;
    output_sink("nullptr"_sv);
  }
};

/**
 * @ref psl_fmt_formatter_builtin "Built-in formatter" for void pointers.
 */
template <typename T>
struct formatter<T*, enable_if_t<is_void_v<T>>> {
  template <typename O>
  static void format(O& output_sink, T* ptr, string_view) {
    if (!ptr) {
      return formatter<nullptr_t>::format(output_sink, nullptr, "");
    }
    psl::format(output_sink, "0x{:x}", reinterpret_cast<uintptr_t>(ptr));
  }
};

} // namespace psl

/** @} */
