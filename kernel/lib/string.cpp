#include <string.h>

#include <stdint.h>

#undef memcpy
#undef memmove
#undef memset

void* memcpy(void* __restrict dest, const void* src, size_t count) {
  uint8_t* ch_dest = static_cast<uint8_t*>(dest);
  const uint8_t* ch_src = static_cast<const uint8_t*>(src);

  for (size_t i = 0; i < count; i++) {
    ch_dest[i] = ch_src[i];
  }

  return dest;
}

void* memmove(void* dest, const void* src, size_t count) {
  uint8_t* ch_dest = static_cast<uint8_t*>(dest);
  const uint8_t* ch_src = static_cast<const uint8_t*>(src);

  if (ch_src < ch_dest) {
    for (size_t i = count; i > 0; i--) {
      ch_dest[i - 1] = ch_src[i - 1];
    }
  } else {
    for (size_t i = 0; i < count; i++) {
      ch_dest[i] = ch_src[i];
    }
  }

  return dest;
}

void* memset(void* dest, int ch, size_t count) {
  uint8_t* ch_dest = static_cast<uint8_t*>(dest);

  for (size_t i = 0; i < count; i++) {
    ch_dest[i] = static_cast<uint8_t>(ch);
  }

  return dest;
}
