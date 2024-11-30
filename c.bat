@cls
@echo off

rem where m68k-elf-gcc
rem m68k-elf-gcc -v

REM C:\SysGCC\m68k-elf\bin\m68k-elf-gcc -std=gnu99 -t -save-temps -mc68000 -mpcrel -fno-aggressive-loop-optimizations -fomit-frame-pointer -nostartfiles -Wno-attributes -T ram.ld  -fverbose-asm -Wa,--w,--listing-lhs-width=5,--pcrel,--disp-size-default-16,-aghls=file.asm,--noexecstack file.c -O3 -lm 
m68k-elf-gcc -std=gnu99 -t -save-temps -mc68000 -mpcrel -aux-info funcs.h -fno-aggressive-loop-optimizations -fomit-frame-pointer -nostartfiles -Wno-attributes -T ram.ld -Wa,--w,--pcrel,-acdhls=file.asm,--noexecstack file.c -Os -lm > null

m68k-elf-strip --input-target=elf32-m68k --output-target=symbolsrec -o file.s19 a.out
m68k-elf-objcopy --input-target=srec --output-target=binary file.s19 file.m68
m68k-elf-objdump -D a.out > file.ass
m68k-elf-objdump -h a.out > file.mem

REM  -static -ffreestanding -Wclobbered -fconserve-stack -register-prefix-optional,
REM -Ttext=0x0 -Tdata=0x3000 -Tbss=0x3200

C:\Users\USER\source\repos\NKCtransfer\NKCtransfer\bin\Debug\NKCtransfer COM9 2000000 18000 file.s19 START
REM C:\Users\USER>C:\Users\USER\source\repos\NKCtransfer\NKCtransfer\bin\Debug\NKCtransfer COM9 2000000 18000 C:\Users\USER\Desktop\gruss.s19
REM C:\Users\USER\source\repos\NKCtransfer\NKCtransfer\bin\Debug\NKCtransfer COM9 2000000 Reset