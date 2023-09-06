#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Music.h"
#include <Palette.h>

IMPORT_MAP(menu);
IMPORT_MAP(tutorial_screen);
DECLARE_MUSIC(title);

UINT8 tutorial = 0;

extern unsigned char level, mundo_actual, vidas, has_key;
extern unsigned int x_checkpoint, y_checkpoint;

void START() {
	
	//Init SFX
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0xF7; //Max volume
	
	//Change Sprite Palettes
	OBP0_REG = PAL_DEF(0,1,3,2); //Cambia la paleta 0
	OBP1_REG = PAL_DEF(1,0,3,2); //Cambia la paleta 1
	
	mundo_actual = 0;
	level = 0;
	
	has_key = 0;
	vidas = 3;
	
	x_checkpoint = 0;
	y_checkpoint = 0;

	InitScroll(BANK(menu), &menu, 0, 0);

	PlayMusic(title, 0);
	SpriteManagerAdd(SpritePress, 40, 94);
	SpriteManagerAdd(SpriteOjillos, 131, 38);

}





void UPDATE() {
	
	if(KEY_TICKED(J_START)) {
		
		if (tutorial == 0){
			tutorial = 1;
			DISPLAY_OFF;
			StopMusic;
			SpriteManagerReset();
			InitScroll(BANK(tutorial_screen), &tutorial_screen, 0, 0);
			DISPLAY_ON;
		}else SetState(StateTituloNivel);
	}
	
	if(KEY_TICKED(J_SELECT)) {
		SetState(StateCredits);
	}
	
}