#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"
// #include "Print.h"
typedef struct {
	UINT32 x_scroll;
} CUSTOM_DATA;

void START() { 
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	// THIS->estado = 1; //Necesario para las colisiones

	data->x_scroll = (UINT32)(THIS->x) << 6;
	THIS->vy = 20; //Uso vy como vx en este caso. Hasta este momento no he usado vx en la estructura de los Sprites.
	THIS->visible = 0;
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	if (THIS->x < scroll_x + 160){
		data->x_scroll += THIS->vy;
		THIS->x = (UINT16)(data->x_scroll >> 6);
	}
}

void DESTROY() { 
}