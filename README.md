This is a bootloader project to experiment with emulating low-level design.

TO RUN THE PROGRAM:
This was designed with QEMU being utilized. The command:
qemu-system-i386 -fda floppy.img


List of commands for compiling, etc:
gcc -ffreestanding -nostdlib -m32 -o kernel.bin kernel.c
gcc create_disk.c -o create_disk
./create_disk boot.bin kernel.bin readme.txt
