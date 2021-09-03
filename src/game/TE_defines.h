#define TE_DEBUG 1
#define NumEngines 3

#define TE_STATE_MAIN 0
#define TE_STATE_AUX 1
#define TE_STATE_BG 2
#define IS_TE_CMD(Char) \
(Char<0x40||\
(Char>0x4F&&Char<0x70)||\
(Char>0xCF&&Char<0xFE)\
||Char==0x9E||Char==0x9F)

//DL print macros, used in order of Translate, Scale, Rotate
#define DL_TRAN_NONE 0 //Followed by nothing
#define DL_TRAN_CONST 1 //Followed by constant vec3s Loc is in Signed 7.8. Basically meaning 0x100 equals 1.
#define DL_TRAN_VEL 2 //Followed by velocity vec3s. Vel is in Signed 7.8. Basically meaning 0x100 equals 1.
#define DL_TRAN_CYCLE 3 //Followed by velocity vec3s, cycle time and cycle type. Vel is in Signed 7.8. Basically meaning 0x100 equals 1.

#define CYCLE_LOOP 0 //starts over from beginning
#define CYCLE_REVERSE 1 //goes in reverse after cycle end
#define CYCLE_SINE 2 //sinusoidal

/* A dictionary of all the TE commands and their explanations is below as well as arg inputs */