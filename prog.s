    jmp reset
    .org $8000
reset:
    lda #$01
    ldy #$00
    ldx #$0b

loop:
    sty $40,X
    sty $30
    tay
    adc $30
    dex
    bne loop

    .word $eaea, $eaea, $eaea, $eaea, $eaea, $eaea, $eaea, $eaea, $eaea
    .org $fffc
    .word reset, $0000