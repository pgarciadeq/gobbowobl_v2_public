#include "FileSystem.h"


/*
void kernel() {
    char *video_memory = (char *)0xB8000;
    const char *message = "Hello, Floppy Disk!";
    int i = 0;

    video_memory[0] = 'K';
    video_memory[1] = 0x0F;

    while (message[i] != '\0') {
        video_memory[i * 2] = message[i]; // Character
        video_memory[i * 2 + 1] = 0x07;  // White text on black background
        i++;
    }
}
*/
void _start() {
    //kernel();
    char *video =(char*) 0xB8000;
    const char *msg="IT BOOTS!!!";
    for(int i = 0;msg[i] !=0;++i){
        video[i*2] =msg[i];
        video[i*2+1]=0x0F;
    }
    while(1){
        __asm__ __volatile__("hlt");
    }
}
