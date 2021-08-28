#ifdef TE
#include <PR/ultratypes.h>
#include <PR/gbi.h>
#include "gfx_dimensions.h"
#include "print.h"
#include "rendering_graph_node.h"
#include "sm64.h"
#include "main.h"
#include "mario.h"
#include "level_update.h"
#include "engine/math_util.h"
#include "object_list_processor.h"
#include "area.h"
#include "audio/external.h"
#include "text_engine.h"
#include "ingame_menu.h"
#include "segment2.h"
#include "game_init.h"
#include "object_helpers.h"
#include "puppyprint.h"
#include "rendering_graph_node.h"

extern u8 gDialogCharWidths[256];
extern struct MarioState *gMarioState;
extern s16 sDelayedWarpOp;
extern s16 sDelayedWarpTimer;
extern s16 sSourceWarpNodeId;

volatile struct TEState TE_Engines[NumEngines];
u8 StrBuffer[NumEngines][0x100];
u8 CmdBuffer[NumEngines][0x400];
u32 TimerBuffer[NumEngines][64]; //stores timers necessary for certain cmds with their own cycles and stuff
u8 UserInputs[NumEngines][16][16]; //16 length 16 strings
//object array
u32 FunctionReturns[NumEngines][8];
static struct TEState *AccessEngine; //for outside functions to access
//during callback functions

//my char and ptr arrays
#include "src/game/Keyboard_te.py"
#include "src/game/TE_strings.inc.h"
#include "text_engine_cmds.inc.h"
#include "text_engine_helpers.inc.h"

//SR11 specific funcs
u32 PrintAnswer(void){
	return gCurrentArea->index;
}
u32 DetermineAnswer(u8 answer){
	if(AccessEngine->ReturnedDialog == answer)
		return 1;
	else
		return 0;
}
u32 DamageAnswer(u8 answer){
	if(AccessEngine->ReturnedDialog == answer)
		return 1;
	else{
		gMarioState->health-=0x0300;
		return 0;
	}
}

void SetupTextEngine(s16 x, s16 y, u8 *str, u8 state){
	TE_flush_eng(&TE_Engines[state]);
	str = segmented_to_virtual(str);
	TE_Engines[state].state = state;
	TE_Engines[state].LastVI = gNumVblanks;
	TE_Engines[state].OgStr = str;
	TE_Engines[state].TempStr = str;
	TE_Engines[state].BufferStrPtr = str;
	TE_Engines[state].StartX = x;
	TE_Engines[state].StartY = y;
	TE_Engines[state].OgSeqID = -1;
	TE_Engines[state].NewSeqID = -1;
	TE_Engines[state].StackDepth = 0;
	TE_Engines[state].NewSpeed = 0x1234;
	TE_Engines[state].EnvColorWord =- 1;
}

