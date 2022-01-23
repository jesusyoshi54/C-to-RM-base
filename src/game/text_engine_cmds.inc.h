s8 TE_jump_cmds(struct TEState *CurEng,u8 cmd,u8 *str){
	//first check plaintext complaint cmds
	s8 Loop;
	switch(cmd){
		case 0x40:
			Loop = TE_set_speed(CurEng,str);
			break;
		case 0x41:
			Loop = TE_set_sfx(CurEng,str);
			break;
		case 0x42:
			Loop = TE_set_env_color(CurEng,str);
			break;
		case 0x43:
			Loop = TE_display_usr_str(CurEng,str);
			break;
		case 0x44:
			Loop = TE_set_scissor(CurEng,str);
			break;
		case 0x45:
			Loop = TE_return_usr_str(CurEng,str);
			break;
		case 0x46:
			Loop = TE_enable_rainbow_txt(CurEng,str);
			break;
		case 0x47:
			Loop = TE_change_origin(CurEng,str);
			break;
		case 0x48:
			Loop = TE_jump_str(CurEng,str);
			break;
		case 0x49:
			Loop = TE_translate_offset(CurEng,str);
			break;
		case 0x4A:
			Loop = TE_translate_absolute(CurEng,str);
			break;
		case 0x4B:
			Loop = TE_pop_transform(CurEng,str);
			break;
		case 0x4C:
			Loop = TE_en_A_spd_incr(CurEng,str);
			break;
		case 0x4D:
			Loop = TE_dis_A_spd_incr(CurEng,str);
			break;
		case 0x4E:
			Loop = TE_always_allow_keyboard(CurEng,str);
			break;
		case 0x4F:
			Loop = TE_make_keyboard(CurEng,str);
			break;
		//characters are from 0x50 to 0x70
		case 0x70:
			Loop = TE_auto_goto_next_box(CurEng,str);
			break;
		case 0x71:
			Loop = TE_Abtn_goto_next_box(CurEng,str);
			break;
		case 0x72:
			Loop = TE_blank_time(CurEng,str);
			break;
		case 0x73:
			Loop = TE_btn_check_start(CurEng,str);
			break;
		case 0x74:
			Loop = TE_pause_time(CurEng,str);
			break;
		case 0x75:
			Loop = TE_advBlen(CurEng,1);
			break;
		case 0x76:
			Loop = TE_enable_blip(CurEng,str);
			break;
		case 0x77:
			Loop = TE_disable_blip(CurEng,str);
			break;
		case 0x78:
			Loop = TE_persist_music(CurEng,str);
			break;
		case 0x79:
			Loop = TE_change_music(CurEng,str);
			break;
		case 0x7A:
			Loop = TE_clean_buffer(CurEng,str);
			break;
		case 0x7B:
			Loop = TE_Abtn_end_string(CurEng,str);
			break;
		case 0x7C:
			Loop = TE_time_end_string(CurEng,str);
			break;
		case 0x7D:
			Loop = TE_mosaic_bg_box(CurEng,str);
			break;
		case 0x7E:
			Loop = TE_print_DL(CurEng,str);
			break;
		case 0x7F:
			Loop = TE_shaded_bg_box(CurEng,str);
			break;
		case 0x80:
			Loop = TE_textured_bg_box(CurEng,str);
			break;
		case 0x81:
			Loop = TE_push_transform(CurEng,str);
			break;
		case 0x82:
			Loop = TE_set_cutscene(CurEng,str);
			break;
		case 0x83:
			Loop = TE_advBlen(CurEng,1);
			break;
		case 0x84:
			Loop = TE_scale_text(CurEng,str);
			break;
		case 0x85:
			Loop = TE_enable_dialog_options(CurEng,str);
			break;
		case 0x86:
			Loop = TE_dialog_response(CurEng,str);
			break;
		case 0x87:
			Loop = TE_advBlen(CurEng,1);
			break;
		case 0x88:
			Loop = TE_screen_shake(CurEng,str);
			break;
		//camera cmds
		case 0x8F:
			Loop = TE_trigger_warp(CurEng,str);
			break;
		//object cmds
		case 0x93:
			Loop = TE_start_bracket(CurEng,str);
			break;
		case 0x94:
			Loop = TE_advBlen(CurEng,2);
			break;
		case 0x95:
			Loop = TE_advBlen(CurEng,2);
			break;
		case 0x96:
			Loop = TE_advBlen(CurEng,2);
			break;
		case 0x97:
			Loop = TE_set_return(CurEng,str);
			break;
		case 0x98:
			Loop = TE_goto_return(CurEng,str);
			break;
		case 0x99:
			Loop = TE_enable_plaintext(CurEng,str);
			break;
		case 0x9A:
			Loop = TE_enable_wobble(CurEng,str);
			break;
		case 0x9B:
			Loop = TE_enable_end_transition(CurEng,str);
			break;
		case 0x9C:
			Loop = TE_enable_start_transition(CurEng,str);
			break;
		//transitions
		case 0x9D:
			Loop = TE_advBlen(CurEng,1);
			break;
		case 0xA0:
			Loop = TE_call_once(CurEng,str);
			break;
		case 0xA1:
			Loop = TE_call_loop(CurEng,str);
			break;
		case 0xA2:
			Loop = TE_function_response(CurEng,str);
			break;
		case 0xA6:
			Loop = TE_set_mario_action(CurEng,str);
			break;
		case 0xAA:
			Loop = TE_box_transition(CurEng,str);
			break;
		case 0xAC:
			Loop = TE_jump_link_str(CurEng,str);
			break;
		case 0xAD:
			Loop = TE_pop_str(CurEng,str);
			break;
		case 0xAE:
			Loop = TE_enable_shake(CurEng,str);
			break;
		case 0xAF:
			Loop = TE_print_glyph(CurEng,str);
			break;
		case 0xB0:
			Loop = TE_word_wrap(CurEng,str);
			break;
		case 0xFE:
			Loop = TE_line_break(CurEng,str);
			break;
		case 0xFF:
			Loop = TE_terminator(CurEng,str);
			break;
		//more camera/object cmds
	}
	return Loop;
}

s8 TE_advBlen(struct TEState *CurEng,u16 len){
	CurEng->TempStr+=len;
	return 1;
}

