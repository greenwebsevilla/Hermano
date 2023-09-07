#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "BankManager.h"
#include "Math.h"
#include "main.h"
#include <string.h>


#define SCREEN_TILES_W       20 // 160 >> 3 = 20
#define SCREEN_TILES_H       18 // 144 >> 3 = 18
#define SCREEN_PAD_LEFT   1
#define SCREEN_PAD_RIGHT  2
#define SCREEN_PAD_TOP    1
#define SCREEN_PAD_BOTTOM 2

#define SCREEN_TILE_REFRES_W (SCREEN_TILES_W + SCREEN_PAD_LEFT + SCREEN_PAD_RIGHT)
#define SCREEN_TILE_REFRES_H (SCREEN_TILES_H + SCREEN_PAD_TOP  + SCREEN_PAD_BOTTOM)

extern UINT8 scroll_top_movement_limit = 30;
extern UINT8 scroll_bottom_movement_limit = 100;

extern unsigned char bocabajo;
extern signed char desplazamiento_y_camara;


//To be defined on the main app
UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile);

unsigned char* scroll_map = 0;
unsigned char* scroll_cmap = 0;
INT16 scroll_x = 0;
INT16 scroll_y = 0;
UINT8 scroll_x_vblank = 0;
UINT8 scroll_y_vblank = 0;
UINT16 scroll_w;
UINT16 scroll_h;
UINT16 scroll_tiles_w;
UINT16 scroll_tiles_h;
Sprite* scroll_target = 0;
UINT8 scroll_collisions[256];
UINT8 scroll_collisions_down[256];
UINT8 scroll_tile_info[256];
UINT8 scroll_bank;
UINT8 scroll_offset_x = 0;
UINT8 scroll_offset_y = 0;

INT16 pending_h_x, pending_h_y;
UINT8 pending_h_i;
unsigned char* pending_h_map = 0;
unsigned char* pending_w_map = 0;
#ifdef CGB
unsigned char* pending_h_cmap = 0;
unsigned char* pending_w_cmap = 0;
#endif
INT16 pending_w_x, pending_w_y;
UINT8 pending_w_i;

UINT8 last_tile_loaded = 0;
UINT8 last_bg_pal_loaded = 0;

UINT16 hud_map_offset;

//Keeping track of the current tiles loaded on offset 0
UINT8 tiles_bank_0;
const struct TilesInfo* tiles_0;

