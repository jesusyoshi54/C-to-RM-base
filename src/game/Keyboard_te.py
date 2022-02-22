"""
---------Text Engine String Source File----------
To make a string, simply create a variable and make a list of strings. This variable
will be turned into byte code with a tool during the make process.
To put externs and headers in your file for references, you must place them in variables
called externs, and headers, which are also iterable strings.
externs and headers must be have these exact names or will be treated as TE strings.
non iterables are ignored. If you have a tuple with a single item, put a comma after it
or else it will be ignored
"""
#This is externs delcared in this file
externs = ("extern const Gfx star_seg3_dl_0302B870[];",)
#These are header files included in this file. Use single quotes so double quotes are delimited for filename
headers = (r'#include "actors/common1.h"',)
#This keyboard is required for Text Engine usage. Do not delete
TE_KEYBOARD_lower = ["[ShadedBGBox(0x3E,0x104,0x18,0x78,0x20,0x20,0x20,0x80)][ShadedBGBox(0x28,0x118,0x98,0xb8,0x20,0x20,0x20,0x80)][ScaleText(1.75,1.25)][this is a test comment][TransAbs(0x41,96)] 0 1 2 3 4 5 6 7 8 9\n",
#You can also put comments in with a '#'. 
#Just make sure to end the string before hand.
"  q w e r t y u i o p\n\
 a s d f g h j k l :\n\
  z x c v b n m"," & ? !\n\
  ^  SPACE   END  -[TransAbs(0x30,0x9C)]","[StartKeyboard(0)][Pad()][Pad()][StartKeyboard(0)]"]
#This keyboard is required for Text Engine usage. Do not delete
TE_KEYBOARD_upper = ["[ShadedBGBox(0x3E,0x104,0x18,0x78,0x20,0x20,0x20,0x80)]","[ShadedBGBox(0x28,0x118,0x98,0xb8,0x20,0x20,0x20,0x80)][ScaleText(1.75,1.25)]","[TransAbs(0x41,96)] 0 1 2 3 4 5 6 7 8 9\n\
  Q W E R T Y U I O P\n\
 A S D F G H J K L :\n\
  Z X C V B N M & ? !\n\
  ^  SPACE   END  -[TransAbs(0x30,0x9C)]","[StartKeyboard(0)]","[Pad()][Pad()][StartKeyboard(0)]"]
#These are test strings used to test TE features. They can be viewed by setting TE_debug to 1 in
#text_engine.h and pressing D pad down inside a level.
TEST_BOX = ["[ShadedBGBox(32,298,32,228,0x20,0x20,0x20,0x80)][Pop()]"]
TEST_TEX_BOX = ["[MosaicBGBox(32,96,96,160,0x09000000,3,3)][Pop()]"]
TEST_STR = ["[JumpLink('TEST_BOX')][JumpLink('TEST_TEX_BOX')]\
[SetEnv(30,255,255,255)][StartKeyboard(0)][WordWrap(296)]Empy test for now. Nothing happening\
[end]"]

msg_SFX = ["running [UsrStr(0)]\n\
A to inc, B to dec, Z to reset, L to Play.\nC up down to change by 10.\n\
R to stop playing.\n\n\
Start to swap to SEQ control\n\
Play SFX Bank [UsrStr(1)] Index [UsrStr(2)]\n\n\
Load External Sfx [UsrStr(3)] LOAD  (PC ONLY)\n\
File must be named dyncall.bin[end]"]

msg_SEQ = ["running [UsrStr(0)]\n\
A to inc, B to dec, Z to reset, L to Play.\nC up down to change by 10.\n\
R to stop playing.\n\n\
Start to swap to SFX control\n\
Play SEQ [UsrStr(1)]\n\n\
Load External M64 w/ BANK [UsrStr(2)]    LOAD  (PC ONLY)\n\
File must be named test.m64[end]"]