#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
const UINT8 anim_pincho[] = {2, 0, 1};
void START() { 
	THIS->estado = 1; //Necesario para las colisiones
	THIS->lim_x = 255;
	THIS->lim_y = 255;
	THIS->y -= 2;
	THIS->life = 0;
	SetSpriteAnim(THIS, anim_pincho, 12u);
}

void UPDATE() {
	
	if(THIS->estado == 99) {
		THIS->visible = 0;
	}
}

void DESTROY() { 
}