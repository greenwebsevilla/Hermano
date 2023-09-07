#ifndef MAIN_H
#define MAIN_H

#include <gb/gb.h>

#include "Sprite.h"
#include "TilesInfo.h"

typedef void (*Void_Func_Void)();
typedef void (*Void_Func_SpritePtr)(Sprite*);

#define DECLARE_STATE(STATE_IDX)   extern UINT8 bank_##STATE_IDX;  void Start_##STATE_IDX(); void Update_##STATE_IDX()
extern UINT8 stateBanks[];
extern Void_Func_Void startFuncs[];
extern Void_Func_Void updateFuncs[];

#define DECLARE_SPRITE(SPRITE_IDX) extern UINT8 bank_##SPRITE_IDX; void Start_##SPRITE_IDX(); void Update_##SPRITE_IDX(); void Destroy_##SPRITE_IDX()
extern UINT8 spriteBanks[];
extern Void_Func_Void spriteStartFuncs[];
extern Void_Func_Void spriteUpdateFuncs[];
extern Void_Func_Void spriteDestroyFuncs[];
extern const struct MetaSpriteInfo* spriteDatas[];
extern UINT8 spriteDataBanks[];
extern UINT8 spriteIdxs[];
extern UINT8 spritePalsOffset[];


extern UINT8 current_state;
void SetState(UINT8 state);
extern UINT8 delta_time;

void SetWindowY(UINT8 y);

void LCD_isr() NONBANKED;

#ifdef CGB
typedef enum {
	BG_PALETTE,
	SPRITES_PALETTE
} PALETTE_TYPE;
void SetPalette(PALETTE_TYPE t, UINT8 first_palette, UINT8 nb_palettes, UINT16 *rgb_data, UINT8 bank);
#else
#define SetPalette(PALETTE_TYPE, first_palette, nb_palettes, rgb_data, bank);
#endif

#define IMPORT_MAP(MAP) extern struct MapInfo MAP; extern const void __bank_##MAP
#define IMPORT_TILES(TILES) extern struct TilesInfo TILES; extern const void __bank_##TILES
#define IMPORT_FONT(FONT) IMPORT_TILES(FONT)

#endif