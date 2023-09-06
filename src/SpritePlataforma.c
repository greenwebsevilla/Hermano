#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"
#include "Print.h"
typedef struct {
	UINT16 init_y;
	INT16 platform_y;
} CUSTOM_DATA;

void START() { 
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	THIS->estado = 1; //Necesario para las colisiones
	data->init_y = THIS->y;
	data->platform_y = THIS->y << 6;
	THIS->vy = -32;
	
	THIS->lim_x = 16;
	THIS->lim_y = 32;
	
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->platform_y += THIS->vy;
	
	THIS->y = data->platform_y >> 6;
	
	if (THIS->y < data->init_y - 40 || THIS->y > data->init_y + 40) THIS->vy = - THIS->vy;
	
	DPRINT_POS(0, 0);
	DPrintf("x:%d y:%d  ", THIS->y, THIS->y);
}

void DESTROY() { 
}