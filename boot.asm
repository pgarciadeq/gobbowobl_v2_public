[BITS 16]
[ORG 0x7C00]

start:
    mov ah, 0x0E       ; Teletype output
    mov al, 'B'
    int 0x10
    
    mov ax, 0x1000        ; Target address for the kernel
    mov es, ax            ; Set ES to the target segment
    xor bx, bx            ; Offset 0
    mov ah, 0x02          ; BIOS read function
    mov al, 1             ; Number of sectors to read
    mov ch, 0             ; Cylinder 0
    mov cl, 2             ; Sector 2
    mov dh, 0             ; Head 0
    mov dl, 0x00          ; (0x80 = hard disk, 0x00 = floppy)
    int 0x13              ; Call BIOS interrupt to read disk
    jc error              ; Jump if carry flag is set (error)

    mov si, 0x9000
    mov [si], ah          ;Dump error code (DEBUG)

    mov ah, 0x0E
    mov al, 'R'           ; R = Read succeeded
    int 0x10

    jmp 0x1000:0x0000     ; Jump to kernel entry point

error:
    hlt                   ; Halt the CPU on error

times 446-($-$$) db 0     ; Pad to 446 bytes for bootloader code
; Fake partition table (not used, safe to zero too)
db 0x00, 0x00, 0x00, 0x00
db 0x00, 0x00, 0x00, 0x00
db 0x00, 0x00, 0x00, 0x00
db 0x00, 0x00, 0x00, 0x00

times 510-($-$$) db 0     ; Pad to 510 bytes
dw 0xAA55                ; Boot signature
