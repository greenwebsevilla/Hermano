#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Scroll.h"


//Animations
const UINT8 anim_cojon_quietor[] = {1, 2};
const UINT8 anim_cojon_saltando[] = {1, 3};
const UINT8 anim_cojon_salta[] = {2, 1, 0};
extern unsigned char half_life;
extern signed int x_player;

extern UINT8 delta_time;

typedef struct {
	INT8 vx;
	INT16 vy;
	INT8 gravedad;
	UINT16 cojon_y;
	UINT16 init_y;
} CUSTOM_DATA;


void check_direction(){
	
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if (THIS->x > x_player){
		data->vx = -1; 
		THIS->mirror = V_MIRROR;
	}else{
		data->vx = 1;
		THIS->mirror = NO_MIRROR;
	}
	
}

void START() { 
	THIS->estado = 3;
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	check_direction();
	
	THIS->y -= 4;
	
	data->vy = 0;
	data->gravedad = 8;
	data->cojon_y = THIS->y << 6;
	data->init_y = data->cojon_y;
	THIS->contador_tiempo = 15;
	THIS->life = 2;
	
	THIS->lim_x = 64;
	THIS->lim_y = 0;
	
	
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	THIS->contador_tiempo --;
	
	if (THIS->tocado > 0) {
		if (THIS->tocado == 10){
		
			THIS->life --;
			if (THIS->life > 200){
				THIS->estado = 99;
			}
			
		}
		
		THIS->tocado --;
		THIS->visible = half_life;
		if (THIS->tocado == 0) THIS->visible = 1u;
	}
	
	
	switch(THIS->estado) {
		
		case 1:
		
			SetSpriteAnim(THIS, anim_cojon_quietor, 30u);
			
			if (THIS->contador_tiempo == 0){
				THIS->contador_tiempo = 10;
				THIS->estado = 2;
				
			}
			
		break;
		
		case 2:
			SetSpriteAnim(THIS, anim_cojon_salta, 15u);
			if (THIS->contador_tiempo == 0){
				THIS->estado = 3;
				data->vy = -150;
			}
		
		break;
		
	    case 3:
			SetSpriteAnim(THIS, anim_cojon_saltando, 30u);
			data->vy += data->gravedad;
			data->cojon_y += data->vy;

			if (half_life == 1) THIS->x += (INT16)data->vx << delta_time;
			
			//Colision con escenario
			THIS->y = data->cojon_y >> 6;
			if(data->vy > 0) {
				if(GetScrollTile((THIS->x + 8)>> 3, (THIS->y + 20) >> 3) < MAX_TILE_TRASPASABLE) {
					THIS->estado = 1; //estado parado
					THIS->contador_tiempo = 30;
					// PlayFx(CHANNEL_4, 10, 0x38, 0xc2, 0x50, 0x80);
					THIS->y &= 0xFFFC;
					data->cojon_y = THIS->y << 6;
					check_direction();
				}
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
	
	
}

void DESTROY() { 
	
}