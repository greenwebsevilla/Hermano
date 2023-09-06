#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Music.h"


IMPORT_MAP(gameover_screen);
DECLARE_MUSIC(gameover);

extern UINT16 tiempo;


void START() {
	
	InitScroll(BANK(gameover_screen), &gameover_screen, 0, 0);

	PlayMusic(gameover, 0);
	tiempo = 500;

}





void UPDATE() {
	tiempo --;
			
	if (tiempo == 0) SetState(StateMenu);
		
		
}
