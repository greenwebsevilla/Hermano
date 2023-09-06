#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

void START() { 
// THIS->attr_add |= S_PALETTE;
THIS->lim_y = 10;
SetFrame(THIS, 0);
}

void UPDATE() {
	THIS->estado ++;
	
	if (THIS->estado > 40) THIS->estado = 0;
	
	if(THIS->estado < 20) {
		THIS->y = 94;
	} else {
		THIS->y = 144;
	}
}

void DESTROY() { 
}