s8 TE_print_adv(struct TEState *CurEng,u16 len){
	CurEng->TempStrEnd-=CurEng->CurPos;
	TE_advBlen(CurEng,len+CurEng->CurPos);
	CurEng->CurPos=0;
	return 1;
}

//40 cmd works
s8 TE_set_speed(struct TEState *CurEng,u8 *str){
	CurEng->VIpChar = TE_get_s16(str);
	return TE_advBlen(CurEng,3);
}
//41 cmd works
s8 TE_set_sfx(struct TEState *CurEng,u8 *str){
	if(CurEng->TempStrEnd==CurEng->CurPos){
		CurEng->SfxArg = TE_get_u16(str);
	}
	return TE_advBlen(CurEng,3);
}
//42 cmd works
s8 TE_set_env_color(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->PrevEnvColorWord = CurEng->EnvColorWord;
	CurEng->EnvColorWord = TE_get_u32(str);
	return TE_print_adv(CurEng,5);
}
//43 cmd works
s8 TE_display_usr_str(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->ReturnUsrStr = CurEng->TempStr+2+CurEng->CurPos;
	CurEng->TempStr = &UserInputs[CurEng->state][str[1]];
	CurEng->CurPos=0;
	return 1;
}
//44 cmd works
s8 TE_set_scissor(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	gDPSetScissor(gDisplayListHead++,G_SC_NON_INTERLACE,TE_get_u16(str)+CurEng->BoxTrXi,SCREEN_HEIGHT-TE_get_u16(str+6)-CurEng->BoxTrYf,TE_get_u16(str+2)+CurEng->BoxTrXf,SCREEN_HEIGHT-TE_get_u16(str+4)-CurEng->BoxTrYi);
	CurEng->ScissorSet = 1;
	TE_clear_box_tr(CurEng);
	return TE_print_adv(CurEng,9);
}
//45 cmd works
s8 TE_return_usr_str(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->TempStrEnd-=CurEng->CurPos;
	CurEng->CurPos=0;
	CurEng->TempStr = CurEng->ReturnUsrStr;
	return 1;
}
//46 cmd works
s8 TE_enable_rainbow_txt(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	u32 i;
	f32 f;
	u32 timer = gNumVblanks;
	for(i=0;i<3;i++){
		f = .04f*timer;
		CurEng->RainbowColorByte[i] = (u8) 127.5f+127.5f*sinf(f);
		timer+=0x78;
	}
	CurEng->RainbowColorByte[3] = str[1];
	CurEng->PrevEnvColorWord = CurEng->EnvColorWord;
	CurEng->EnvColorWord = CurEng->RainbowColorWord;
	return TE_print_adv(CurEng,2);
}
//47 cmd works
s8 TE_change_origin(struct TEState *CurEng,u8 *str){
	CurEng->StartX = TE_get_s16(str);
	CurEng->StartY = TE_get_s16(str+2);
	CurEng->TempXOrigin = CurEng->StartX;
	CurEng->TempYOrigin = CurEng->StartY;
	return TE_advBlen(CurEng,5);
}
//48 cmd works
s8 TE_jump_str(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->TempStr = segmented_to_virtual(TE_get_ptr(str,str));
	CurEng->CurPos = 0;
	return 1;
}
//49 cmd works
s8 TE_translate_offset(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->TempX = CurEng->TempX+TE_get_s16(str);
	CurEng->TempY = CurEng->TempY+TE_get_s16(str+2);
	CurEng->TempXOrigin = CurEng->TempX;
	CurEng->TempYOrigin = CurEng->TempY;
	return TE_print_adv(CurEng,5);
}
//4A cmd works
s8 TE_translate_absolute(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->TempX = TE_get_s16(str);
	CurEng->TempY = TE_get_s16(str+2);
	CurEng->TempXOrigin = CurEng->TempX;
	CurEng->TempYOrigin = CurEng->TempY;
	return TE_print_adv(CurEng,5);
}
//4B cmd works
s8 TE_pop_transform(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	u8 z;
	if (CurEng->TransformStackPos){
		for(z=0;z<CurEng->TransformStack[CurEng->TransformStackPos-1];z++){
			gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
		}
		CurEng->TransformStack[CurEng->TransformStackPos-1] = 0;
		CurEng->TransformStackPos--;
	}
	return TE_print_adv(CurEng,1);
}
//4C cmd works
s8 TE_en_A_spd_incr(struct TEState *CurEng,u8 *str){
	CurEng->NewSpeed = TE_get_s16(str);
	return TE_advBlen(CurEng,3);
}
//4D cmd works
s8 TE_dis_A_spd_incr(struct TEState *CurEng,u8 *str){
	CurEng->NewSpeed = 0x1234;
	return TE_advBlen(CurEng,1);
}
//4E cmd works
s8 TE_always_allow_keyboard(struct TEState *CurEng,u8 *str){
	CurEng->KeyboardReset |= 1;
	return TE_advBlen(CurEng,1);
}
//4F cmd works
s8 TE_make_keyboard(struct TEState *CurEng,u8 *str){
	//end keyboard
	if(CurEng->KeyboardState == 1){
		return TE_draw_keyboard(CurEng,str);
	}
	if(CurEng->KeyboardState == 5){
		CurEng->TempStr = CurEng->PreKeyboardStr+2;
		UserInputs[CurEng->state][CurEng->CurUsrStr][CurEng->UserInput] = 0x45;
		if(CurEng->KeyboardReset==0){
			CurEng->PreKeyboardStr[0] = 0x83;
			CurEng->PreKeyboardStr[1] = 0x83;
			CurEng->KeyboardReset &= -2;
		}
		CurEng->KeyboardState = 0;
		TE_add_to_cmd_buffer(CurEng,CurEng->PreKeyboardStr,2);
		return -1;
	}
	if(CurEng->KeyboardState == 2){
		return TE_add_usr_str(CurEng,str);
	}
	if(CurEng->KeyboardState == 3){
		CurEng->KeyboardState = 1;
		CurEng->KeyboardChar = 0;
		return -1;
	}
	CurEng->KeyboardState = 1;
	CurEng->InputStr = 0;
	CurEng->UserInput = 0;
	CurEng->IntendedLetter = 0;
	CurEng->ShiftPressed = 0;
	CurEng->PlainText = 0;
	CurEng->PreKeyboardStr = str;
	u8 ind = str[1];
	u8 i;
	CurEng->CurUsrStr = ind;
	//fill in user input
	for (i=0; i<15;i++){
		UserInputs[CurEng->state][ind][i] = 0x9f;
	}
	UserInputs[CurEng->state][ind][15] = 0x45;
	return -1;
}
s8 TE_add_usr_str(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	gDPSetRenderMode(gDisplayListHead++,G_RM_XLU_SURF, G_RM_XLU_SURF2);
	CurEng->TempStrEnd-=CurEng->CurPos;
	CurEng->CurPos=0;
	CurEng->KeyboardState = 3;
	str[1] = CurEng->CurUsrStr;
	return TE_display_usr_str(CurEng,str);
}
//cringer
s8 TE_draw_keyboard(struct TEState *CurEng,u8 *str){
	//draw user input
	//draw keyboard
	CurEng->KeyboardState = 2;
	if(CurEng->ShiftPressed){
		CurEng->TempStr = &TE_KEYBOARD_upper;
	}else{
		CurEng->TempStr = &TE_KEYBOARD_lower;
	}
	if(gNumVblanks > CurEng->KeyboardTimer){
		if(gPlayer1Controller->buttonPressed&START_BUTTON){
			CurEng->KeyboardTimer = gNumVblanks+2;
			CurEng->IntendedLetter = 42;
		}
		else if(gPlayer1Controller->buttonPressed&A_BUTTON){
			CurEng->KeyboardTimer = gNumVblanks+2;
			//handle shift, end and backspace
			switch(CurEng->SelLetter){
				//shift
				case 0x50:
					CurEng->ShiftPressed^=1;
					break;
				//backspace
				case 0x9F:
					if(CurEng->UserInput>0){
						UserInputs[CurEng->state][CurEng->CurUsrStr][CurEng->UserInput-1] = 0x9F;
						CurEng->UserInput-=1;
					}
					break;
				default:
					//end string
					if(CurEng->IntendedLetter == 42){
						CurEng->KeyboardState = 5;
						return -1;
					}
					//space
					u8 letter = CurEng->SelLetter;
					if(CurEng->IntendedLetter == 41){
						letter = 0x9e;
					}
					if(CurEng->UserInput<15){
						UserInputs[CurEng->state][CurEng->CurUsrStr][CurEng->UserInput] = letter;
						CurEng->UserInput+=1;
					}
					break;
			}
		}else if(gPlayer1Controller->buttonPressed&B_BUTTON && CurEng->UserInput>0){
			CurEng->KeyboardTimer = gNumVblanks+2;
			UserInputs[CurEng->state][CurEng->CurUsrStr][CurEng->UserInput-1] = 0x9F;
			CurEng->UserInput-=1;
		}
	}if(CurEng->KeyboardTimerScroll<gNumVblanks){
		CurEng->KeyboardTimerScroll = gNumVblanks+2;
		//for overflow
		s8 vert = 0;
		handle_menu_scrolling(MENU_SCROLL_VERTICAL,&vert,-1,1);
		handle_menu_scrolling(MENU_SCROLL_HORIZONTAL,&CurEng->IntendedLetter,-1,44);
		//another very large inefficiency
		if (vert==1){
			CurEng->IntendedLetter+=10;
			if(CurEng->IntendedLetter>43 && CurEng->IntendedLetter<50){
				CurEng->IntendedLetter = 41;
			}
			else if(CurEng->IntendedLetter>43){
				CurEng->IntendedLetter = 0;
			}
		}else if(vert==-1){
			CurEng->IntendedLetter-=10;
			if(CurEng->IntendedLetter<0){
				CurEng->IntendedLetter = 42;
			}
		}if(CurEng->IntendedLetter==44){
			CurEng->IntendedLetter = 0;
		}
		else if(CurEng->IntendedLetter==-1){
			CurEng->IntendedLetter = 43;
		}
	}
	return 1;
}
//draw the BG as black for selected letter
s8 TE_keyboard_sel(struct TEState *CurEng,u8 *str,u8 state){
	TE_print(CurEng);
	if(state){
		gDPSetRenderMode(gDisplayListHead++,G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
		CurEng->SelLetter = str[0];
	}else{
		gDPSetRenderMode(gDisplayListHead++,G_RM_XLU_SURF, G_RM_XLU_SURF2);
	}
	return TE_print_adv(CurEng,0);
}
//generic text box handler
s8 TE_next_box(struct TEState *CurEng,u8 *str){
	CurEng->TrEnd.TransVI = 0;
	CurEng->KeyboardReset &= 0x7F;
	if(CurEng->TrStart.TransLength != 0){
		CurEng->TrStart.TransVI = gNumVblanks;
	}
	CurEng->OgStr = CurEng->CurPos+CurEng->TempStr+1;
	//pop all transforms
	u8 z;
	u8 j;
	for(j=0;z<CurEng->TransformStackPos;j++){
		for(z=0;z<CurEng->TransformStack[j];z++){
			gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
			CurEng->TransformStack[j] = 0;
		}
	}
	CurEng->TransformStackPos = 0;
	CurEng->StackLocked = CurEng->StackDepth;
	CurEng->CurPos = 0;
	CurEng->StrEnd = 0;
	CurEng->TransX = 0;
	CurEng->TransY = 0;
	TE_clear_box_tr(CurEng);
	StrBuffer[CurEng->state][0] = 0xFF;
	return -1;
}
//70 cmd works
s8 TE_auto_goto_next_box(struct TEState *CurEng,u8 *str){
	CurEng->LastVI = gNumVblanks;
	if(CurEng->TrEnd.TransLength == 0){
		return TE_next_box(CurEng, str);
	}else{
		if(CurEng->TrEnd.TransVI == 0){
			CurEng->TrEnd.TransVI = gNumVblanks;
			return 0;
		}
		else{
			if(gNumVblanks >= (CurEng->TrEnd.TransLength+CurEng->TrEnd.TransVI)){
				return TE_next_box(CurEng, str);
			}
			return 0;
		}
	}
}
//71 cmd works
s8 TE_Abtn_goto_next_box(struct TEState *CurEng,u8 *str){
	if(CurEng->TrStart.TransVI != 0){
		return 0;
	}
	u8 arrow = 0xFF;
	u8 check = CurEng->KeyboardReset&0x80;
	CurEng->KeyboardReset |= 0x80;
	CurEng->LastVI = gNumVblanks;
	if(check == 0){
		return 0;
	}
	if(CurEng->TrEnd.TransVI != 0){
		return TE_auto_goto_next_box(CurEng,str);
	}
	if(gNumVblanks & 0x20){
		arrow = 0x51;
	}
	StrBuffer[CurEng->state][CurEng->CurPos] = arrow;
	StrBuffer[CurEng->state][CurEng->CurPos+1] = 0xFF;
	if(gPlayer1Controller->buttonPressed&A_BUTTON){
		return TE_auto_goto_next_box(CurEng,str);
	}else{
		return 0;
	}
}
//72 cmd works
s8 TE_blank_time(struct TEState *CurEng,u8 *str){
	if(CurEng->BlankTimer != 0){
		CurEng->LastVI = gNumVblanks;
		if((CurEng->BlankTimer+TE_get_u16(CurEng->StrRemoval)) < gNumVblanks){
			CurEng->TempStr = CurEng->StrRemoval+3;
			CurEng->BlankTimer = 0;
			TE_add_to_cmd_buffer(CurEng,CurEng->StrRemoval,3);
			return 1;
		}else{
			return -1;
		}
	}else{
		CurEng->BlankTimer = gNumVblanks;
		CurEng->StrRemoval = str;
		return -1;
	}
}
//73 cmd works
s8 TE_btn_check_start(struct TEState *CurEng,u8 *str){
	if(gPlayer1Controller->buttonDown&TE_get_u16(str)){
		return TE_advBlen(CurEng,3);
	}else{
		str += 3;
		CurEng->TempStr += 3;
		while(str[0] != 0x75){
			str += 1;
			CurEng->TempStr += 1;
		}
		return 1;
	}
}
//74 cmd works
s8 TE_pause_time(struct TEState *CurEng,u8 *str){
	if(getTEspd(CurEng) == 0){
		PauseDone:
			CurEng->PauseTimer = 0;
			TE_add_to_cmd_buffer(CurEng,str,3);
			return TE_advBlen(CurEng,3);
	}else{
		CurEng->LastVI = gNumVblanks;
		StrBuffer[CurEng->state][CurEng->CurPos+1] = 0xFF;
		if(CurEng->PauseTimer == 0){
			CurEng->PauseTimer = gNumVblanks;
			return 0;
		}
		if((CurEng->PauseTimer+TE_get_u16(str)) > gNumVblanks){
			return 0;
		}
		goto PauseDone;
	}
}
//76 cmd works
s8 TE_enable_blip(struct TEState *CurEng,u8 *str){
	CurEng->CheckBlip = 1;
	return TE_advBlen(CurEng,1);
}
//77 cmd works
s8 TE_disable_blip(struct TEState *CurEng,u8 *str){
	CurEng->CheckBlip = 0;
	return TE_advBlen(CurEng,1);
}
//78 cmd works
s8 TE_persist_music(struct TEState *CurEng,u8 *str){
	if(CurEng->NewSeqID != -1){
		CurEng->OgSeqID = CurEng->NewSeqID;
	}
	return TE_advBlen(CurEng,1);
}
//79 cmd works
s8 TE_change_music(struct TEState *CurEng,u8 *str){
	u16 CurSeq = gCurrentArea->musicParam;
	if(CurEng->OgSeqID == -1){
		CurEng->OgSeqID = CurSeq;
	}
	if(CurEng->NewSeqID == str[1]){
		return TE_advBlen(CurEng,2);
	}
	CurEng->NewSeqID = str[1];
	gCurrentArea->musicParam = str[1];
	play_music(SEQ_PLAYER_LEVEL,str[1],15);
	return TE_advBlen(CurEng,2);
}
//7a cmd works
s8 TE_clean_buffer(struct TEState *CurEng,u8 *str){
	TE_clear_cmd_buffer(CurEng);
	return TE_advBlen(CurEng,1);
}
//generic string ender
s8 TE_end_str(struct TEState *CurEng){
	if(CurEng->TrEnd.TransLength != 0){
		if(CurEng->TrEnd.TransVI == 0){
			CurEng->TrEnd.TransVI = gNumVblanks;
			return 0;
		}
		if(gNumVblanks >= (CurEng->TrEnd.TransLength+CurEng->TrEnd.TransVI)){
			return TE_reset_str(CurEng);
		}
		return 0;
	}else{
		return TE_reset_str(CurEng);
	}
}
//reset everything back to 0
s8 TE_reset_str(struct TEState *CurEng){
	if(CurEng->NewSeqID != CurEng->OgSeqID){
		stop_background_music(CurEng->NewSeqID);
		gCurrentArea->musicParam = CurEng->OgSeqID;
	}
	TE_flush_buffers(CurEng);
	return -2;
}
//7b cmd works
s8 TE_Abtn_end_string(struct TEState *CurEng,u8 *str){
	if(CurEng->TrStart.TransVI != 0){
		return 0;
	}
	u8 arrow = 0xFF;
	u8 check = CurEng->KeyboardReset&0x80;
	CurEng->KeyboardReset |= 0x80;
	CurEng->LastVI = gNumVblanks;
	if(check == 0){
		return 0;
	}
	if(gNumVblanks & 0x20){
		arrow = 0x51;
	}
	StrBuffer[CurEng->state][CurEng->CurPos] = arrow;
	StrBuffer[CurEng->state][CurEng->CurPos+1] = 0xFF;
	if(CurEng->TrEnd.TransVI != 0){
		return TE_end_str(CurEng);
	}
	if(gPlayer1Controller->buttonPressed&A_BUTTON){
		return TE_end_str(CurEng);
	}
	return 0;
}
//7c cmd works
s8 TE_time_end_string(struct TEState *CurEng,u8 *str){
	if(CurEng->EndTimer == 0){
		CurEng->EndTimer = gNumVblanks;
		return 0;
	}
	if(gNumVblanks >= (CurEng->EndTimer+TE_get_u16(str))){
		return TE_end_str(CurEng);
	}
	StrBuffer[CurEng->state][CurEng->CurPos] = 0xFF;
	return 0;
}
//generic BG box handlers
void TE_bg_box_finish(struct TEState *CurEng){
	TE_reset_Xpos(CurEng);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
}
void TE_clear_box_tr(struct TEState *CurEng){
	CurEng->BoxTrXi = 0;
	CurEng->BoxTrXf = 0;
	CurEng->BoxTrYi = 0;
	CurEng->BoxTrYf = 0;
}
void TE_bg_box_setup(struct TEState *CurEng){
	//print shadow with plaintext
	if(CurEng->PlainText){
		s32 Env = CurEng->EnvColorWord;
		CurEng->EnvColorWord = 0x10101000 | CurEng->EnvColorByte[3];
		CurEng->TempX += 1;
		CurEng->TempY -= 1;
		if(!(StrBuffer[CurEng->state][0] == 0xFF))
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
void TE_bg_coords(struct TEState *CurEng,u8 *str){
	f32 x1 = (f32) (TE_get_u16(str)+CurEng->BoxTrXi);
	f32 y1 = (f32) (TE_get_u16(str+4)+CurEng->BoxTrYi);
	f32 x2 = (f32) (TE_get_u16(str+2)+CurEng->BoxTrXf);
	f32 y2 = (f32) (TE_get_u16(str+6)+CurEng->BoxTrYf);
	create_dl_scale_matrix(MENU_MTX_PUSH, (x2-x1)/130.0f,(y2-y1)/80.0f, 1.0f);
	create_dl_translation_matrix(MENU_MTX_NOPUSH, 130.0f*(x1/(x2-x1)),80.0f*y2/(y2-y1), 1.0f);
}
//7d cmd works
s8 TE_mosaic_bg_box(struct TEState *CurEng,u8 *str){
	u8 i;
	u8 j;
	TE_bg_box_setup(CurEng);
	gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
	f32 x1 = (f32) (TE_get_u16(str)+CurEng->BoxTrXi);
	f32 y1 = (f32) (TE_get_u16(str+4)+CurEng->BoxTrYi);
	f32 x2 = (f32) (TE_get_u16(str+2)+CurEng->BoxTrXf);
	f32 y2 = (f32) (TE_get_u16(str+6)+CurEng->BoxTrYf);
	u16 lenX = str[13];
	u16 lenY = str[14];
	u32 *ptr = segmented_to_virtual(TE_get_ptr(str+8,str));
	f32 W = (x2-x1)/lenX;
	f32 H = (y2-y1)/lenY;
	f32 xOff;
	f32 yOff;
	for(j=0;j<lenY;j++){
		for(i=0;i<lenX;i++){
			// xOff = i*130.0f;
			// yOff = j*80.0f;
			// create_dl_scale_matrix(MENU_MTX_PUSH, W/130.0f,H/80.0f, 1.0f);
			// create_dl_translation_matrix(MENU_MTX_NOPUSH, xOff+(130.0f*x1/W),(80.0f*y2/H)-yOff, 1.0f);
			// gDPLoadTextureBlock(gDisplayListHead++,ptr+(i*0x200)+(str[13]*j*0x200),G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			// gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box_TE);
			// gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
			u16 ULx = (u16) x1+W*i;
			u16 ULy = (u16) y1+H*(lenY-j-1)+H;
			u16 LRx = (u16) x1+W*i+W;
			u16 LRy = (u16) y1+H*(lenY-j-1);
			gDPLoadSync(gDisplayListHead++);
			gDPLoadTextureTile(gDisplayListHead++, ptr, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32*lenX, 32*lenY, 32*i, 32*j, 32*i+31, 32*j+31, 0,  G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 0, 0);
			gSPScisTextureRectangle(gDisplayListHead++, (ULx) << 2, (SCREEN_HEIGHT-ULy) << 2, (LRx) << 2,(SCREEN_HEIGHT-LRy) << 2, G_TX_RENDERTILE, 0, 0, (u32)((32.0f/W)*1024.0f), (u32)((32.0f/H)*1024.0f));
		}
	}
	TE_clear_box_tr(CurEng);
	return TE_print_adv(CurEng,15);
}
void PushTransform(u8 Type,f32 x,f32 y, f32 z){
	switch(Type){
		case 0:
			create_dl_translation_matrix(MENU_MTX_PUSH, x, y, z);
			break;
		case 1:
			create_dl_scale_matrix(MENU_MTX_PUSH, x,y,z);
			break;
		case 2:; //can't declare after label
			Mat4 mtxf;
			Mtx *mtx = alloc_display_list(sizeof(*mtx));
			Vec3f translation;
			Vec3s rotation;
			//Can be optmized?
			translation[0]=0;
			translation[1]=0;
			translation[2]=0;
			rotation[0]=(u16)x*(32768.0f/180.0f);
			rotation[1]=(u16)y*(32768.0f/180.0f);
			rotation[2]=(u16)z*(32768.0f/180.0f);
			mtxf_rotate_zxy_and_translate(mtxf,translation,rotation);
			mtxf_to_mtx(mtx, mtxf);
			gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
			break;
	}
}
void Process_Transformation(struct TEState *CurEng,u8 *str,u8 *CmdLoc,u8 *CmdSize, u8 *PushCnt,s16 Timer,u8 Type){
	f32 x,y,z;
	u32 Counter = TimerBuffer[CurEng->state][Timer+1];
	u32 *CountInc = &TimerBuffer[CurEng->state][Timer+1];
	switch(TE_get_ptr(str+*CmdLoc-1,str)){
		case DL_TRAN_CONST:
			x = (f32) (TE_get_ptr(str+*CmdLoc+3,str));
			y = (f32) (TE_get_ptr(str+*CmdLoc+7,str));
			z = (f32) (TE_get_ptr(str+*CmdLoc+11,str));
			PushTransform(Type,x/256.0f,y/256.0f,z/256.0f);
			*CmdSize += 16;
			*CmdLoc += 16;
			*PushCnt += 1;
			break;
		case DL_TRAN_VEL:
			x = (f32) (TE_get_ptr(str+*CmdLoc+3,str))*Counter;
			y = (f32) (TE_get_ptr(str+*CmdLoc+7,str))*Counter;
			z = (f32) (TE_get_ptr(str+*CmdLoc+11,str))*Counter;
			PushTransform(Type,x/256.0f,y/256.0f,z/256.0f);
			*CmdSize += 16;
			*CmdLoc += 16;
			*PushCnt += 1;
			break;
		case DL_TRAN_CYCLE:
			x = (f32) (TE_get_ptr(str+*CmdLoc+3,str));
			y = (f32) (TE_get_ptr(str+*CmdLoc+7,str));
			z = (f32) (TE_get_ptr(str+*CmdLoc+11,str));
			u32 CycleTime = TE_get_ptr(str+*CmdLoc+15,str);
			u32 CycleType = TE_get_ptr(str+*CmdLoc+19,str);
			switch(CycleType){
				case CYCLE_LOOP:
					if (Counter>CycleTime){
						*CountInc = 0;
					}
					x=Counter*(x/(f32)CycleTime);
					y=Counter*(y/(f32)CycleTime);
					z=Counter*(z/(f32)CycleTime);
					break;
				case CYCLE_REVERSE:
					if (Counter>(CycleTime*2)){
						*CountInc = 0;
					}
					else if (Counter>(CycleTime)){
						x=2*x-Counter*(x/(f32)CycleTime);
						y=2*y-Counter*(y/(f32)CycleTime);
						z=2*z-Counter*(z/(f32)CycleTime);
					}
					else{
						x=Counter*(x/(f32)CycleTime);
						y=Counter*(y/(f32)CycleTime);
						z=Counter*(z/(f32)CycleTime);
					}
					break;
				case CYCLE_SINE:
					if (Counter>CycleTime){
						*CountInc = 0;
					}
					x+=x*sinf(6.28f*((f32)Counter/(f32)CycleTime));
					y+=y*sinf(6.28f*((f32)Counter/(f32)CycleTime));
					z+=z*sinf(6.28f*((f32)Counter/(f32)CycleTime));
					break;
			}
			PushTransform(Type,x/256.0f,y/256.0f,z/256.0f);
			*CmdSize += 24;
			*CmdLoc += 24;
			*PushCnt += 1;
			break;
		default:
			*CmdSize += 4;
			*CmdLoc += 4;
			break;
	}
	*CountInc += 1;
}
void Apply_Transform(struct TEState *CurEng,u8 *str,u8 *CmdLoc,u8 *CmdSize, u8 *PushCnt,s16 Timer){
	//Translation
	Process_Transformation(CurEng,str,CmdLoc,CmdSize,PushCnt,Timer,0);
	//Scale
	Process_Transformation(CurEng,str,CmdLoc,CmdSize,PushCnt,Timer+2,1);
	//Rotation
	Process_Transformation(CurEng,str,CmdLoc,CmdSize,PushCnt,Timer+4,2);
}
//7e cmd works
s8 TE_print_DL(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	u32 *ptr = segmented_to_virtual(TE_get_ptr(str+3,str));
	s16 Timer = TE_get_s16(str);
	u8 z=0;
	if (Timer==-1){
		str[1] = CurEng->BufferTimeBytes[0];
		str[2] = CurEng->BufferTimeBytes[1];
		for(z=0;z<3;z++){
			u32 *Tbuf = &TimerBuffer[CurEng->state][CurEng->BufferTimePtr];
			*Tbuf = str;
			*(Tbuf+1) = 0;
			CurEng->BufferTimePtr+=2;
		}
		Timer = TE_get_s16(str);
	}
	u8 PushCnt = 0;
	u8 CmdSize = 8;
	u8 CmdLoc = 8;
	u8 layer = str[3];
	Apply_Transform(CurEng,str,&CmdLoc,&CmdSize,&PushCnt,Timer);
	//set layer
	gDPSetRenderMode(gDisplayListHead++, renderModeTable_1Cycle->modes[layer], renderModeTable_2Cycle->modes[layer]);
	gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
	gSPDisplayList(gDisplayListHead++, ptr);
	for(z=0;z<PushCnt;z++){
		gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
	}
	gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
	return TE_print_adv(CurEng,CmdSize);
}
//7f cmd works
s8 TE_shaded_bg_box(struct TEState *CurEng,u8 *str){
	TE_bg_box_setup(CurEng);
	TE_bg_coords(CurEng,str);
	union WordByte Env;
	Env.w0 = TE_get_u32(str+8);
	gDPSetCombineMode(gDisplayListHead++,G_CC_FADE, G_CC_FADE);
	gDPSetEnvColor(gDisplayListHead++, Env.col[0], Env.col[1], Env.col[2], Env.col[3]);
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box_TE);
	TE_bg_box_finish(CurEng);
	return TE_print_adv(CurEng,13);
}
//80 cmd works
s8 TE_textured_bg_box(struct TEState *CurEng,u8 *str){
	TE_bg_box_setup(CurEng);
	TE_bg_coords(CurEng,str);
	u32 *ptr = segmented_to_virtual(TE_get_ptr(str+8,str));
	//pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt
	gDPLoadTextureBlock(gDisplayListHead++,ptr,G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box_TE);
	TE_bg_box_finish(CurEng);
	return TE_print_adv(CurEng,13);
}
//81 cmd works
s8 TE_push_transform(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	s16 Timer = TE_get_s16(str);
	u8 z=0;
	if (Timer==-1){
		str[1] = CurEng->BufferTimeBytes[0];
		str[2] = CurEng->BufferTimeBytes[1];
		for(z=0;z<3;z++){
			u32 *Tbuf = &TimerBuffer[CurEng->state][CurEng->BufferTimePtr];
			*Tbuf = str;
			*(Tbuf+1) = 0;
			CurEng->BufferTimePtr+=2;
		}
		Timer = TE_get_s16(str);
	}
	u8 PushCnt = 0;
	u8 CmdSize = 3;
	u8 CmdLoc = 3;
	Apply_Transform(CurEng,str,&CmdLoc,&CmdSize,&PushCnt,Timer);
	CurEng->TransformStack[CurEng->TransformStackPos] = PushCnt;
	CurEng->TransformStackPos++;
	return TE_print_adv(CurEng,CmdSize);
}
//82 cmd works
s8 TE_set_cutscene(struct TEState *CurEng,u8 *str){
	if(str[1] == 0 && gCurrentArea->camera->cutscene == 0){
		reset_camera(gCurrentArea->camera);
	}else{
		gCurrentArea->camera->cutscene = str[1];
		TE_add_to_cmd_buffer(CurEng,str,2);
	}
	return TE_advBlen(CurEng,2);
}
//84 cmd works
s8 TE_scale_text(struct TEState *CurEng,u8 *str){
	//TE print but with scale placed after resetting X pos
	if(CurEng->PlainText){
		s32 Env = CurEng->EnvColorWord;
		CurEng->EnvColorWord = 0x10101000 | CurEng->EnvColorByte[3];
		CurEng->TempX += 1;
		CurEng->TempY -= 1;
		if(!(StrBuffer[CurEng->state][0] == 0xFF))
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
	CurEng->ScaleU[0] = TE_get_u32(str);
	CurEng->ScaleU[1] = TE_get_u32(str+4);
	return TE_print_adv(CurEng,9);
}
//85 cmd works
s8 TE_enable_dialog_options(struct TEState *CurEng,u8 *str){
	u8 arrow = 0xFF;
	TE_print(CurEng);
	CurEng->TempY -= ((u16) 0xD*CurEng->ScaleF[1]);
	CurEng->TempYOrigin = CurEng->TempY;
	CurEng->TempXOrigin -= 1;
	CurEng->NumDialogs = str[1];
	if(CurEng->DialogEnd != 0 && (CurEng->LastVI+2)<gNumVblanks){
		if(gPlayer1Controller->buttonPressed&A_BUTTON){
			CurEng->OgStr = CurEng->DialogEnd;
			CurEng->StackLocked = CurEng->StackDepth;
			CurEng->DialogEnd = 0;
			CurEng->CurPos = 0;
			CurEng->StrEnd = 0;
			CurEng->NumDialogs = 0;
			CurEng->DisplayingDialog = 0;
			CurEng->LastVI = gNumVblanks;
			CurEng->ReturnedDialog = CurEng->HoveredDialog;
			return -1;
		}else{
			handle_menu_scrolling(MENU_SCROLL_VERTICAL,&CurEng->HoveredDialog,0,CurEng->NumDialogs);
			if(CurEng->HoveredDialog == 0){
				arrow = 0x53;
			}
		}
	}
	StrBuffer[CurEng->state][0] = arrow;
	StrBuffer[CurEng->state][1] = 0xFF;
	CurEng->TempX = CurEng->TempXOrigin-1;
	TE_print(CurEng);
	CurEng->TempXOrigin += ((u16)(8*CurEng->ScaleF[0]));
	CurEng->TempX = CurEng->TempXOrigin-1;
	CurEng->DisplayingDialog = 0;
	return TE_print_adv(CurEng,2);
}
//86 cmd works
s8 TE_dialog_response(struct TEState *CurEng,u8 *str){
	if(CurEng->ReturnedDialog == str[1]){
		TE_print(CurEng);
		return TE_print_adv(CurEng,2);
	}else{
		u16 off = 0;
		str += 2;
		off += 2;
		while(str[0] != 0x86 || str[1] != CurEng->ReturnedDialog){
			if(str[0] == 0x95){
				str = TE_mask_nested_dialog_option(CurEng,str);
			}else if(str[0] == 0x87){
				TE_print(CurEng);
				CurEng->TempStr+=off;
				return TE_print_adv(CurEng,1);
			}else{
				str += 1;
				off += 1;
			}
		}
		TE_print(CurEng);
		CurEng->TempStr+=off;
		CurEng->CurPos=0;
		return TE_print_adv(CurEng,2);
	}
}
//88 cmd works
s8 TE_screen_shake(struct TEState *CurEng,u8 *str){
	CurEng->ShakeScreen = str[1];
	return TE_advBlen(CurEng,2);
}
//8F cmd works
s8 TE_trigger_warp(struct TEState *CurEng,u8 *str){
	sDelayedWarpOp = 1;
	sDelayedWarpTimer = TE_get_u16(str);
	sSourceWarpNodeId = str[3];
	TE_add_to_cmd_buffer(CurEng,str,4);
	return TE_advBlen(CurEng,4);
}
//93 cmd works
s8 TE_start_bracket(struct TEState *CurEng,u8 *str){
	u8 key = str[1];
	str += 2;
	CurEng->TempStr += 2;
	while(str[0] != 0x94 || str[1] != key){
		str += 1;
		CurEng->TempStr += 1;
	}
	str += 2;
	CurEng->TempStr += 2;
	return 1;
}
//95 cmd works
u8 *TE_mask_nested_dialog_option(struct TEState *CurEng,u8 *str){
	u8 key = str[1];
	str += 2;
	CurEng->TempStr += 2;
	while(str[0] != 0x96 || str[1] != key){
		str += 1;
		CurEng->TempStr += 1;
	}
	str += 2;
	CurEng->TempStr += 2;
	return str;
}
//97 cmd works
s8 TE_set_return(struct TEState *CurEng,u8 *str){
	CurEng->ReturnStr[str[1]] = str+2;
	return TE_advBlen(CurEng,2);
}
//98 cmd works
s8 TE_goto_return(struct TEState *CurEng,u8 *str){
	if(CurEng->ReturnStr[str[1]] == 0){
		return TE_advBlen(CurEng,2);
	}else{
		CurEng->OgStr = CurEng->ReturnStr[str[1]];
		CurEng->StackLocked = CurEng->StackDepth;
		CurEng->StrEnd = 0;
		return -1;
	}
}
//99 cmd
s8 TE_enable_plaintext(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->PlainText = str[1];
	return TE_print_adv(CurEng,2);
}
//9a cmd works
s8 TE_enable_wobble(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	waveToggle = str[1];
	return TE_print_adv(CurEng,2);
}
//9b cmd works
s8 TE_enable_end_transition(struct TEState *CurEng,u8 *str){
	CurEng->TrEnd.TransLength = str[1];
	CurEng->TrEnd.TransAlpha = str[2];
	CurEng->TrEnd.TransDir = str[3];
	CurEng->TrEnd.TransSpeed = str[4];
	return TE_advBlen(CurEng,5);
}
//9c cmd works
s8 TE_enable_start_transition(struct TEState *CurEng,u8 *str){
	CurEng->TrStart.TransLength = str[1];
	CurEng->TrStart.TransAlpha = str[2];
	CurEng->TrStart.TransDir = str[3];
	CurEng->TrStart.TransSpeed = str[4];
	return TE_advBlen(CurEng,5);
}
//a0 cmd works
s8 TE_call_once(struct TEState *CurEng,u8 *str){
	u8 ID = str[1];
	u32 (*function)(u32,...) = TE_get_ptr(str+1,str);
	u8 i;
	u8 num = str[6];
	u32 args[num];
	for(i=0;i<num;i++){
		args[i] = TE_get_ptr(str+6+4*i,str);
	}
	u32 res = function(args[0],args[1],args[2],args[3]);
	FunctionReturns[CurEng->state][ID] = res;
	TE_add_to_cmd_buffer(CurEng,str,num*4+7);
	return TE_advBlen(CurEng,num*4+7);
}
//a1 cmd works
s8 TE_call_loop(struct TEState *CurEng,u8 *str){
	u8 ID = str[1];
	u32 (*function)(u32,...) = TE_get_ptr(str+1,str);
	u8 i;
	u8 num = str[6];
	u32 args[num];
	for(i=0;i<num;i++){
		args[i] = TE_get_ptr(str+6+4*i,str);
	}
	u32 res = function(args[0],args[1],args[2],args[3]);
	FunctionReturns[CurEng->state][ID] = res;
	return TE_advBlen(CurEng,num*4+7);
}
//a2 cmd works
s8 TE_function_response(struct TEState *CurEng,u8 *str){
	u32 key = FunctionReturns[CurEng->state][str[1]];
	if(key == TE_get_u32(str+1)){
		return TE_advBlen(CurEng,6);
	}else{
		str += 6;
		CurEng->TempStr += 6;
		while(1){
			if(str[0] == 0x87){
				return TE_advBlen(CurEng,1);
			}else if(str[0] != 0xA2){
				str += 1;
				CurEng->TempStr += 1;
			}else{
				key = FunctionReturns[CurEng->state][str[1]];
				if(key == TE_get_u32(str+1)){
					return TE_advBlen(CurEng,6);
				}else{
					str += 6;
					CurEng->TempStr += 6;
				}
			}
		}
	}
}
//a6 cmd works
s8 TE_set_mario_action(struct TEState *CurEng,u8 *str){
	gMarioState->action = TE_get_ptr(str,str);
	return TE_advBlen(CurEng,5);
}
//aa cmd works
s8 TE_box_transition(struct TEState *CurEng,u8 *str){
	CurEng->BoxTrXi = (s16) (TE_get_s16(str)*CurEng->TrPct);
	CurEng->BoxTrXf = (s16) (TE_get_s16(str+2)*CurEng->TrPct);
	CurEng->BoxTrYi = (s16) (TE_get_s16(str+4)*CurEng->TrPct);
	CurEng->BoxTrYf = (s16) (TE_get_s16(str+6)*CurEng->TrPct);
	return TE_advBlen(CurEng,9);
}
//ac cmd works
s8 TE_jump_link_str(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->TempStr = segmented_to_virtual(TE_get_ptr(str,str));
	CurEng->StrStack[CurEng->StackDepth] = str+5;
	CurEng->StackDepth++;
	CurEng->CurPos = 0;
	return 1;
}
//ad cmd works
s8 TE_pop_str(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	CurEng->TempStr = CurEng->StrStack[CurEng->StackDepth-1];
	CurEng->StackDepth--;
	//if you pop after a new box, you will lose  your jump and break
	//the chain of jump/pop, therefore you have to start a new box
	if(CurEng->StackDepth<CurEng->StackLocked){
		CurEng->OgStr = CurEng->TempStr;
		CurEng->StackLocked = CurEng->StackDepth;
		CurEng->StrEnd = 0;
		return -1;
	}
	CurEng->CurPos = 0;
	return 1;
}
//ae cmd works
s8 TE_enable_shake(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	shakeToggle = str[1];
	return TE_print_adv(CurEng,2);
}
//af cmd works
s8 TE_print_glyph(struct TEState *CurEng,u8 *str){
	TE_bg_box_setup(CurEng);
	/*bg box coords*/
	f32 x1 = (f32) (CurEng->TempX+CurEng->TransX+1);
	f32 y1 = (f32) (CurEng->TempY+CurEng->TransY+1);
	f32 x2 = x1+12*CurEng->ScaleF[0];
	f32 y2 = y1+12*CurEng->ScaleF[1];
	create_dl_scale_matrix(MENU_MTX_PUSH, (x2-x1)/130.0f,(y2-y1)/80.0f, 1.0f);
	create_dl_translation_matrix(MENU_MTX_NOPUSH, 130.0f*(x1/(x2-x1)),80.0f*(y2)/(y2-y1), 1.0f);
	/* end bg box cords*/
	u32 *ptr = segmented_to_virtual(TE_get_ptr(str,str));
	//pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt
	gDPLoadTextureBlock(gDisplayListHead++,ptr,G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
	CurEng->PrevEnvColorWord = -1;
	gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box_TE);
	CurEng->TotalXOff+=14;
	TE_bg_box_finish(CurEng);
	return TE_print_adv(CurEng,5);
}
//b0 cmd works
s8 TE_word_wrap(struct TEState *CurEng,u8 *str){
	CurEng->WordWrap = TE_get_u16(str);
	return TE_advBlen(CurEng,3);
}
//fe cmd works
s8 TE_line_break(struct TEState *CurEng,u8 *str){
	//modified print function to make printing not fuck with X pos
	//rather inefficient but I'm lazy
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
	CurEng->TempX = CurEng->TempXOrigin;
	CurEng->TempY -= ((u16) 0xD*CurEng->ScaleF[1]);
	CurEng->TempYOrigin -= ((u16) 0xD*CurEng->ScaleF[1]);
	CurEng->TotalXOff = 0;
	return TE_print_adv(CurEng,1);
}
//ff cmd works
s8 TE_terminator(struct TEState *CurEng,u8 *str){
	TE_print(CurEng);
	if(CurEng->DisplayingDialog == CurEng->NumDialogs){
		CurEng->DialogEnd = str+1;
		return -1;
	}
	CurEng->DisplayingDialog += 1;
	CurEng->TempX = CurEng->TempXOrigin-1;
	CurEng->TempY -= ((u16) 0xD*CurEng->ScaleF[1]);
	CurEng->TempYOrigin -= ((u16) 0xD*CurEng->ScaleF[1]);
	if(CurEng->HoveredDialog == CurEng->DisplayingDialog){
		StrBuffer[CurEng->state][0] = 0x53;
		StrBuffer[CurEng->state][1] = 0xFF;
		CurEng->TempX -= gDialogCharWidths[0x53];
		TE_print(CurEng);
		CurEng->TempX = CurEng->TempXOrigin-1;
	}
	CurEng->TotalXOff = 0;
	return TE_print_adv(CurEng,1);
}