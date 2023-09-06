#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

extern unsigned char has_key;

/*
has_key = 0 -- NO TIENE LA LLAVE
has_key = 1 -- SI TIENE LA LLAVE
has_key = 2 -- SI TIENE LA LLAVE Y HA GUARDADO CHECKPOINT
*/

void START() { 
	if (has_key > 0) {
			SpriteManagerRemove(THIS_IDX);
			return;
		}
		
	THIS->estado = 1; //Necesario para las colisiones
	THIS->x += 4;
}

void UPDATE() {
	
}

void DESTROY() { 
}