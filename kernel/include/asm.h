/**
 * @addtogroup kernel
 * @{
 * @file asm.h Assembly macros
 */

#pragma once

/**
 * Define a file-local assembly function with the specified name.
 */
#define LOCAL_FUNC(name) \
  .type name, @function; \
  name:

/**
 * Define a global assembly function with the specified name.
 */
#define FUNC(name) \
  .global name;    \
  LOCAL_FUNC(name)

/**
 * Indicate the end of an assembly function defined with FUNC or LOCAL_FUNC.
 */
#define FUNC_END(name) .size name, .- name


/**
 * Define a file-local assembly variable with the specified name.
 */
#define LOCAL_DATA(name) \
  .type name, @object;   \
  name:

/**
 * Define a global assembly variable with the specified name.
 */
#define DATA(name) \
  .global name;    \
  LOCAL_DATA(name)

/**
 * Indicate the end of an assembly variable defined with DATA or LOCAL_DATA.
 */
#define DATA_END(name) .size name, .- name

/** @} */
