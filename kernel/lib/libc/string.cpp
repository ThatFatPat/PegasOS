#include <string.h>

#include <psl/byte.h>
#include <stdint.h>

#undef memcpy
#undef memmove
#undef memset
#undef strlen

void* memcpy(void* __restrict dest, const void* src, size_t count) {
  auto* byte_dest = static_cast<psl::byte*>(dest);
  const auto* byte_src = static_cast<const psl::byte*>(src);

  for (size_t i = 0; i < count; i++) {
    byte_dest[i] = byte_src[i];
  }

  return dest;
}

void* memmove(void* dest, const void* src, size_t count) {
  auto* byte_dest = static_cast<psl::byte*>(dest);
  const auto* byte_src = static_cast<const psl::byte*>(src);

  if (byte_src < byte_dest) {
    for (size_t i = count; i > 0; i--) {
      byte_dest[i - 1] = byte_src[i - 1];
    }
  } else {
    for (size_t i = 0; i < count; i++) {
      byte_dest[i] = byte_src[i];
    }
  }

  return dest;
}

void* memset(void* dest, int ch, size_t count) {
  auto* byte_dest = static_cast<psl::byte*>(dest);

  for (size_t i = 0; i < count; i++) {
    byte_dest[i] = static_cast<psl::byte>(ch);
  }

  return dest;
}

size_t strlen(const char* str) {
  size_t len = 0;
  for (; *str; str++, len++) {
  }
  return len;
}
