.include "macro.inc"

.section .data, "wa"

dlabel D_8006FCF4
dlabel g_SonyImage
.if GAME_VER == VER_C3_DEMO_NTSC_U
.incbin "assets/sony_text_eu.databin.bin"
.else
.incbin "assets/sony_text.databin.bin"
.endif
