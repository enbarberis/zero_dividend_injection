section .text


align 0x1000
global zdi

;rdi = upper dividend
;rsi = lower dividend
;rdx = divisor
;rcx = fr_buf
zdi:
    push    r15
    mov     r15, rcx            ;Backup of fr_buf

    mov     rcx, rdx            ;Setting the divisor
    mov     rax, rsi            ;Setting lower part of dividend
    mov     rdx, rdi            ;Setting upper part of dividend

    %rep 64
    imul    rdx, 1              ;Slow computation of rdx
    %endrep

    div     rcx                 ;Division affected by ZDI
    mov     rax, [r15 + rax]    ;Leave uarch trace 

    pop r15
    ret

