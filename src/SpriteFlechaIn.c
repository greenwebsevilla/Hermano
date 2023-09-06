#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

extern unsigned char has_key;

void START() { 
	if (has_key == 0) {
			SpriteManagerRemove(THIS_IDX);
			return;
		}
}

void UPDATE() {
	
	THIS->contador_tiempo++;
	
	if (THIS->contador_tiempo > 20){
		THIS->visible = 0;
		if (THIS->contador_tiempo > 40) THIS->contador_tiempo = 0;
	}else THIS->visible = 1;
	
}

void DESTROY() { 
}