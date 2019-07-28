            [bits 32]

video_mem_c         equ     0xb8000
white_on_black_c    equ     0x0f

print_pm:   pusha
            mov     edx, video_mem_c

.loop:      mov     al, [ebx]
            mov     ah, white_on_black_c

            cmp     al, 0
            je      .end

            mov     [edx], ax
            inc     ebx
            add     edx, 2

            jmp     .loop

.end:       popa
            ret
