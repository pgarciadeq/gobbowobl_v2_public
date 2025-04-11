all: floppy.img

boot.bin: boot.asm
	nasm -f bin $< -o $@

kernel.bin: kernel.c FileSystem.h
	gcc -ffreestanding -m32 -nostdlib -c kernel.c -o kernel.o
	ld -m elf_i386 -T link.ld -o kernel.elf kernel.o
	objcopy -O binary kernel.elf $@

create_disk: create-disk.c
	gcc $< -o $@

floppy.img: boot.bin kernel.bin create_disk
	./create_disk boot.bin kernel.bin

run: floppy.img
	qemu-system-i386 -fda floppy.img

clean:
	rm -f boot.bin kernel.o kernel.elf kernel.bin floppy.img create_disk