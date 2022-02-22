#include <PR/ultratypes.h>

#include "audio/external.h"
#include "engine/math_util.h"
#include "game/area.h"
#include "game/game_init.h"
#include "game/level_update.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/print.h"
#include "game/save_file.h"
#include "game/sound_init.h"
#include "game/rumble_init.h"
#include "level_table.h"
#include "seq_ids.h"
#include "sm64.h"

#define PRESS_START_DEMO_TIMER 800

#define STUB_LEVEL(textname, _1, _2, _3, _4, _5, _6, _7, _8) textname,
#define DEFINE_LEVEL(textname, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) textname,

static char gLevelSelect_StageNamesText[64][16] = {
    #include "levels/level_defines.h"
};
#undef STUB_LEVEL
#undef DEFINE_LEVEL

static u16 gDemoCountdown = 0;
#ifndef VERSION_JP
static s16 D_U_801A7C34 = 1;
static s16 gameOverNotPlayed = 1;
#endif

// run the demo timer on the PRESS START screen.
// this function will return a non-0 timer once
// the demo starts, signaling to the subsystem that
// the demo needs to be ran.

// don't shift this function from being the first function in the segment.
// the level scripts assume this function is the first, so it cant be moved.
s32 run_press_start_demo_timer(s32 timer) {
    gCurrDemoInput = NULL;

    if (timer == 0) {
        if (!gPlayer1Controller->buttonDown && !gPlayer1Controller->stickMag) {
            if ((++gDemoCountdown) == PRESS_START_DEMO_TIMER) {
                // start the demo. 800 frames has passed while
                // player is idle on PRESS START screen.
#ifndef VERSION_JP
                D_U_801A7C34 = 1;
#endif                                 
                // start the Mario demo animation for the demo list.
                load_patchable_table(&gDemo, gDemoInputListID);

                // if the next demo sequence ID is the count limit, reset it back to
                // the first sequence.
                if (++gDemoInputListID == gDemo.animDmaTable->count) {
                    gDemoInputListID = 0;
                }

                // add 1 (+4) to the pointer to skip the demoID.
                gCurrDemoInput = ((struct DemoInput *) gDemo.targetAnim) + 1;
                timer = (s8)((struct DemoInput *) gDemo.targetAnim)->timer;
                gCurrSaveFileNum = 1;
                gCurrActNum = 1;
            }
        } else { // activity was detected, so reset the demo countdown.
            gDemoCountdown = 0;
        }
    }
    return timer;
}

extern int gDemoInputListID_2;
extern int gPressedStart;

int start_demo(int timer)
{
	gCurrDemoInput = NULL;
	gPressedStart = 0;
    // start the mario demo animation for the demo list.
    //func_80278AD4(&gDemo, gDemoInputListID_2);

    // if the next demo sequence ID is the count limit, reset it back to
    // the first sequence.

    if((++gDemoInputListID_2) == gDemo.animDmaTable->count)
        gDemoInputListID_2 = 0;

    gCurrDemoInput = ((struct DemoInput *) gDemo.targetAnim) + 1; // add 1 (+4) to the pointer to skip the demoID.
    timer = (s8)((struct DemoInput *) gDemo.targetAnim)->timer; // TODO: see if making timer s8 matches
    gCurrSaveFileNum = 1;
    gCurrActNum = 6;
    return timer;
}

// input loop for the level select menu. updates the selected stage
// count if an input was received. signals the stage to be started
// or the level select to be exited if start or the quit combo is
// pressed.