extern UINT8 vbl_count;
UINT8 current_vbl_count;
void SetTile(UINT16 r, UINT8 t) OLDCALL{
	r; t;
	//while((STAT_REG & 0x2) != 0);
	//*(__REG)(r) = t;
__asm
;bc = r, hl = t
	ldhl	sp,#2
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	ldhl	sp,#4

;while 0xff41 & 02 != 0 (cannot write)
1$:
	ld	a,(#_STAT_REG)
	and	a, #0x02
	jr	NZ,1$

;Write tile
	ld	a,(hl)
	ld	(bc),a
	ret
__endasm;
}

void UPDATE_TILE(INT16 x, INT16 y, UINT8* t, UINT8* c) {
	UINT8 replacement = *t;
	UINT8 i;
	Sprite* s = 0;
	UINT8 type = 255u;
	UINT16 id = 0u;
	UINT16 sprite_y;
	c;

	if((UINT16)x >= scroll_tiles_w || (UINT16)y >= scroll_tiles_h) { //This also checks x < 0 || y < 0
		replacement = 0;
	} else {
		type = GetTileReplacement(t, &replacement);
		if(type != 255u) {
			id = SPRITE_UNIQUE_ID(x, y);
			for(i = 0u; i != sprite_manager_updatables[0]; ++i) {
				s = sprite_manager_sprites[sprite_manager_updatables[i + 1]];
				if((s->type == type) && (s->unique_id == id)) {
					break;
				}
			}

			if(i == sprite_manager_updatables[0]) {
				PUSH_BANK(spriteDataBanks[type]);
					sprite_y = ((y + 1) << 3) - spriteDatas[type]->height;
				POP_BANK;
				s = SpriteManagerAdd(type, x << 3, sprite_y);
			}
		}
	}

	id = 0x9800 + (0x1F & (x + scroll_offset_x)) + ((0x1F & (y + scroll_offset_y)) << 5);
	SetTile(id, replacement);
	

	#ifdef CGB
		if (_cpu == CGB_TYPE) {
			VBK_REG = 1;
			if(!scroll_cmap) {
				c = &scroll_tile_info[replacement];
			}
			set_bkg_tile_xy(0x1F & (x + scroll_offset_x), 0x1F & (y + scroll_offset_y), *c);
			VBK_REG = 0;
		}
	#endif
}

extern UWORD ZGB_Fading_BPal[32];
UINT16 ScrollSetTiles(UINT8 first_tile, UINT8 tiles_bank, const struct TilesInfo* tiles) {
	UINT8 i;
	UINT8 n_tiles;
	UINT8* palette_entries;

	UINT16 offset = first_tile;

	if(first_tile == 0)
	{
		tiles_bank_0 = tiles_bank;
		tiles_0 = tiles;
	}

	PUSH_BANK(tiles_bank);
	n_tiles = tiles->num_frames;
	palette_entries = tiles->color_data;

	set_bkg_data(first_tile, n_tiles, tiles->data);
	last_tile_loaded = first_tile + n_tiles;
	for(i = first_tile; i != last_tile_loaded; ++i) {
		scroll_tile_info[i] = palette_entries ? palette_entries[i - first_tile] : 0;
	}

#ifdef CGB
	//Load palettes
	for(i = 0; i != last_bg_pal_loaded; ++ i)
	{
		if(memcmp(&ZGB_Fading_BPal[i << 2], tiles->pals, tiles->num_pals << 3) == 0)
			break;
	}

	offset |= (i << 8);
	if(i == last_bg_pal_loaded)
	{
		SetPalette(BG_PALETTE, last_bg_pal_loaded, tiles->num_pals, tiles->pals, tiles_bank);
		last_bg_pal_loaded += tiles->num_pals;
	}
#endif

	POP_BANK;

	return offset;
}

void UpdateMapTile(UINT8 bg_or_win, UINT8 x, UINT8 y, UINT16 map_offset, UINT8 data, UINT8* attr)
{
attr;
	UINT8* offsetts = &map_offset;
	data += offsetts[0];
	if(bg_or_win == 0)
		set_bkg_tile_xy(x, y, data);
	else
		set_win_tile_xy(x, y, data);

#ifdef CGB
	if (_cpu == CGB_TYPE) {
		VBK_REG = 1;

		UINT8 c = attr ? *attr : scroll_tile_info[data];
		c += offsetts[1];

		if(bg_or_win == 0)
			set_bkg_tile_xy(x, y, c);
		else
			set_win_tile_xy(x, y, c);
		VBK_REG = 0;
	}
#endif
}

UINT16 LoadMap(UINT8 bg_or_win, UINT8 x, UINT8 y, UINT8 map_bank, struct MapInfo* map) {
	PUSH_BANK(map_bank);

	//Load Tiles
	UINT8 load_tiles = tiles_bank_0 != map->tiles_bank || tiles_0 != map->tiles; //If the tile set is the same as the one used for the scroll or the bg (which is stored in tiles_bank_0 and tiles0) then do not load the tiles again
	UINT16 map_offset = 0;
	if(load_tiles)
		map_offset = ScrollSetTiles(last_tile_loaded, map->tiles_bank, map->tiles);

	//Load map (tile by tile because it there are not attributes when need to pick them from scroll_tile_info)
	UINT8* data = map->data;
	UINT8* attrs = map->attributes;
	for(UINT8 j = 0; j < map->height; ++j) {
		for(UINT8 i = 0; i < map->width; ++i) {
			UpdateMapTile(bg_or_win, x + i, y + j, map_offset, *data, attrs);
			
			++ data;
			if(attrs)
				++ attrs;
		}
	}

	POP_BANK;
	
	//Return the offset so the user can pass it as parameter to UpdateMapTile
	return map_offset;
}

INT8 scroll_h_border = 0;
UINT8 clamp_enabled = 1;
void ClampScrollLimits() {
	if(clamp_enabled) {
		if(U_LESS_THAN(scroll_x, 0u)) {
			scroll_x = 0u;		
		}
		if(scroll_x > (scroll_w - SCREENWIDTH)) {
			scroll_x = (scroll_w - SCREENWIDTH);
		}
		if(U_LESS_THAN(scroll_y, 0u)) {
			scroll_y = 0u;		
		}
		if(scroll_y > (scroll_h - SCREENHEIGHT + scroll_h_border)) {
			scroll_y = (scroll_h - SCREENHEIGHT + scroll_h_border);
		}
	}
}

void ScrollSetMap(UINT8 map_bank, const struct MapInfo* map) {
	PUSH_BANK(map_bank);
	scroll_tiles_w = map->width;
	scroll_tiles_h = map->height;
	scroll_map = map->data;
	scroll_cmap = map->attributes;
	scroll_x = 0;
	scroll_y = 0;
	scroll_w = scroll_tiles_w << 3;
	scroll_h = scroll_tiles_h << 3;
	scroll_bank = map_bank;
	if(scroll_target) {
		// scroll_x = scroll_target->x - (SCREENWIDTH >> 1);
		// scroll_y = scroll_target->y - scroll_bottom_movement_limit; //Move the camera to its bottom limit
		
		//CENTRAR LA CAMARA EN EL PLAYER
		scroll_x = scroll_target->x + 8 - (SCREENWIDTH >> 1);
		scroll_y = scroll_target->y + (INT16)(bocabajo<<5) - (SCREENHEIGHT >> 1); //Move the camera to its bottom limit

		ClampScrollLimits();
	}
	pending_h_i = 0;
	pending_w_i = 0;
	POP_BANK;
}

void InitScroll(UINT8 map_bank, const struct MapInfo* map, const UINT8* coll_list, const UINT8* coll_list_down) {
	UINT8 tiles_bank;
	struct TilesInfo* tiles;

	//Init Tiles
	PUSH_BANK(map_bank)
		tiles_bank = map->tiles_bank;
		tiles = map->tiles;
	POP_BANK;
	
	InitScrollWithTiles(map_bank, map, tiles_bank, tiles, coll_list, coll_list_down);
}

void InitScrollWithTiles(UINT8 map_bank, const struct MapInfo* map, UINT8 tiles_info_bank, const struct TilesInfo* tiles_info, const UINT8* coll_list, const UINT8* coll_list_down)
{
	UINT8 i;
	INT16 y;

	ScrollSetTiles(0, tiles_info_bank, tiles_info);

	ScrollSetMap(map_bank, map);

	memset(scroll_collisions, 0, sizeof(scroll_collisions));
	memset(scroll_collisions_down, 0, sizeof(scroll_collisions_down));

	if(coll_list) {
		for(i = 0u; coll_list[i] != 0u; ++i) {
			scroll_collisions[coll_list[i]] = 1u;
			scroll_collisions_down[coll_list[i]] = 1u;
		}
	}
	if(coll_list_down) {
		for(i = 0u; coll_list_down[i] != 0u; ++i) {
			scroll_collisions_down[coll_list_down[i]] = 1u;
		}
	}

	//Change bank now, after copying the collision array (it can be in a different bank)
	PUSH_BANK(map_bank);
	y = scroll_y >> 3;
	for(i = 0u; i != (SCREEN_TILE_REFRES_H) && y != scroll_h; ++i, y ++) {
		ScrollUpdateRow((scroll_x >> 3) - SCREEN_PAD_LEFT,  y - SCREEN_PAD_TOP);
	}
	POP_BANK;
}

void ScrollUpdateRowR() {
	UINT8 i = 0u;
	
	for(i = 0u; i != 5 && pending_w_i != 0; ++i, -- pending_w_i)  {
		#ifdef CGB
		UPDATE_TILE(pending_w_x ++, pending_w_y, pending_w_map ++, pending_w_cmap++);
		#else
		UPDATE_TILE(pending_w_x ++, pending_w_y, pending_w_map ++,0);
		#endif
	}
}

void ScrollUpdateRowWithDelay(INT16 x, INT16 y) {
	while(pending_w_i) {
		ScrollUpdateRowR();
	}

	pending_w_x = x;
	pending_w_y = y;
	pending_w_i = SCREEN_TILE_REFRES_W;
	pending_w_map = scroll_map + scroll_tiles_w * y + x;

	#ifdef CGB
	pending_w_cmap = scroll_cmap + scroll_tiles_w * y + x;
	#endif
}

void ScrollUpdateRow(INT16 x, INT16 y) {
	UINT8 i = 0u;
	unsigned char* map = scroll_map + scroll_tiles_w * y + x;

	#ifdef CGB
	unsigned char* cmap = scroll_cmap + scroll_tiles_w * y + x;
	#endif

	PUSH_BANK(scroll_bank);
	for(i = 0u; i != SCREEN_TILE_REFRES_W; ++i) {
		#ifdef CGB
		UPDATE_TILE(x + i, y, map ++, cmap ++);
		#else
		UPDATE_TILE(x + i, y, map ++, 0);
		#endif
	}
	POP_BANK;
}

void ScrollUpdateColumnR() {
	UINT8 i = 0u;

	for(i = 0u; i != 5 && pending_h_i != 0; ++i, pending_h_i --) {
		#ifdef CGB
		UPDATE_TILE(pending_h_x, pending_h_y ++, pending_h_map, pending_h_cmap);
		pending_h_map += scroll_tiles_w;
		pending_h_cmap += scroll_tiles_w;
		#else
		UPDATE_TILE(pending_h_x, pending_h_y ++, pending_h_map, 0);
		pending_h_map += scroll_tiles_w;
		#endif
	}
}

void ScrollUpdateColumnWithDelay(INT16 x, INT16 y) {
	while(pending_h_i) {
		ScrollUpdateColumnR();
	}

	pending_h_x = x;
	pending_h_y = y;
	pending_h_i = SCREEN_TILE_REFRES_H;
	pending_h_map = scroll_map + scroll_tiles_w * y + x;

	#ifdef CGB
	pending_h_cmap = scroll_cmap + scroll_tiles_w * y + x;
	#endif
}

void ScrollUpdateColumn(INT16 x, INT16 y) {
	UINT8 i = 0u;
	unsigned char* map = &scroll_map[scroll_tiles_w * y + x];
	#ifdef CGB
	unsigned char* cmap = &scroll_cmap[scroll_tiles_w * y + x];
	#endif
	
	PUSH_BANK(scroll_bank);
	for(i = 0u; i != SCREEN_TILE_REFRES_H; ++i) {
		#ifdef CGB
		UPDATE_TILE(x, y + i, map, cmap);
		map += scroll_tiles_w;
		cmap += scroll_tiles_w;
		#else
		UPDATE_TILE(x, y + i, map, 0);
		map += scroll_tiles_w;
		#endif
	}
	POP_BANK;
}

void RefreshScroll() {
	UINT16 ny = scroll_y;
	
	if(scroll_target) {
		if(U_LESS_THAN(scroll_bottom_movement_limit, scroll_target->y - scroll_y)) {
			ny = scroll_target->y - scroll_bottom_movement_limit;
		} else if(U_LESS_THAN(scroll_target->y - scroll_y, scroll_top_movement_limit)) {
			ny = scroll_target->y - scroll_top_movement_limit;
		}
	if (desplazamiento_y_camara < (bocabajo<<5)) desplazamiento_y_camara++;
	if (desplazamiento_y_camara > (bocabajo<<5)) desplazamiento_y_camara--;
		// MoveScroll(scroll_target->x - (SCREENWIDTH >> 1), ny);
		MoveScroll(scroll_target->x + 8 - (SCREENWIDTH >> 1), scroll_target->y + (INT16)(desplazamiento_y_camara) - (SCREENHEIGHT >> 1));  //Centrar el scroll siempre en el personaje
		
	}
}

void MoveScroll(INT16 x, INT16 y) {
	INT16 current_column, new_column, current_row, new_row;
	
	PUSH_BANK(scroll_bank);

	current_column = scroll_x >> 3;
	current_row    = scroll_y >> 3;

	scroll_x = x;
	scroll_y = y;
	ClampScrollLimits();

	new_column     = scroll_x >> 3;
	new_row        = scroll_y >> 3;

	if(current_column != new_column) {
		if(new_column > current_column) {
			ScrollUpdateColumnWithDelay(new_column - SCREEN_PAD_LEFT + SCREEN_TILE_REFRES_W - 1, new_row - SCREEN_PAD_TOP);
		} else {
			ScrollUpdateColumnWithDelay(new_column - SCREEN_PAD_LEFT, new_row - SCREEN_PAD_TOP);
		}
	}
	
	if(current_row != new_row) {
		if(new_row > current_row) {
			ScrollUpdateRowWithDelay(new_column - SCREEN_PAD_LEFT, new_row - SCREEN_PAD_TOP + SCREEN_TILE_REFRES_H - 1);
		} else {
			ScrollUpdateRowWithDelay(new_column - SCREEN_PAD_LEFT, new_row - SCREEN_PAD_TOP);
		}
	}

	if(pending_w_i) {
		ScrollUpdateRowR();
	}
	if(pending_h_i) {
		ScrollUpdateColumnR();
	}
	POP_BANK;
}

UINT8* GetScrollTilePtr(UINT16 x, UINT16 y) {
	//Ensure you have selected scroll_bank before calling this function
	//And it is returning a pointer so don't swap banks after you get the value

	return scroll_map + (scroll_tiles_w * y + x); //TODO: fix this mult!!
}

UINT8 GetScrollTile(UINT16 x, UINT16 y) {
	UINT8 ret;
	PUSH_BANK(scroll_bank);
		ret = *GetScrollTilePtr(x, y);
	POP_BANK;
	return ret;
}

void GetMapSize(UINT8 map_bank, const struct MapInfo* map, UINT8* tiles_w, UINT8* tiles_h)
{
	PUSH_BANK(map_bank);
		if(tiles_w) *tiles_w = map->width;
		if(tiles_h) *tiles_h = map->height;
	POP_BANK;
}

UINT8 ScrollFindTile(UINT8 map_bank, const struct MapInfo* map, UINT8 tile,
	UINT8 start_x, UINT8 start_y, UINT8 w, UINT8 h,
	UINT16* x, UINT16* y) {
	UINT16 xt = 0;
	UINT16 yt = 0;
	UINT8 found = 1;

	PUSH_BANK(map_bank);
	for(xt = start_x; xt != start_x + w; ++ xt) {
		for(yt = start_y; yt != start_y + h; ++ yt) {
			if(map->data[map->width * yt + xt] == (UINT16)tile) { //That cast over there is mandatory and gave me a lot of headaches
				goto done;
			}
		}
	}
	found = 0;

done:
	POP_BANK;
	*x = xt;
	*y = yt;

	return found;
}

