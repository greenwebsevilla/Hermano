#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"


//Animations
const UINT8 anim_topo_walk[] = {4, 0, 1, 2, 1};

extern unsigned char half_life;
extern unsigned int y_eje_actual;
extern UINT8 delta_time;

typedef struct {
	UINT8 facedown;
	INT8 vx, vy;
	INT8 offset_x;
	INT16 offset_y;
	UINT8 last_mirror;
} CUSTOM_DATA;

void START() { 
	THIS->estado = 1; //Necesario para las colisiones
	THIS->lim_x = 64;
	THIS->lim_y = 64;
	
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	if (THIS->x > scroll_x) data->vx = -1; else data->vx = 1;
	
	
	
	if( GetScrollTile((THIS->x)>> 3, (THIS->y - 5) >> 3) < MAX_TILE_SOLIDO ) { 
		data->facedown = 1;
		data->offset_y = -1;
	}else{
		data->facedown = 0;
		data->offset_y = 13;
	}
	
	data->last_mirror = 0;
	THIS->life = 0;
	
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;


	
	switch (THIS->estado) {
		
		case 1: //Andando
		
			//Tocado y muere
			if (THIS->tocado > 0) {
				THIS->estado = 99;
			}
			
			if (data->vx < 0){
				data->last_mirror = V_MIRROR;
				data->offset_x = -4;
			}
			if (data->vx > 0){
				data->last_mirror = NO_MIRROR;
				data->offset_x = 20;
			}
			
			
			
			if( GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y + data->offset_y) >> 3) > MAX_TILE_SOLIDO )
			{
				// data->vx = -data->vx;
				THIS->estado = 2; 
				data->facedown = !data->facedown;
				
				if (data->facedown == 1){
					data->offset_y = - 1;
					data->vy = -2;
				}else{
					data->offset_y = 13;
					data->vy = 2;
				}
			}
			THIS->x += data->vx << delta_time;
			
			SetSpriteAnim(THIS, anim_topo_walk, 20u);
			
		break;
		
		case 2:  //caida a otra altura
			SetFrame(THIS, 1);
			THIS->y += data->vy << delta_time;
			if( GetScrollTile((THIS->x)>> 3, (THIS->y + data->offset_y) >> 3) < MAX_TILE_SOLIDO ||
			GetScrollTile((THIS->x + 16)>> 3, (THIS->y + data->offset_y) >> 3) < MAX_TILE_SOLIDO
			){
				
				THIS->estado = 1;
				if( GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y +data->offset_y) >> 3) > MAX_TILE_SOLIDO ) data->vx = -data->vx;
			}
			
		
		break;
		
		default:

			if (THIS->estado == 99){
				SpriteManagerAdd(SpriteSoul, THIS->x + 4, THIS->y + 4);
				THIS->estado ++;
				
			}
			THIS->visible = 0; //Hacerlo invisible
		
		break;
	}
	THIS->mirror = data->last_mirror + data->facedown;
}

void DESTROY() { 

	

}