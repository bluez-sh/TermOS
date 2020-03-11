            [org 0x7c00]

kernel_base_c     equ     0x0800

start:      mov     [boot_drive_c], dl 
            mov     bp, 0x3000
            mov     sp, bp

            mov     bx, msg_rm_c
            call    print
            call    print_nl

            call    ld_kernel
            jmp     switch_to_pm

%include    "boot/print.asm"
%include    "boot/print_hex.asm"
%include    "boot/disk.asm"
%include    "boot/32b-print.asm"
%include    "boot/32b-gdt.asm"
%include    "boot/32b-switch.asm"


            [bits 16]

ld_kernel:  pusha 
            mov     bx, msg_ld_kernel_c 
            call    print
            call    print_nl

            push    es
            mov     ax, kernel_base_c
            mov     es, ax

            mov     bx, 0x0000
            mov     dh, 60
            mov     dl, [boot_drive_c]
            call    disk_load

            pop     es
            popa
            ret


            [bits 32]

begin_pm:   mov     ebx, msg_pm_c
            call    print_pm
            jmp     0x8000

boot_drive_c        db      0
msg_rm_c            db      "In 16-bit real mode", 0
msg_pm_c            db      "Switched to 32-bit protected mode", 0
msg_ld_kernel_c     db      "Loading kernel into memory...", 0

            times   510-($-$$)  db  0
            dw      0xaa55