s16 level_select_input_loop(void) {
    s32 stageChanged = FALSE;

    // perform the ID updates per each button press.
    if (gPlayer1Controller->buttonPressed & A_BUTTON) {
        ++gCurrLevelNum, stageChanged = TRUE;
    }
    if (gPlayer1Controller->buttonPressed & B_BUTTON) {
        --gCurrLevelNum, stageChanged = TRUE;
    }
    if (gPlayer1Controller->buttonPressed & U_JPAD) {
        --gCurrLevelNum, stageChanged = TRUE;
    }
    if (gPlayer1Controller->buttonPressed & D_JPAD) {
        ++gCurrLevelNum, stageChanged = TRUE;
    }
    if (gPlayer1Controller->buttonPressed & L_JPAD) {
        gCurrLevelNum -= 10, stageChanged = TRUE;
    }
    if (gPlayer1Controller->buttonPressed & R_JPAD) {
        gCurrLevelNum += 10, stageChanged = TRUE;
    }

    // if the stage was changed, play the sound for changing a stage.
    if (stageChanged) {
        play_sound(SOUND_GENERAL_LEVEL_SELECT_CHANGE, gGlobalSoundSource);
    }

    // TODO: enum counts for the stage lists
    if (gCurrLevelNum > LEVEL_MAX) {
        gCurrLevelNum = LEVEL_MIN; // exceeded max. set to min.
    }

    if (gCurrLevelNum < LEVEL_MIN) {
        gCurrLevelNum = LEVEL_MAX; // exceeded min. set to max.
    }

    gCurrSaveFileNum = 4; // file 4 is used for level select tests
    gCurrActNum = 6;
    print_text_centered(160, 80, "SELECT STAGE");
    print_text_centered(160, 30, "PRESS START BUTTON");
    print_text_fmt_int(40, 60, "%2d", gCurrLevelNum);
    print_text(80, 60, gLevelSelect_StageNamesText[gCurrLevelNum - 1]); // print stage name

#define QUIT_LEVEL_SELECT_COMBO (Z_TRIG | START_BUTTON | L_CBUTTONS | R_CBUTTONS)

    // start being pressed signals the stage to be started. that is, unless...
    if (gPlayer1Controller->buttonPressed & START_BUTTON) {
        // ... the level select quit combo is being pressed, which uses START. If this
        // is the case, quit the menu instead.
        if (gPlayer1Controller->buttonDown == QUIT_LEVEL_SELECT_COMBO) {
            gDebugLevelSelect = FALSE;
            return -1;
        }
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
        return gCurrLevelNum;
    }
    return 0;
}


s32 intro_default(void) {
    s32 sp1C = 0;

#ifndef VERSION_JP
    if (D_U_801A7C34 == 1) {
        if (gGlobalTimer < 0x81) {
            play_sound(SOUND_MARIO_HELLO, gGlobalSoundSource);
        } else {
            play_sound(SOUND_MARIO_PRESS_START_TO_PLAY, gGlobalSoundSource);
        }
        D_U_801A7C34 = 0;
    }
#endif
    print_intro_text();

    if (gPlayer1Controller->buttonPressed & START_BUTTON) {
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
#ifdef RUMBLE_FEEDBACK
        queue_rumble_data(60, 70);
        queue_rumble_decay(1);
#endif
        sp1C = 100 + gDebugLevelSelect;
#ifndef VERSION_JP
        D_U_801A7C34 = 1;
#endif
    }
    return run_press_start_demo_timer(sp1C);
}

s32 intro_game_over(void) {
    s32 sp1C = 0;

#ifndef VERSION_JP
    if (gameOverNotPlayed == 1) {
        play_sound(SOUND_MARIO_GAME_OVER, gGlobalSoundSource);
        gameOverNotPlayed = 0;
    }
#endif

    print_intro_text();

    if (gPlayer1Controller->buttonPressed & START_BUTTON) {
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
#ifdef RUMBLE_FEEDBACK
        queue_rumble_data(60, 70);
        queue_rumble_decay(1);
#endif
        sp1C = 100 + gDebugLevelSelect;
#ifndef VERSION_JP
        gameOverNotPlayed = 1;
#endif
    }
    return run_press_start_demo_timer(sp1C);
}
static u8 running = 0;
static u16 Bank = 0;
static u16 Index = 0;
static u16 m64 = 1;
u16 Extm64 = 12;
static s8 selected = 0;
static s8 type = -1; //0 for m64, 1 for sfx
extern void stop_sounds_in_bank(u8 bank);
extern u16 gSequenceCount;
#include "src/game/Keyboard_te.h"
#include "src/game/text_engine.h"
#include "src/audio/load.h"
#ifndef TARGET_N64
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *ptr_sfx_dyncall;
u8 *dyncall_read;
#endif

