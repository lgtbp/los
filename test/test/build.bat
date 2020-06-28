clang test.c -o test.out
llvm-objcopy -O binary test.out test.bin
los.exe