void RunTextEngine(void){
	u8 i;
	struct TEState *CurEng;
	register u32 CurVI = gNumVblanks;
	u16 CharsThisFrame;
	u8 CurChar;
	s8 loop;
	u8 *str;
	//for puppyprint
	asciiToggle = 1;
	for(i=0;i<NumEngines;i++){
		CurEng = &TE_Engines[i];
		AccessEngine = CurEng;
		if (CurEng->OgStr==0){
			continue;
		}
		//there is an engine
		CurEng->TempStr = CurEng->OgStr;
		if (CurEng->VICounter!=CurVI){
			CurEng->VICounter=CurVI;
		}else{
			continue;
		}
		TE_setup_ia8();
		//init TE state vars
		CharsThisFrame = 1;
		TE_frame_init(CurEng);
		#if TE_DEBUG
		TE_debug_print(CurEng);
		#endif
		//run until either the string ends or you need to wait to draw more chars.
		loop:
			CurChar = CurEng->TempStr[CurEng->CurPos];
			str = &CurEng->TempStr[CurEng->CurPos];
			//no draw
			if (CurEng->BlankTimer>CurVI){
				loop = TE_blank_time(CurEng,str);
				goto loopswitch;
			}
			//draw keyboard
			if (CurEng->KeyboardState==1){
				loop = TE_draw_keyboard(CurEng,str);
				goto loopswitch;
			}
			if (CurEng->KeyboardState==5){
				loop = TE_make_keyboard(CurEng,str);
				goto loopswitch;
			}
			if(!(CurChar<0x40||(CurChar>0x4F&&CurChar<0x70)||(CurChar>0xCF&&CurChar<0xFE)||CurChar==0x9E||CurChar==0x9F)){
				loop = TE_jump_cmds(CurEng,CurChar,str);
				loopswitch:
					if (loop==1)
						goto loop;
					else if (loop==0)
						goto nonewchar;
					else if (loop==-1)
						goto printnone;
					else if (loop==-2)
						goto printnone;
			}
			//keep track of current keyboard index while drawing keyboard
			if (CurEng->KeyboardState==2 && CurChar!=0x9E){
				//the worst conditional of all time
				if(CurEng->IntendedLetter>=41 && CurChar == 0x19 && CurEng->KeyboardChar==42){
					CurEng->KeyboardChar-=4;
				}
				//end
				if(CurEng->IntendedLetter>=41 && CurChar == 0x17 && CurEng->KeyboardChar==43){
					CurEng->KeyboardChar-=2;
				}
				CurEng->KeyboardChar+=1;
				if((CurEng->KeyboardChar-1)==CurEng->IntendedLetter){
					loop = TE_keyboard_sel(CurEng,str,1);
					TE_add_char2buf(CurEng);
					goto loopswitch;
				}
				if((CurEng->KeyboardChar-2)==CurEng->IntendedLetter){
					loop = TE_keyboard_sel(CurEng,str,0);
					goto loopswitch;
				}
				TE_add_char2buf(CurEng);
				goto loop;
			}
			//normal character is detected
			if(CurEng->TempStrEnd!=CurEng->CurPos){
				TE_add_char2buf(CurEng);
				goto loop;
			}
			//now I have to check if a new character has to be drawn
			else{
				//check char speed for neg
				s16 TEspd = getTEspd(CurEng);
				if(TEspd<0){
					if(((CurVI*absi(TEspd)))>=((CurEng->LastVI*absi(TEspd))+CharsThisFrame)){
						//draw a new char
						TE_add_new_char(CurEng,CurEng->LastVI);
						CharsThisFrame++;
						goto loop;
					}else if(CharsThisFrame>1){
						CurEng->LastVI = CurVI;
					}
				}else{
					if(CurVI>=(CurEng->LastVI+TEspd)){
						//draw a new char
						TE_add_new_char(CurEng,CurVI+TEspd);
						goto loop;
					}
				}
			}
		//no new char. end loop
		nonewchar:
		TE_print(CurEng);
		printnone:
		// CurEng->PlainText = 0;
		if(CurEng->ScissorSet){
			gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			CurEng->ScissorSet = 0;
		}
		if(CurEng->ShakeScreen){
			set_camera_shake_from_point(SHAKE_POS_SMALL,gMarioState->pos[0],gMarioState->pos[1],gMarioState->pos[2]);
		}
		TE_flush_str_buff(CurEng);
		TE_end_ia8();
		//write end string DL ptr
		CurEng->VICounter = 0;
		CurEng->CurPos = 0;
		// end:
	}
	//for puppyprint
	asciiToggle = 0;
}

//inits the variables needed at the start of a frame
void TE_frame_init(struct TEState *CurEng){
	CurEng->TotalXOff=0;
	CurEng->TempX = CurEng->StartX;
	CurEng->TempY = CurEng->StartY;
	CurEng->TempXOrigin = CurEng->StartX;
	CurEng->TempYOrigin = CurEng->StartY;
	CurEng->TempStrEnd = CurEng->StrEnd;
	CurEng->TempStr = CurEng->OgStr;
	CurEng->SfxArg = 0;
	CurEng->ScaleF[0] = 1.0f;
	CurEng->ScaleF[1] = 1.0f;
	CurEng->StackDepth = CurEng->StackLocked;
	CurEng->ShakeScreen = 0;
	StrBuffer[CurEng->state][0] = 0xFF;
}

