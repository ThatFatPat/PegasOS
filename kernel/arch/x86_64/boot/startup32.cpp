#include <stdint.h>

extern "C" int main() {
  // Constants
  uint32_t VIDEO_MEMORY = 0xb8000;

  // Pre-allocated
  uint8_t pml4[4096];
  uint8_t pdpte1[4096];

  // Code
  uint32_t video_offset = 0x0;
  char* hello = "Hello from\n\r";
  return 0;
}
