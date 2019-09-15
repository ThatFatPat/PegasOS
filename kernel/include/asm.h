#pragma once

#define LOCAL_FUNC(name) \
  .type name, @function; \
  name:

#define FUNC(name) \
  .global name;    \
  LOCAL_FUNC(name)

#define FUNC_END(name) .size name, .- name

#define LOCAL_DATA(name) \
  .type name, @object;   \
  name:

#define DATA(name) \
  .global name;    \
  LOCAL_DATA(name)

#define DATA_END(name) .size name, .- name
