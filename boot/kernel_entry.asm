global _start
[bits 32]
[extern kmain]

_start:     
            ;jmp setup_stack
;l1:         
            call    kmain
            jmp     $

;setup_stack:
            ;mov     ebp, stack_end
            ;mov     esp, ebp
            ;jmp     l1

            ;[section .bss]

;stack_begin:
            ;resb    16777216
;stack_end:
