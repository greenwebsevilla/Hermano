#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"
// #include "Print.h"
typedef struct {
	UINT32 y_scroll;
} CUSTOM_DATA;

void START() { 

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	THIS->y = 40;
	data->y_scroll = (UINT32)(THIS->y) << 6;
	THIS->vy = 20; 
	THIS->visible = 0;
}

void UPDATE() {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	if (THIS->y < scroll_y + 144){
		data->y_scroll += THIS->vy;
		THIS->y = (UINT16)(data->y_scroll >> 6);
	}else{
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void DESTROY() { 
	SetState(StateMenu);
}