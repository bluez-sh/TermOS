disk_load:      pusha
                push    dx

                mov     ah, 0x02    ; disk read 
                mov     al, dh      ; nb sectors to read
                mov     ch, 0x00    ; cylinder 0
                mov     cl, 0x02    ; sector 2
                mov     dh, 0x00    ; head 0

                int     0x13
                jc      .disk_err

                pop     dx
                cmp     al, dh
                jne     .sect_err

                popa
                ret

.disk_err:      mov     bx, disk_err_c
                call    print
                call    print_nl

                mov     dh, ah
                call    print_hex
                call    print_nl

                jmp     .end_loop

.sect_err:      mov     bx, sect_err_c
                call    print
                call    print_nl

.end_loop:      jmp     $

disk_err_c:     db      "Error reading disk", 0
sect_err_c:     db      "Number of sectors read mismatch", 0
