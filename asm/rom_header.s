/*
 * Super Mario 64 ROM header
 * Only the first 0x18 bytes matter to the console.
 */
 
.byte  0x80, 0x37, 0x12, 0x40   /* PI BSD Domain 1 register */
.word  0x0000000F               /* Clockrate setting*/
.word  entry_point              /* Entrypoint */

/* Revision */
.if VERSION_SH == 1
    .word  0x00001448
.elseif VERSION_EU == 1
    .word  0x00001446
.else /* NTSC-U and NTSC-J 1.0 */
    .word  0x00001444
.endif

.word  0x4EAA3D0E               /* Checksum 1 */
.word  0x74757C24               /* Checksum 2 */
.word  0x00000000               /* Unknown */
.word  0x00000000               /* Unknown */

.include "src/internal_name.s"

#I will fix later
# .if USE_GC == 1
.if  1
/* Advanced homebrew ROM header bytes: https://n64brew.dev/wiki/ROM_Header#Advanced_Homebrew_ROM_Header */
.word  0x82000000
.else
.word  0x00000000               /* Unknown */
.endif
.word  0x0000004E               /* Cartridge */

.if 0
# .if EEP4K == 1 && USE_GC == 0
.ascii "SM"                     /* Cartridge ID */
.else
.ascii "ED"                     /* Cartridge ID */
.endif

/* Region */
.if VERSION_EU == 1
    .ascii "P"                  /* PAL (Europe) */
.elseif (VERSION_JP == 1 || VERSION_SH == 1)
    .ascii "J"                  /* NTSC-J (Japan) */
.else
    .ascii "E"                  /* NTSC-U (North America) */
.endif

/* Undefined versions 1 and 2 are NTSC-U (North America) and PAL (Europe) respectively */
.if VERSION_SH == 1
    .byte 0x03                  /* Version (Shindou) */
.else
    .byte 0x00                  /* Version */
.endif

