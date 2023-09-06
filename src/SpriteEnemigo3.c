#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
// #include "Scroll.h"

// MANOS EN SUELO

//Animations
const UINT8 anim_enem3[] = {4, 0, 1, 2, 1};

void START() { 

	SetSpriteAnim(THIS, anim_enem3, 12u);
	THIS->estado = 1;

}

void UPDATE() {

	//Tocado y muere
	if (THIS->tocado > 0) {
		THIS->estado = 99;
	}
	
	if(THIS->estado != 1) {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void DESTROY() { 
	
}