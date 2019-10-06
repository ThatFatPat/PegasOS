/**
 * @file intellisense_workaround.h Workaround for VSCode's intellisense assuming
 * that the implementation is hosted.
 * @ingroup klibc
 */

#pragma once

// IntelliSense assumes we are hosted, which causes problems with some of GCC's
// headers. Undef the macro here, which will cause them to (correctly) believe
// that the implementation is freestanding.
#if __STDC_HOSTED__
#undef __STDC_HOSTED__
#endif
