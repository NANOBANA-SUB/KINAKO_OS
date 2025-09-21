#!/bin/bash
set -xue

QEMU=qemu-system-riscv64

# clangのパス (Ubuntuの場合は CC=clang)
CC=/opt/homebrew/opt/llvm/bin/clang

CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra \
  --target=riscv64-unknown-elf \
  -fno-stack-protector -ffreestanding -nostdlib \
  -mcmodel=medany -fno-pic -fno-pie"

cd kernel
# カーネルをビルド
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c kprintf.c kstring.c

# elf2bin
riscv64-elf-objcopy -O binary kernel.elf kernel.bin