static void Control_SFX(void){
	switch(selected){
		case 0:
			if(gPlayer3Controller->buttonPressed & U_CBUTTONS){
				Bank += 10;
			}if(gPlayer3Controller->buttonPressed & D_CBUTTONS){
				Bank -= 10;
			}if(gPlayer3Controller->buttonPressed & A_BUTTON){
				Bank += 1;
			}if(gPlayer3Controller->buttonPressed & B_BUTTON){
				Bank -= 1;
			}if(gPlayer3Controller->buttonPressed & Z_TRIG){
				Bank = 0;
			}if(gPlayer3Controller->buttonPressed & L_TRIG){
				play_sound(SOUND_ARG_LOAD(Bank,Index, 0xFF, SOUND_DISCRETE), gGlobalSoundSource);
			}if(gPlayer3Controller->buttonPressed & R_TRIG){
				stop_sounds_in_bank(Bank);
				stop_sounds_in_continuous_banks();
			}
			break;
		case 1:
			if(gPlayer3Controller->buttonPressed & U_CBUTTONS){
				Index += 10;
			}if(gPlayer3Controller->buttonPressed & D_CBUTTONS){
				Index -= 10;
			}if(gPlayer3Controller->buttonPressed & A_BUTTON){
				Index += 1;
			}if(gPlayer3Controller->buttonPressed & B_BUTTON){
				Index -= 1;
			}if(gPlayer3Controller->buttonPressed & Z_TRIG){
				Index = 0;
			}if(gPlayer3Controller->buttonPressed & L_TRIG){
				play_sound(SOUND_ARG_LOAD(Bank,Index, 0xFF, SOUND_DISCRETE), gGlobalSoundSource);
			}if(gPlayer3Controller->buttonPressed & R_TRIG){
				stop_sounds_in_bank(Bank);
				stop_sounds_in_continuous_banks();
			}
			break;
		#ifndef TARGET_N64
		case 2:
			//load external file
			if(gPlayer3Controller->buttonPressed & L_TRIG){
				ptr_sfx_dyncall = fopen("dyncall.bin","rb");
				if (ptr_sfx_dyncall){
					printf("file loaded %p\n",ptr_sfx_dyncall);
					int i;
					fseek(ptr_sfx_dyncall,0,SEEK_END);
					i = ftell(ptr_sfx_dyncall);
					fseek(ptr_sfx_dyncall,0,0);
					dyncall_read = malloc(i);
					fread(dyncall_read,i,1,ptr_sfx_dyncall);
					fclose(ptr_sfx_dyncall);
					play_sound(SOUND_ARG_LOAD(10,0, 0xFF, SOUND_DISCRETE), gGlobalSoundSource);
				}else{
					printf("file not found!\n");
				}
			}
			if(gPlayer3Controller->buttonPressed & R_TRIG){
				stop_sounds_in_bank(Bank);
				stop_sounds_in_continuous_banks();
			}
			break;
		case 3:
			selected = 0;
			break;
		#else
		case 2:
			selected = 0;
			break;
		#endif
	}
	u8 i = 0;
	if(selected == 0){
		UserInputs[0][1][0] = 0x53;
	}else{
		UserInputs[0][1][0] = 0x9E;
	}
	UserInputs[0][1][1] = (u8)Bank/10;
	UserInputs[0][1][2] = (u8)Bank%10;
	UserInputs[0][1][3] = 0x45;
	if(selected == 1){
		UserInputs[0][2][0] = 0x53;
	}else{
		UserInputs[0][2][0] = 0x9E;
	}
	UserInputs[0][2][1] = (u8)Index/10;
	UserInputs[0][2][2] = (u8)Index%10;
	UserInputs[0][2][3] = 0x45;
	#ifndef TARGET_N64
	if(selected == 2){
		UserInputs[0][3][0] = 0x53;
	}else{
		UserInputs[0][3][0] = 0x9E;
	}
	UserInputs[0][3][1] = 0x45;
	handle_menu_scrolling(2,&selected,0,3);
	#else
	handle_menu_scrolling(2,&selected,0,2);
	#endif
}

