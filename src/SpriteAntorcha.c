#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
const UINT8 anim_antorcha[] = {3, 0, 1, 2};
// const UINT8 anim_apagada[] = {1, 3};

void START() { 
	THIS->estado = 1; //Necesario para las colisiones
	THIS->lim_x = 64;
	THIS->lim_y = 8;
	THIS->x += 2;
	THIS->y += 8;
	THIS->life = 0;
	SetSpriteAnim(THIS, anim_antorcha, 10u);
}

void UPDATE() {
	
	if(THIS->estado == 99) {
	THIS->visible = 0;
		// SetSpriteAnim(THIS, anim_apagada, 1u);
	}
}

void DESTROY() { 
}