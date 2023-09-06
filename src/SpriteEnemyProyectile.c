#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Scroll.h"


extern unsigned char half_life;
extern UINT8 delta_time;

typedef struct {
	INT8 vx;
	INT16 vy;
	INT8 inc_x;
	INT8 inc_y;
	INT8 gravedad;
	INT16 bomb_y;
} CUSTOM_DATA;

void CreateEnemyProyectile(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED {
	Sprite* spr = SpriteManagerAdd(SpriteEnemyProyectile, x, y);
	CUSTOM_DATA* data = (CUSTOM_DATA*)spr->custom_data;
	
	data->vx = vx_;
	data->vy = vy_;
	data->gravedad = gravity_;
	data->bomb_y = y << 6;
	THIS->lim_y = 32;
	PlayFx(CHANNEL_1, 5, 0x3d, 0x4a, 0x6d, 0x99, 0xc6);
}


void START() { 
	THIS->estado = 1;
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if(THIS->estado == 1) {
		
		SetFrame(THIS, 0);
		
		data->vy += data->gravedad;
		data->bomb_y += data->vy << delta_time;
		
		if (half_life == 0) THIS->x += (INT16)data->vx << delta_time;
		THIS->y = data->bomb_y >> 6;
		
	} else {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void DESTROY() { 
}