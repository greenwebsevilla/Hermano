#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

// void START() { 
	// THIS->estado = 1; //Necesario para las colisiones
	// THIS->y += 12;
	// THIS->x += 12;

// }

extern unsigned int y_eje_actual;

void START() { 
	THIS->x += 12;
	THIS->estado = 1; //Necesario para las colisiones
	
	if ((INT16)THIS->y > (INT16)y_eje_actual) { 
		THIS->vy = -1;
		THIS->tocado = 0;		//Use "tocado" as Y offset to detect collision with ground
		THIS->mirror = H_MIRROR;
	}else{
		THIS->vy = 1;
		THIS->tocado = 11;
		THIS->mirror = NO_MIRROR;
	}
}

void UPDATE() {
	THIS->life = GetScrollTile((THIS->x+8)>>3, (THIS->y+THIS->tocado)>>3); //Use life as auxiliar variable to save the tile numer
	if(THIS->life >= MAX_TILE_TRASPASABLE) THIS->y += THIS->vy;

}

void DESTROY() { 
}