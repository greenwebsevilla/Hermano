#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

extern unsigned char half_life, mundo_actual;

void START() { 
	THIS->lim_x = 100;
}

void UPDATE() {
	half_life != half_life;
	SetFrame(THIS, mundo_actual);
	
	if (THIS->x < 120) THIS->x += 2;
}

void DESTROY() { 
}