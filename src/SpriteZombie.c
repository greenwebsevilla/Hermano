#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

//Animations
const UINT8 anim_zombie_walk[] = {4, 0, 1, 2, 1};
extern UINT8 delta_time;
extern unsigned char half_life;

typedef struct {
	INT8 vx;
	INT8 offset_x;
	INT16 limite_xi, limite_xd;
} CUSTOM_DATA;

void START() { 

	THIS->attr_add |= S_PALETTE;

	THIS->estado = 1; //Necesario para las colisiones
	THIS->lim_x = 32;
	THIS->lim_y = 0;
	
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	if (THIS->x > scroll_x) data->vx = -1; else data->vx = 1;
	
	data->limite_xi = THIS->x - 90;
	data->limite_xd = THIS->x + 90;
	
	THIS->y -= 2;
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if(THIS->estado == 1) {
		
		//Tocado y muere
		if (THIS->tocado > 0) {
			THIS->estado = 99;
		}
		
		if (data->vx < 0){
			THIS->mirror = V_MIRROR;
			data->offset_x = -4;
		}
		if (data->vx > 0){
			THIS->mirror = NO_MIRROR;
			data->offset_x = 20;
		}
		
		if(GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y + 25) >> 3) > MAX_TILE_TRASPASABLE ||
			GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y + 12) >> 3) < MAX_TILE_SOLIDO || THIS->x < 4 )
		{
			data->vx = -data->vx;
		}else if (THIS->x < data->limite_xi)
		{
			data->vx = 1;
		}else if (THIS->x > data->limite_xd)
		{
			data->vx = -1;
		}
		
		
		
		if (half_life == 0) THIS->x += data->vx << delta_time;
		
		SetSpriteAnim(THIS, anim_zombie_walk, 12u);
		
	} else {
		THIS->visible = 0;
		if (THIS->estado == 99){
			SpriteManagerAdd(SpriteSoul, THIS->x + 4, THIS->y + 4);
			THIS->estado ++;
		}
	}
}

void DESTROY() { 

	

}