static void Control_SEQ(void){
	switch(selected){
		case 0:
			if(gPlayer3Controller->buttonPressed & U_CBUTTONS){
				m64 += 10;
			}if(gPlayer3Controller->buttonPressed & D_CBUTTONS){
				m64 -= 10;
			}if(gPlayer3Controller->buttonPressed & A_BUTTON){
				m64 += 1;
			}if(gPlayer3Controller->buttonPressed & B_BUTTON){
				m64 -= 1;
			}if(gPlayer3Controller->buttonPressed & Z_TRIG){
				m64 = 0;
			}if(gPlayer3Controller->buttonPressed & L_TRIG){
				play_music(1, SEQUENCE_ARGS(4, m64), 0);
			}if(gPlayer3Controller->buttonPressed & R_TRIG){
				play_music(1, 0, 0);
			}
			break;
		#ifndef TARGET_N64
		case 1:
			//sequence reading/loading is done in load.c around line 1440
			if(gPlayer3Controller->buttonPressed & U_CBUTTONS){
				Extm64 += 10;
			}if(gPlayer3Controller->buttonPressed & D_CBUTTONS){
				Extm64 -= 10;
			}if(gPlayer3Controller->buttonPressed & A_BUTTON){
				Extm64 += 1;
			}if(gPlayer3Controller->buttonPressed & B_BUTTON){
				Extm64 -= 1;
			}if(gPlayer3Controller->buttonPressed & Z_TRIG){
				Extm64 = 0;
			}if(gPlayer3Controller->buttonPressed & L_TRIG){
				play_music(1, SEQUENCE_ARGS(4, gSequenceCount+1), 0);
			}if(gPlayer3Controller->buttonPressed & R_TRIG){
				play_music(1, 0, 0);
			}
			break;
		case 2:
			selected = 0;
			break;
			#endif
	}
	if(selected == 0){
		UserInputs[0][1][0] = 0x53;
	}else{
		UserInputs[0][1][0] = 0x9E;
	}
	UserInputs[0][1][1] = (u8)m64/10;
	UserInputs[0][1][2] = (u8)m64%10;
	UserInputs[0][1][3] = 0x45;
	#ifndef TARGET_N64
	if(selected == 1){
		UserInputs[0][2][0] = 0x53;
	}else{
		UserInputs[0][2][0] = 0x9E;
	}
	UserInputs[0][2][1] = (u8)Extm64/10;
	UserInputs[0][2][2] = (u8)Extm64%10;
	UserInputs[0][2][3] = 0x45;
	handle_menu_scrolling(2,&selected,0,2);
	#endif
}

s32 intro_play_its_a_me_mario(void) {
    //do controls for sound player here
	if(type == -1){
		SetupTextEngine(16,200,&msg_SEQ,0);
		type = 0;
	}
	running += 1;
	UserInputs[0][0][0] = running&3;
	UserInputs[0][0][2] = 0x45;
	switch(type){
		case 0:
			Control_SEQ();
			if(gPlayer3Controller->buttonPressed & START_BUTTON){
				TE_end_str(&TE_Engines[0]);
				SetupTextEngine(16,200,&msg_SFX,0);
				type = 1;
			}
			break;
		case 1:
			Control_SFX();
			if(gPlayer3Controller->buttonPressed & START_BUTTON){
				TE_end_str(&TE_Engines[0]);
				SetupTextEngine(16,200,&msg_SEQ,0);
				type = 0;
			}
			break;
	}
	return 0;
}

s32 lvl_intro_update(s16 arg1, UNUSED s32 arg2) {
    s32 retVar;

    switch (arg1) {
        case 0:
            retVar = intro_play_its_a_me_mario();
            break;
        case 1:
            retVar = intro_default();
            break;
        case 2:
            retVar = intro_game_over();
            break;
        case 3:
            retVar = level_select_input_loop();
            break;
    }
    return retVar;
}
