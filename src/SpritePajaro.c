#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

//Animations
const UINT8 anim_flying[] = {4, 0, 1, 2, 1};

extern UINT8 delta_time;
extern unsigned int x_player;
extern signed int y_player;
void CreateEnemyProyectile(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED;

extern unsigned char half_life;
typedef struct {
	INT8 vx;
	UINT16 x_scroll_base;
	UINT8 disparado;
} CUSTOM_DATA;

void START() { 

	THIS->attr_add |= S_PALETTE;

	THIS->estado = 1; //Necesario para las colisiones
	THIS->lim_x = 16;
	THIS->lim_y = 0;
	
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->x_scroll_base = scroll_x;
	if (THIS->x > scroll_x){
		data->vx = -1;
		THIS->mirror = V_MIRROR;
	}else{
		data->vx = 1;
		THIS->mirror = NO_MIRROR;
	}
	
	data->disparado = 0;
	
	THIS->y -= 4;
	THIS->life = 0;
	
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	switch (THIS->estado) {
		
		case 1:
		
			//Tocado y muere
			if (THIS->tocado > 0) {
				THIS->estado = 99;
			}
			
			if (data->disparado == 0){
				if( THIS->x > (x_player - 10)){
					if( THIS->x < (x_player + 10))
					{
						CreateEnemyProyectile(THIS->x+8, THIS->y + 16, 0, 0, 10);
						data->disparado = 1;
					}
				}
			}
	
			THIS->x += data->vx << delta_time;

			SetSpriteAnim(THIS, anim_flying, 20u);
		
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