void TE_transition_print(struct TEState *CurEng){
	if(CurEng->TrEnd.TransVI != 0){
		CurEng->LastVI = gNumVblanks;
		if(CurEng->TrEnd.TransLength == 0){
			TE_set_env(CurEng);
			// print_generic_string(CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY, &StrBuffer[CurEng->state]);
			print_small_text(CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY, &StrBuffer[CurEng->state], PRINT_TEXT_ALIGN_LEFT, PRINT_ALL);
			return;
		}else{
			TE_transition_active(CurEng,&CurEng->TrEnd,0);
			return;
		}
	}
	if(CurEng->TrStart.TransVI != 0){
		if(CurEng->TrStart.TransLength == 0){
			TE_set_env(CurEng);
			// print_generic_string(CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY,&StrBuffer[CurEng->state]);
			print_small_text(CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY,&StrBuffer[CurEng->state], PRINT_TEXT_ALIGN_LEFT, PRINT_ALL);
			return;
		}else{
			TE_transition_active(CurEng,&CurEng->TrStart,1);
			return;
		}
	}
	TE_set_env(CurEng);
	// print_generic_string(CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY,&StrBuffer[CurEng->state]);
	print_small_text(CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY,&StrBuffer[CurEng->state], PRINT_TEXT_ALIGN_LEFT, PRINT_ALL);
	return;
}
void TE_transition_active(struct TEState *CurEng,struct Transition *Tr,u8 flip){
	//on a start transition, you start from the struct alpha and end at env stored one
	s16 TarAlpha;
	s16 CurAlpha;
	if (flip){
		TarAlpha = CurEng->EnvColorByte[3];
		CurAlpha = Tr->TransAlpha;
	}else{
		CurAlpha = CurEng->EnvColorByte[3];
		TarAlpha = Tr->TransAlpha;
	}
	u32 Env = CurEng->EnvColorWord;
	u32 Time = (gNumVblanks-Tr->TransVI);
	f32 Pct = ((f32) Time) / ((f32) Tr->TransLength);
	f32 Spd = ((f32)Tr->TransSpeed)/((f32) Tr->TransLength);
	u16 Dir = Tr->TransDir<<16;
	u16 Yoff = (u16) (sins(Dir)*Spd*Time);
	u16 Xoff = (u16) (coss(Dir)*Spd*Time);
	//should never be true really
	if (Pct>1.0f){
		Pct = 1.0f;
		Time = Tr->TransLength;
		//disable start transition and set temp X+Y offsets to match total distance traveled
		if (flip){
			Tr->TransVI = 0;
			CurEng->TransX = Xoff;
			CurEng->TransY = Yoff;
			Xoff = 0;
			Yoff = 0;
		}
	}
	TarAlpha = CurAlpha + (TarAlpha-CurAlpha)*Pct;
	CurEng->EnvColorWord = (CurEng->EnvColorWord&0xFFFFFF00) | (u8) TarAlpha;
	TE_set_env(CurEng);
	CurEng->EnvColorWord = Env;
	if (flip){
		CurEng->TrPct = 1.0f-Pct;
	}else{
		CurEng->TrPct = Pct;
	}
	// print_generic_string(CurEng->TempX+Xoff+CurEng->TransX,CurEng->TempY+Yoff+CurEng->TransY,&StrBuffer[CurEng->state]);
	print_small_text(CurEng->TempX+Xoff+CurEng->TransX,CurEng->TempY+Yoff+CurEng->TransY,&StrBuffer[CurEng->state], PRINT_TEXT_ALIGN_LEFT, PRINT_ALL);
}

void TE_print(struct TEState *CurEng){
	//deal with case where buffer is empty
	if(!(StrBuffer[CurEng->state][0] == 0xFF)){
		//print shadow with plaintext
		if(CurEng->PlainText){
			u32 Env = CurEng->EnvColorWord;
			CurEng->EnvColorWord = 0x10101000 | CurEng->EnvColorByte[3];
			CurEng->TempX += 1;
			CurEng->TempY -= 1;
			TE_transition_print(CurEng);
			CurEng->TempX -= 1;
			CurEng->TempY += 1;
			CurEng->EnvColorWord = Env;
		}
		TE_transition_print(CurEng);
		TE_flush_str_buff(CurEng);
		TE_reset_Xpos(CurEng);
		gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
		create_dl_scale_matrix(MENU_MTX_PUSH, CurEng->ScaleF[0], CurEng->ScaleF[1], 1.0f);
		TE_fix_scale_Xpos(CurEng);
	}
}

void TE_add_new_char(struct TEState *CurEng,u32 VI_inc){
	//I should use macros here but I'm not really sure how they work
	if(CurEng->SfxArg){
		play_sound((CurEng->SfxArg<<16)+0x81, gGlobalSoundSource);
	}else if(CurEng->CheckBlip){
		play_sound(0x16FF81, gGlobalSoundSource);
	}
	CurEng->StrEnd+=1;
	CurEng->TempStrEnd+=1;
	CurEng->LastVI = VI_inc;
	TE_add_char2buf(CurEng);
}

void TE_add_char2buf(struct TEState *CurEng){
	u8 CharWrite;
	//get char
	CharWrite = CurEng->TempStr[CurEng->CurPos];
	//increase X pos
	s32 textX, textY, offsetY, spaceX;
	get_char_from_byte_sm64(CharWrite,&textX, &textY, &spaceX, &offsetY);
	CurEng->TotalXOff+=spaceX+1;
	//write char to buffer
	StrBuffer[CurEng->state][CurEng->CurPos] = CharWrite;
	StrBuffer[CurEng->state][CurEng->CurPos+1] = 0xFF;
	CurEng->CurPos+=1;
}

void TE_add_to_cmd_buffer(struct TEState *CurEng,u8 *str,u8 len){
	u32 i;
	union PtrByte Offset;
	Offset.ptr = str;
	for(i=0;i<4;i++){
		CmdBuffer[CurEng->state][CurEng->BufferPos+i] = Offset.bytes[i];
	}
	CmdBuffer[CurEng->state][CurEng->BufferPos+4] = len;
	for(i=0;i<len;i++){
		CmdBuffer[CurEng->state][CurEng->BufferPos+i+5] = str[i];
		str[i] = 0x9D;
	}
	CurEng->BufferPos += len+5;
}

