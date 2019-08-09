UNAME := $(shell uname)
all: boot.bin

KERNEL32_LOCATION := 0x8001
ifeq ($(UNAME), Darwin)
	export LDFLAGS="-L/usr/local/opt/llvm/bin"
	export CPPFLAGS="-I/usr/local/opt/llvm/include"
	PATH := "/usr/local/opt/llvm/bin:$(PATH)"

endif

kernel32.o: kernel32.c
	clang -ffreestanding -c --target=i386 $< -o $@

kernel32.bin: kernel32.o
ifeq ($(UNAME), Darwin)
		ld.lld -o kernel32.bin -Ttext $(KERNEL32_LOCATION) $^ --oformat binary
endif
ifeq ($(UNAME), Linux)
		ld -o kernel32.bin -Ttext $(KERNEL32_LOCATION) $^ --oformat binary
endif

boot16.bin: boot.asm
	nasm -f bin $< -o $@

boot.bin: boot16.bin kernel32.bin
ifeq ($(UNAME), Darwin)
		cat $^ >> boot.bin
endif
ifeq ($(UNAME), Linux)
		cat $^ > boot.bin
endif

clean:
	rm *.bin *.o