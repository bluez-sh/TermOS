print_hex:  pusha
            xor     cx, cx

.loop:      cmp     cx, 4
            je      .end

            mov     ax, dx
            and     ax, 0x000f
            add     al, 0x30
            cmp     al, 0x39
            jle     .next
            add     al, 0x7

.next       mov     bx, hex_char_v + 5
            sub     bx, cx
            mov     [bx], al

            ror     dx, 4
            inc     cx
            jmp     .loop

.end        mov     bx, hex_char_v
            call    print

            popa
            ret

hex_char_v: db      "0x0000", 0
