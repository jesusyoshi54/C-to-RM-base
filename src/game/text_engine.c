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

struct TEState TE_Engines[NumEngines];
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

void SetupTextEngine(s16 x, s16 y, u8 *str, u8 state){
	TE_flush_buffers(&TE_Engines[state]);
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
	TE_Engines[state].EnvColorWord = -1;
	TE_Engines[state].PrevEnvColorWord =-1;
	TE_Engines[state].WordWrap = 0;
}

void RunTextEngine(void){
	u8 i;
	struct TEState *CurEng;
	register u32 CurVI = gNumVblanks;
	u16 CharsThisFrame;
	u8 CurChar;
	s8 loop;
	u8 *str;
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
			//printing ascii, no cmds should be encountered
			if(CurEng->Ascii){
				if(CurChar == 0xFF){
					//basically pop str
					TE_print(CurEng);
					CurEng->CurPos = 0;
					CurEng->TempStr = CurEng->AsciiRet;
					CurEng->Ascii = 0;
					goto loop;
				}
				//deal with newline so spacing doesn't get bonked
				if (str[i] == '\n'){
					TE_line_break(CurEng,str);
					goto loop;
				}
				goto normal_char;
			}

			if (CurEng->KeyboardState==5){
				loop = TE_make_keyboard(CurEng,str);
				goto loopswitch;
			}
			if(!IS_TE_CMD(CurChar)){
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
			//draw keyboard but after processing other cmds. Can cause conflicts but allows
			//effects. Rely on users not messing up (impossible)
			if (CurEng->KeyboardState==1){
				loop = TE_draw_keyboard(CurEng,str);
				goto loopswitch;
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
			normal_char:
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
	CurEng->TransformStackPos = 0;
	CurEng->EnvColorWord =- 1;
	CurEng->PrevEnvColorWord =- 1;
}

void TE_transition_print(struct TEState *CurEng){
	if(CurEng->TrEnd.TransVI != 0){
		CurEng->LastVI = gNumVblanks;
		if(CurEng->TrEnd.TransLength == 0){
			TE_set_env(CurEng);
			print_small_text_TE(CurEng->ScaleF[0],CurEng->ScaleF[1],CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY, &StrBuffer[CurEng->state],CurEng);
			return;
		}else{
			TE_transition_active(CurEng,&CurEng->TrEnd,0);
			return;
		}
	}
	if(CurEng->TrStart.TransVI != 0){
		if(CurEng->TrStart.TransLength == 0){
			TE_set_env(CurEng);
			print_small_text_TE(CurEng->ScaleF[0],CurEng->ScaleF[1],CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY,&StrBuffer[CurEng->state],CurEng);
			return;
		}else{
			TE_transition_active(CurEng,&CurEng->TrStart,1);
			return;
		}
	}
	TE_set_env(CurEng);
	print_small_text_TE(CurEng->ScaleF[0],CurEng->ScaleF[1],CurEng->TempX+CurEng->TransX,CurEng->TempY+CurEng->TransY,&StrBuffer[CurEng->state],CurEng);
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
	s32 Env = CurEng->EnvColorWord;
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
	CurEng->TrPct = Pct;
	// print_generic_string(CurEng->TempX+Xoff+CurEng->TransX,CurEng->TempY+Yoff+CurEng->TransY,&StrBuffer[CurEng->state]);
	print_small_text_TE(CurEng->ScaleF[0],CurEng->ScaleF[1],CurEng->TempX+Xoff+CurEng->TransX,CurEng->TempY+Yoff+CurEng->TransY,&StrBuffer[CurEng->state],CurEng);
}

void TE_print(struct TEState *CurEng){
	//deal with case where buffer is empty
	if(!(StrBuffer[CurEng->state][0] == 0xFF)){
		//print shadow with plaintext
		if(CurEng->PlainText){
			s32 Env = CurEng->EnvColorWord;
			CurEng->EnvColorWord = 0x10101000 | CurEng->EnvColorByte[3];
			CurEng->TempX += 1;
			CurEng->TempY -= 1;
			TE_transition_print(CurEng);
			CurEng->TempX -= 1;
			CurEng->TempY += 1;
			CurEng->PrevEnvColorWord = CurEng->EnvColorWord;
			CurEng->EnvColorWord = Env;
		}
		TE_transition_print(CurEng);
		CurEng->PrevEnvColorWord = CurEng->EnvColorWord;
		TE_flush_str_buff(CurEng);
		TE_reset_Xpos(CurEng);
	}
}

void TE_add_new_char(struct TEState *CurEng,u32 VI_inc){
	//I should use macros here but I'm not really sure how they work
	if(CurEng->SfxArg){
		play_sound((CurEng->SfxArg<<16)+0x81, gGlobalSoundSource);
		CurEng->SfxArg = 0;
	}else if(CurEng->CheckBlip && getTEspd(CurEng)){
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
	if(!CurEng->Ascii){
		get_char_from_byte_sm64(CharWrite,&textX, &textY, &spaceX, &offsetY);
	}else{
		get_char_from_byte_ascii(CharWrite,&textX, &textY, &spaceX, &offsetY);
	}
	CurEng->TotalXOff += ((u32)(spaceX*CurEng->ScaleF[0]))+1;
	//write char to buffer
	StrBuffer[CurEng->state][CurEng->CurPos] = CharWrite;
	StrBuffer[CurEng->state][CurEng->CurPos+1] = 0xFF;
	CurEng->CurPos+=1;
	//word wrap when next letter will be over word wrap var
	u8 space;
	u8 new;
	if(CurEng->Ascii){
		space = ' ';
		new = '\n';
	}else{
		space = 0x9E;
		new = 0xFE;
	}
	if (CurEng->WordWrap){
		u8 Nxt = TE_find_next_space(CurEng,CurEng->TempStr);
		if((CurEng->TempX+(u32)((CurEng->TotalXOff+(Nxt*8))*CurEng->ScaleF[0]))>(CurEng->WordWrap)){
			TE_line_break(CurEng,CurEng->OgStr);
			if(!((CurEng->TempStr[CurEng->CurPos-1]==space)||(CurEng->TempStr[CurEng->CurPos-1]==new))){
				CurEng->TempStr-=1;
			}
		}
	}
}
u8 TE_find_next_space(struct TEState *CurEng,u8 *str){
	u8 x = 0;
	u8 CharWrite = str[CurEng->CurPos+x];
	u8 space;
	if(CurEng->Ascii){
		space = ' ';
	}else{
		space = 0x9E;
	}
	while(CharWrite != space){
		CharWrite = str[CurEng->CurPos+x];
		//generic
		if(!IS_TE_CMD(CharWrite) && (!CurEng->Ascii && (CharWrite==0xFF | CharWrite=='\n'))){
			break;
		}
		x++;
		if(x>100){
			break; //fail condition
		}
	}
	return x;
}
void TE_add_to_cmd_buffer(struct TEState *CurEng,u8 *str,u8 len){
	u32 i;
	union PtrByte Offset;
	Offset.ptr = str;
	for(i=0;i<sizeof(u8*);i++){
		CmdBuffer[CurEng->state][CurEng->BufferPos+i] = Offset.bytes[i];
	}
	CmdBuffer[CurEng->state][CurEng->BufferPos+sizeof(u8*)] = len;
	for(i=0;i<len;i++){
		CmdBuffer[CurEng->state][CurEng->BufferPos+i+sizeof(u8*)+1] = str[i];
		str[i] = 0x9D;
	}
	CurEng->BufferPos += len+sizeof(u8*)+1;
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
		for(i=0;i<sizeof(u8*);i++){
			Offset.bytes[i] = CmdBuffer[CurEng->state][i+n];
			CmdBuffer[CurEng->state][i+n] = 0;
		}
		len = CmdBuffer[CurEng->state][4+n];
		if (len == 0){
			break;
		}
		CmdBuffer[CurEng->state][4+n] = 0;
		for(i=0;i<len;i++){
			Offset.ptr[i] = CmdBuffer[CurEng->state][i+sizeof(u8*)+1+n];
			CmdBuffer[CurEng->state][i+sizeof(u8*)+1+n] = 0;
		}
		n += sizeof(u8*)+1+len;
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
	if (CurEng->PrevEnvColorWord!=CurEng->EnvColorWord)
		gDPSetEnvColor(gDisplayListHead++,CurEng->EnvColorByte[0], CurEng->EnvColorByte[1], CurEng->EnvColorByte[2], CurEng->EnvColorByte[3]);
}

void TE_reset_Xpos(struct TEState *CurEng){
	CurEng->TempX += CurEng->TotalXOff;
	CurEng->TotalXOff = 0;
}

//gets str ready to display characters
void TE_setup_ia8(void){
	create_dl_ortho_matrix();
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
}
void TE_end_ia8(void){
	gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

static s8 shakeToggle = 0;
static s8 waveToggle = 0;
static u8 currEnv[4];
void print_small_text_TE(f32 xScale, f32 yScale, s32 x, s32 y, const char *str,struct TEState *CurEng)
{
    s32 textX = 0;
    s32 textY = 0;
    s32 offsetY = 0;
    s32 i = 0;
    s32 textPos[2] = {0,0};
    s32 spaceX = 0;
    s32 wideX[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    s32 tx = 1000; //just generic limit. Should be stopped by terminator way earlier
    s32 shakePos[2];
    s32 wavePos;
    s32 lines = 0;
    s32 xlu = currEnv[3];
    s32 prevxlu = 256; //Set out of bounds, so it will *always* be different at first.

	gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
	gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
    gDPLoadTextureBlock_4b(gDisplayListHead++, segmented_to_virtual(small_font), G_IM_FMT_I, 128, 60, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 0, 0, 0, 0, 0);
	u32 dsdx = (u32)((1.0f/xScale)*1024.0f);
	u32 dsdy = (u32)((1.0f/yScale)*1024.0f);
	s32 xInc = (s32) (8.0f*xScale);
	s32 yInc = (s32) (12.0f*yScale);
    for (i = 0; i < tx; i++)
    {
        if (str[i] == -1)
			break;
        if (shakeToggle)
        {
            shakePos[0] = -1+(random_u16() % 2);
            shakePos[1] = -1+(random_u16() % 2);
        }
        else
        {
            shakePos[0] = 0;
            shakePos[1] = 0;
        }
        if (waveToggle)
        {
            wavePos = (sins((gGlobalTimer*3000)+(i*10000)))*2;
        }
        else
        {
            wavePos = 0;
        }
		if(CurEng->Ascii == 0){
			get_char_from_byte_sm64(str[i], &textX, &textY, &spaceX, &offsetY);
		}else{
			get_char_from_byte_ascii(str[i], &textX, &textY, &spaceX, &offsetY);
		}
        if (xlu != prevxlu)
        {
            prevxlu = xlu;
        }
		//space or end
		if(textX != 128){
			gSPScisTextureRectangle(gDisplayListHead++, ((x+shakePos[0]+textPos[0])) << 2, (SCREEN_HEIGHT-(y+yInc+shakePos[1]+offsetY+textPos[1]+wavePos)) << 2, ((x+textPos[0]+shakePos[0]+xInc)) << 2, (SCREEN_HEIGHT-(y+wavePos+offsetY+shakePos[1]+textPos[1])) << 2, G_TX_RENDERTILE, textX << 6, textY << 6, dsdx, dsdy);
		}
		textPos[0]+=((s32)(spaceX*xScale))+1;
    }
	printf("print pos %d, end pos %d len str %d, ascii %d\n",textPos[0],CurEng->TotalXOff,i, CurEng->Ascii);
}

u8 textLen_TE[] = {
    /*0*/ 7, /*1*/ 6, /*2*/ 7, /*3*/ 7, /*4*/ 7, /*5*/ 7, /*6*/ 7, /*7*/ 7, /*8*/ 7, /*9*/ 7, /*-*/ 8, /*+*/ 8, /*(*/ 5, /*)*/ 5, /*!*/ 4, /*?*/ 7,
    /*A*/ 7, /*B*/ 7, /*C*/ 7, /*D*/ 7, /*E*/ 6, /*F*/ 5, /*G*/ 8, /*H*/ 6, /*I*/ 6, /*J*/ 5, /*K*/ 7, /*L*/ 6, /*M*/ 7, /*N*/ 7, /*O*/ 7, /*P*/ 6,
    /*Q*/ 8, /*R*/ 6, /*S*/ 7, /*T*/ 7, /*U*/ 7, /*V*/ 7, /*W*/ 8, /*X*/ 7, /*Y*/ 7, /*Z*/ 7, /*"*/ 5, /*'*/ 2, /*:*/ 3, /*;*/ 3, /*.*/ 3, /*,*/ 3,
    /*a*/ 7, /*b*/ 7, /*c*/ 6, /*d*/ 7, /*e*/ 7, /*f*/ 7, /*g*/ 7, /*h*/ 7, /*i*/ 3, /*j*/ 5, /*k*/ 8, /*l*/ 4, /*m*/ 7, /*n*/ 7, /*o*/ 7, /*p*/ 7,
    /*q*/ 7, /*r*/ 6, /*s*/ 6, /*t*/ 6, /*u*/ 6, /*v*/ 7, /*w*/ 8, /*x*/ 6, /*y*/ 8, /*z*/ 7, /*~*/ 8, /*..*/ 8, /*^*/ 8, /*/*/ 8, /*%*/ 8, /*&*/ 8,
};
//same as below but uses SM64 strings as input, also uses even kerning
/* Unsupported characters
[x] / cross
[Cur*] / cur star count
[you][the] / you or the in one byte
[·] / a coin symbol
[A][B][C][Z][R] / bold letters to indicate buttons to press
< > / bold left right chevrons to indicate C button direction
★ ☆ / hollow and full stars
” “ / directional quotes (instead we have generic non angled quotes)
--extra characters in ascii that sm64 doesn't have
semi colon
*/
void get_char_from_byte_sm64(u8 letter, s32 *textX, s32 *textY, s32 *spaceX, s32 *offsetY)
{
    *offsetY = 0;
    //Line 1. numbers match
    if (letter >= 0 && letter <= 9)
    {
        *textX = letter * 4;
        *textY = 0;
        *spaceX = textLen_TE[letter];
		return;
    }
	else
    //Line 2. 'A' starts at 16 in texture, at 10 in sm64
    if (letter >= 10 && letter <= 0x19)
    {
        *textX = (letter-10) * 4;
        *textY = 6;
        *spaceX = textLen_TE[letter + 6];
		return;
    }
	else
    //Line 3, 'Q' starts at 32 in texture, at 26 in sm64
    if (letter >= 26 && letter <= 35)
    {
        *textX = ((letter - 26) * 4);
        *textY = 12;
        *spaceX = textLen_TE[letter + 6];
		return;
    }
	else
    //Line 4, 'a' starts at 48 in texture, at 36 in sm64
    if (letter >= 36 && letter <= 51)
    {
        *textX = ((letter - 36) * 4);
        *textY = 18;
        *spaceX = textLen_TE[letter + 12];
		return;
    }
    else
    //Line 5, 'q' starts at 64 in texture, at 52 in sm64
    if (letter >= 52 && letter <= 61)
    {
        *textX = ((letter - 52) * 4);
        *textY = 24;
        *spaceX = textLen_TE[letter + 12];
		return;
    }
    else if (letter==0x9e)
    {//Space, the final frontier.
        *textX = 128;
        *textY = 0;
        *spaceX = 3;
    }
	//just replaced the ascii case with sm64 byte
    switch (letter)
    {
        case 0xFF: *textX = 128; *textY = 0; *spaceX = 0; break; //END, shouldn't be encountered anyway
        case 0x9F: *textX = 40; *textY = 0; *spaceX = textLen_TE[10]; break; //Hyphen
        case 0xE4: *textX = 44; *textY = 0; *spaceX = textLen_TE[11]; break; //Plus
        case 0xE1: *textX = 48; *textY = 0; *spaceX = textLen_TE[12]; break; //Open Bracket
        case 0xE3: *textX = 52; *textY = 0; *spaceX = textLen_TE[13]; break; //Close Bracket
        case 0xF2: *textX = 56; *textY = 0; *spaceX = textLen_TE[14]; break; //Exclamation mark
        case 0xF4: *textX = 60; *textY = 0; *spaceX = textLen_TE[15]; break; //Question mark

        case 0xF5: *textX = 40; *textY = 12; *spaceX = textLen_TE[42]; break; //Speech mark
        case 0xF6: *textX = 40; *textY = 12; *spaceX = textLen_TE[42]; break; //Speech mark
        case 0x3E: *textX = 44; *textY = 12; *spaceX = textLen_TE[43]; break; //Apostrophe
        case 0xE6: *textX = 48; *textY = 12; *spaceX = textLen_TE[44]; break; //Colon
        case 0x3F: *textX = 56; *textY = 12; *spaceX = textLen_TE[46]; break; //Period
        case 0x6F: *textX = 60; *textY = 12; *spaceX = textLen_TE[47]; break; //Comma

        case 0xF7: *textX = 40; *textY = 24; *spaceX = textLen_TE[74]; break; //Tilde
        case 0x50: *textX = 48; *textY = 24; *spaceX = textLen_TE[76]; break; //Caret
        case 0x53: *textX = 40; *textY = 24; *spaceX = textLen_TE[74]; break; //side ARROW
        case 0x51: *textX = 44; *textY = 24; *spaceX = textLen_TE[75]; break; //Down arrow
        case 0xD0: *textX = 52; *textY = 24; *spaceX = textLen_TE[77]; break; //Slash
        case 0xF3: *textX = 56; *textY = 24; *spaceX = textLen_TE[78]; break; //percent
        case 0xE5: *textX = 60; *textY = 24; *spaceX = textLen_TE[79]; break; //Ampersand
		
        //unsupported but wanted to make them draw something close
		case 0x54: *textX = 0; *textY = 6; *spaceX = textLen_TE[79]; break; //A bold
        case 0x55: *textX = 4; *textY = 6; *spaceX = textLen_TE[79]; break; //B bold
        case 0x56: *textX = 8; *textY = 6; *spaceX = textLen_TE[79]; break; //C bold
        case 0x57: *textX = 12; *textY = 6; *spaceX = textLen_TE[79]; break; //Z bold
        case 0x58: *textX = 16; *textY = 6; *spaceX = textLen_TE[79]; break; //R bold

        //This is for the letters that sit differently on the line. It just moves them down a bit.
        case 0x2A: *offsetY = 1; break;
        case 0x34: *offsetY = 1; break;
        case 0x33: *offsetY = 3; break;
        case 0x3C: *offsetY = 1; break;
    }
}
//ascii equiv
void get_char_from_byte_ascii(u8 letter, s32 *textX, s32 *textY, s32 *spaceX, s32 *offsetY)
{
	*offsetY = 0;
    //Line 1
    if (letter >= '0' && letter <= '9')
    {
        *textX = (letter - '0') * 4;
        *textY = 0;
        *spaceX = textLen[letter - '0'];
		return;
    }
    else
    //Line 2
    if (letter >= 'A' && letter <= 'P')
    {
        *textX = ((letter - 'A') * 4);
        *textY = 6;
        *spaceX = textLen[letter - 'A'+16];
		return;
    }
    else
    //Line 3
    if (letter >= 'Q' && letter <= 'Z')
    {
        *textX = ((letter - 'Q') * 4);
        *textY = 12;
        *spaceX = textLen[letter - 'Q'+32];
		return;
    }
    else
    //Line 4
    if (letter >= 'a' && letter <= 'p')
    {
        *textX = ((letter - 'a') * 4);
        *textY = 18;
        *spaceX = textLen[letter - 'a'+48];
		return;
    }
    else
    //Line 5
    if (letter >= 'q' && letter <= 'z')
    {
        *textX = ((letter - 'q') * 4);
        *textY = 24;
        *spaceX = textLen[letter - 'q'+64];
		return;
    }
    else
    {//Space, the final frontier.
        *textX = 128;
        *textY = 0;
        *spaceX = 3;
    }

    switch (letter)
    {
        case 0xFF: *textX = 128; *textY = 0; *spaceX = 0; break; //END, shouldn't be encountered anyway
		
		case '-': *textX = 40; *textY = 0; *spaceX = textLen[10]; break; //Hyphen
        case '+': *textX = 44; *textY = 0; *spaceX = textLen[11]; break; //Plus
        case '(': *textX = 48; *textY = 0; *spaceX = textLen[12]; break; //Open Bracket
        case ')': *textX = 52; *textY = 0; *spaceX = textLen[13]; break; //Close Bracket
        case '!': *textX = 56; *textY = 0; *spaceX = textLen[14]; break; //Exclamation mark
        case '?': *textX = 60; *textY = 0; *spaceX = textLen[15]; break; //Question mark

        case '"': *textX = 40; *textY = 12; *spaceX = textLen[42]; break; //Speech mark
        case 0x27: *textX = 44; *textY = 12; *spaceX = textLen[43]; break; //Apostrophe.
        case ':': *textX = 48; *textY = 12; *spaceX = textLen[44]; break; //Colon
        case ';': *textX = 52; *textY = 12; *spaceX = textLen[45]; break; //Semicolon
        case '.': *textX = 56; *textY = 12; *spaceX = textLen[46]; break; //Full stop
        case ',': *textX = 60; *textY = 12; *spaceX = textLen[47]; break; //Comma

        case '>': *textX = 40; *textY = 24; *spaceX = textLen[74]; break; //Side arrow
        case '@': *textX = 44; *textY = 24; *spaceX = textLen[75]; break; //Down Arrow
        case '^': *textX = 48; *textY = 24; *spaceX = textLen[76]; break; //Caret
        case '/': *textX = 52; *textY = 24; *spaceX = textLen[77]; break; //Slash
        case '_': *textX = 56; *textY = 24; *spaceX = textLen[78]; break; //Percent
        case '&': *textX = 60; *textY = 24; *spaceX = textLen[79]; break; //Ampersand

        //This is for the letters that sit differently on the line. It just moves them down a bit.
        case 'g': *offsetY = 1; break;
        case 'q': *offsetY = 1; break;
        case 'p': *offsetY = 3; break;
        case 'y': *offsetY = 1; break;
    }
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

u32* TE_get_ptr(u8 *strArgs,u8 *str){
	u16 pos = TE_get_u16(strArgs);
	u16 ptrID = TE_get_u16(strArgs+2);
	str = (u32*)(str-sizeof(u32*)-pos); //to move to start of ptrptr
	//PC specific behavior, there can be anywhere from 0 to 0x18 bytes
	//of padding, I don't know how to detect it or place a pointer to my pointers
	//in a way that will actually compile
	#ifndef TARGET_N64
	//still in padding, check a max of 3 times
	if(*(u32*)str == NULL){
		str -= 8;
	}
	if(*(u32*)str == NULL){
		str -= 8;
	}
	if(*(u32*)str == NULL){
		str -= 8;
	}
	#endif
	u32 **Ptrptr = str;
	u32 **ptr = segmented_to_virtual(*Ptrptr);
	return ptr[ptrID];
}

//includes
#include "text_engine_cmds.inc.h"
#include "text_engine_helpers.inc.h"

#if TE_DEBUG
extern uintptr_t sSegmentTable[32];
#include "magic.h"
void TE_debug_print(struct TEState *CurEng){
	u8 buf[32];
	// if (gPlayer1Controller->buttonDown&L_TRIG){
		// sprintf(buf,"og %x",CurEng->OgStr);
		// print_text(32,64,buf);
		// sprintf(buf,"temp %x",CurEng->TempStr);
		// print_text(32,96,buf);
		// sprintf(buf,"stck %x",CurEng->StrStack[0]);
		// print_text(32,128,buf);
	// }
	
}
#endif

#endif