.if GAME_VER >= VER_C3_DEMO_NTSC_U
.equ CLIP_Y_MIN, 9
.equ CLIP_Y_MAX, 232
.else
.equ CLIP_Y_MIN, 1
.equ CLIP_Y_MAX, 256
.endif