void TE_flush_buffers(struct TEState *CurEng){
	TE_clear_cmd_buffer(CurEng);
	TE_clear_timer_buffer(CurEng);
	TE_flush_eng(CurEng);
	TE_flush_str_buff(CurEng);
}

void TE_flush_eng(struct TEState *CurEng){
	bzero(CurEng, sizeof(*CurEng));
}

void TE_clear_timer_buffer(struct TEState *CurEng){
	u32 i;
	u32 *T = TimerBuffer[CurEng->state];
	u8 *str;
	for(i=0;i<CurEng->BufferTimePtr;i+=2){
		str = T[i];
		str[0] = 0;
		str[1] = 0;
		*(T+i) = 0;
		*(T+i+1) = 0;
	}
	CurEng->BufferTimePtr = 0;
}

void TE_clear_cmd_buffer(struct TEState *CurEng){
	u32 i;
	union PtrByte Offset;
	u32 n = 0;
	u8 *str;
	u8 len;
	while(1){
		for(i=0;i<4;i++){
			Offset.bytes[i] = CmdBuffer[CurEng->state][i+n];
			CmdBuffer[CurEng->state][i+n] = 0;
		}
		len = CmdBuffer[CurEng->state][4+n];
		if (len == 0){
			break;
		}
		CmdBuffer[CurEng->state][4+n] = 0;
		for(i=0;i<len;i++){
			Offset.ptr[i] = CmdBuffer[CurEng->state][i+5+n];
			CmdBuffer[CurEng->state][i+5+n] = 0;
		}
		n += 5+len;
	}
	CurEng->BufferPos = 0;
}

void TE_flush_str_buff(struct TEState *CurEng){
	u32 i;
	for(i=0;i<0x100;i++){
		if (StrBuffer[CurEng->state][i]==0){
			break;
		}
		StrBuffer[CurEng->state][i] = 0;
	}
	StrBuffer[CurEng->state][0]=0xFF;
}
s16 getTEspd(struct TEState *CurEng){
	if(gPlayer1Controller->buttonDown&A_BUTTON && CurEng->NewSpeed!=0x1234){
		return CurEng->NewSpeed;
	}else{
		return CurEng->VIpChar;
	}
	
}
void TE_set_env(struct TEState *CurEng){
	print_set_envcolour(CurEng->EnvColorByte[0], CurEng->EnvColorByte[1], CurEng->EnvColorByte[2], CurEng->EnvColorByte[3]);
}

void TE_reset_Xpos(struct TEState *CurEng){
	CurEng->TempX += CurEng->TotalXOff*CurEng->ScaleF[0];
	CurEng->TotalXOff = 0;
}

void TE_fix_scale_Xpos(struct TEState *CurEng){
	create_dl_translation_matrix(MENU_MTX_NOPUSH, ((f32)CurEng->TempX)*((1.0f/CurEng->ScaleF[0])-1.0f), ((f32)(CurEng->TempY)*((1.0f/CurEng->ScaleF[1])-1.0f)), 0);
}

//gets str ready to display characters
void TE_setup_ia8(void){
	create_dl_ortho_matrix();
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
}
void TE_end_ia8(void){
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

u16 TE_get_u16(u8 *str){
	u16 res;
	res = (str[1]<<8)+str[2];
	return res;
}

s16 TE_get_s16(u8 *str){
	s16 res;
	res = (str[1]<<8)+str[2];
	return res;
}

s32 TE_get_s32(u8 *str){
	s32 res;
	res = (str[1]<<24)+(str[2]<<16)+(str[3]<<8)+str[4];
	return res;
}

u32 TE_get_u32(u8 *str){
	u32 res;
	res = (str[1]<<24)+(str[2]<<16)+(str[3]<<8)+str[4];
	return res;
}

u32 TE_get_ptr(u8 *strArgs,u8 *str){
	u16 pos = TE_get_u16(strArgs);
	u16 ptrID = TE_get_u16(strArgs+2);
	str = (u32)str-4-pos;
	u32 **Ptrptr = str;
	u32 *ptr =  segmented_to_virtual(*Ptrptr);
	return ptr[ptrID];
}

#if TE_DEBUG
extern uintptr_t sSegmentTable[32];
void TE_debug_print(struct TEState *CurEng){
	u8 buf[32];
	if (gPlayer1Controller->buttonDown&L_TRIG){
		sprintf(buf,"col %d",CurEng->EnvColorWord);
		print_text(32,64,buf);
		sprintf(buf,"og %d",CurEng->OgSeqID);
		print_text(32,128,buf);
		sprintf(buf,"param %d",gCurrentArea->musicParam);
		print_text(32,96,buf);
	}
	
}
#endif


#endif