            [org 0x7c00]

kernel_offset_c     equ     0x1000

start:      mov     [boot_drive_c], dl 
            mov     bp, 0x9000
            mov     sp, bp

            mov     bx, msg_rm_c
            call    print
            call    print_nl

            call    ld_kernel
            call    switch_to_pm
            jmp     $

%include    "boot/print.asm"
%include    "boot/print_hex.asm"
%include    "boot/disk.asm"
%include    "boot/32b-print.asm"
%include    "boot/32b-gdt.asm"
%include    "boot/32b-switch.asm"


            [bits 16]

ld_kernel:  mov     bx, msg_ld_kernel_c 
            call    print
            call    print_nl

            mov     bx, kernel_offset_c
            mov     dh, 50
            mov     dl, [boot_drive_c]
            call    disk_load
            ret


            [bits 32]

begin_pm:   mov     ebx, msg_pm_c
            call    print_pm
            call    kernel_offset_c

boot_drive_c        db      0
msg_rm_c            db      "In 16-bit real mode", 0
msg_pm_c            db      "Switched to 32-bit protected mode", 0
msg_ld_kernel_c     db      "Loading kernel into memory...", 0

            times   510-($-$$)  db  0
            dw      0xaa55
