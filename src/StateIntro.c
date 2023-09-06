#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Music.h"
#include <Palette.h>
#include <Fade.h>

IMPORT_MAP(zgb_logo);
IMPORT_MAP(pat_logo);
IMPORT_MAP(compo_logo);
IMPORT_MAP(intro1);
IMPORT_MAP(intro2);
IMPORT_MAP(intro3);
IMPORT_MAP(intro4);
DECLARE_MUSIC(intro);

UINT8 pantalla;
UINT16 temporizador;
UINT16 next_time;

UINT16 tiempo_cambios_pantalla[] = {350,320,460,360,340,300,0};
void pausa(unsigned int time) BANKED;

void START() {
	
	//Init SFX
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0xF7; //Max volume
	
	//Change Sprite Palettes
	OBP0_REG = PAL_DEF(0,1,3,2); //Cambia la paleta 0
	OBP1_REG = PAL_DEF(1,0,3,2); //Cambia la paleta 1
	
	InitScroll(BANK(zgb_logo), &zgb_logo, 0, 0);

	PlayMusic(intro, 0);
	// SpriteManagerAdd(SpritePress, 40, 94);
	// SpriteManagerAdd(SpriteOjillos, 131, 38);
	pantalla = 0;
	next_time = 200;
	temporizador = 0;
}

void UPDATE() {
	temporizador++;
	
	
	if(temporizador == next_time) {
		next_time = next_time + tiempo_cambios_pantalla[pantalla];
		pantalla++;
		FadeIn();
		DISPLAY_OFF;
		
		
		switch (pantalla){
			
			case 1:  
				InitScroll(BANK(pat_logo), &pat_logo, 0, 0);
				
			break;			
			
			case 2:  
				InitScroll(BANK(compo_logo), &compo_logo, 0, 0);

			break;			
			
			case 3:  
				InitScroll(BANK(intro1), &intro1, 0, 0);

			break;	
			
			case 4:  
				InitScroll(BANK(intro2), &intro2, 0, 0);

			break;
						
			case 5:  
				InitScroll(BANK(intro3), &intro3, 0, 0);

			break;
						
			case 6:  
				InitScroll(BANK(intro4), &intro4, 0, 0);

			break;
			
			case 7:
				SetState(StateMenu);
			break;
		}
		
		if (pantalla < 7){ 
			BGP_REG = OBP0_REG = OBP1_REG = PAL_DEF(0, 1, 2, 3);
			DISPLAY_ON;
			FadeOut();
		}
		
	}
	
	if(KEY_TICKED(J_START)) {
		SetState(StateMenu);
	}
}