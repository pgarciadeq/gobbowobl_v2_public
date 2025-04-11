#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "FileSystem.h"

#define DISK_SIZE 1474560
#define BOOTLOADER_SIZE 512
#define FILE_TABLE_OFFSET 512
#define FILE_TABLE_SIZE 512
#define FILE_SIZE 1024
#define DATA_START_OFFSET 1536

void write_entry(FILE* disk, int entry_index, struct FileEntry* entry) {
	fseek(disk, FILE_TABLE_OFFSET + entry_index * FILE_ENTRY_SIZE, SEEK_SET);
	fwrite(entry, sizeof(struct FileEntry), 1, disk);
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Usage: %s <file> <name>\n", argv[0]);
		return 1;
	}

	//Create a new disk image
	FILE* disk = fopen("floppy.img", "wb+");
	if (!disk) {
		perror("Failed to create disk image");
		return 1;

	}

	//Zero out the disk image
	uint8_t zero = 0;
	for (int i = 0; i < DISK_SIZE; i++) {
		fwrite(&zero, 1, 1, disk);
	}

	//Writes bootloader to sector 0
	FILE* bootloader = fopen(argv[1], "rb");
	if (!bootloader) {
		perror("Failed to open bootloader file");
		fclose(disk);
		return 1;
	}

	uint8_t bootloader_buffer[BOOTLOADER_SIZE] = { 0 };
	fread(bootloader_buffer, 1, BOOTLOADER_SIZE, bootloader);
	fclose(bootloader);

	fseek(disk, 0, SEEK_SET);
	fwrite(bootloader_buffer, 1, BOOTLOADER_SIZE, disk);


	int file_count = 0;
	uint32_t current_offset = DATA_START_OFFSET;

	for (int i = 2; i < argc && file_count < MAX_FILES; i++) {
		FILE* in = fopen(argv[i], "rb");
		if (!in) {
			perror("Failed to open input file");
			continue;
		}

		fseek(in, 0, SEEK_END);
		uint32_t size = ftell(in);
		fseek(in, 0, SEEK_SET);

		if (current_offset + size > DISK_SIZE) {
			printf("Disk is full, cannot add more files.\n");
			fclose(in);
			break;
		}

		struct FileEntry entry = { 0 };
		strncpy(entry.name, argv[i], sizeof(entry.name) - 1);
		entry.size = size;
		entry.offset = current_offset;

		write_entry(disk, file_count, &entry);

		fseek(disk, current_offset, SEEK_SET);
		uint8_t buffer[1024];
		int read;
		while ((read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
			fwrite(buffer, 1, read, disk);
		}

		fclose(in);
		printf("Added file: %s (%u bytes)\n", argv[i], size);
		current_offset += size;
		file_count++;
	}
	fclose(disk);
	printf("Disk image 'floppy.img' created with %d file(s).\n", file_count);
	return 0;
}