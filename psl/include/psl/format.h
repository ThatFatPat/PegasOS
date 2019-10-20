/**
 * @addtogroup psl
 * @{
 * @file format.h String formatting library.
 */

#pragma once

#include <psl/algorithm.h>
#include <psl/string_view.h>
#include <psl/type_traits.h>
#include <psl/util.h>
#include <stddef.h>

namespace psl {

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
        output_func([](O& outputter, const void* obj, string_view spec) {
          formatter<T>::format(outputter, *static_cast<T*>(obj), spec);
        }) {}

  const void* obj;
  void (*output_func)(O&, const void*, string_view);
};

template <typename O, typename... Args>
class format_arg_store {
public:
  constexpr format_arg_store(const Args&... args) : args_{{args}...} {}

private:
  friend class format_args_ref<O>;

  format_arg<O> args_[sizeof...(Args)];
};


template <typename O>
class format_args_ref {
public:
  template <typename... Args>
  constexpr format_args_ref(const format_arg_store<O, Args...>& args)
      : args_(args.args_), size_(sizeof...(Args)) {}

  template <typename... Args>
  format_args_ref(const format_arg_store<O, Args...>&&) = delete;

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
bool handle_escaped(O& outputter, string_view& str, char c) {
  if (!str.empty() && str.front() == c) {
    str.remove_prefix(1);
    outputter(string_view{&c, 1});
    return true;
  }
  return false;
}

template <typename O>
void output_format_arg(O& outputter, const format_args_ref<O>& args,
                       size_t& curr_arg, string_view spec) {
  if (curr_arg >= args.size_) {
    return;
  }

  auto user_delim_pos = find(spec.begin(), spec.end(), ':');
  string_view user_spec = user_delim_pos == spec.end()
                              ? ""
                              : spec.substr(user_delim_pos - spec.begin() + 1);

  const auto& arg = args.args_[curr_arg++];
  arg.output_func(outputter, arg.obj, user_spec);
}

} // namespace impl


template <typename O>
using format_args_ref_t = impl::format_args_ref<type_identity_t<O>>;

template <typename O, typename... Args>
constexpr impl::format_arg_store<O, Args...> make_format_args(
    const Args&... args) {
  return {args...};
}


template <typename O>
void vformat(O& outputter, string_view fmt, format_args_ref_t<O> args) {
  size_t curr_arg = 0;

  while (!fmt.empty()) {
    auto brace_pos = find_if(fmt.begin(), fmt.end(),
                             [](char c) { return c == '{' || c == '}'; });
    auto brace_off = brace_pos - fmt.begin();
    outputter(impl::extract_prefix(fmt, brace_off));

    if (fmt.empty()) {
      // Nothing more to do
      break;
    }

    fmt.remove_prefix(1); // Skip brace character

    switch (*brace_pos) {
    case '{':
      if (impl::handle_escaped(outputter, fmt, '{')) {
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
        impl::output_format_arg(outputter, args, curr_arg, spec);
      }

      break;

    case '}':
      if (!impl::handle_escaped(outputter, fmt, '}')) {
        // Invalid format string
        return;
      }
      break;
    }
  }
}

template <typename O, typename... Args>
void format(O& outputter, string_view fmt, const Args&... args) {
  auto erased_args = make_format_args<O>(args...);
  vformat(outputter, fmt, erased_args);
}

} // namespace psl

/** @} */
