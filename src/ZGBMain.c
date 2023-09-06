#include "ZGBMain.h"
#include "Math.h"

//Custom variables TOTALES


UINT16 tiempo = 0;
unsigned char level, has_key, vidas;
signed char energy;
signed char desplazamiento_y_camara;
unsigned char bombas, frame_counter;
unsigned char mundo_actual, autoscroll, vidaextra;
unsigned char bombas_disparadas, half_life, disparando;
UINT16 x_player, x_checkpoint, y_checkpoint;
signed int y_player;
unsigned char bocabajo = 0;

// unsigned int *inframundo_pointer;
unsigned int y_eje_actual;

UINT8 next_state = StateIntro;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	
	if(current_state == StateGame) {
		if(U_LESS_THAN(255 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = *(tile_ptr+1);
			return 255 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}
	return 255u;
	
}

