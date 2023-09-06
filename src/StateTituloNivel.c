#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Music.h"
#include <Palette.h>

IMPORT_MAP(titulo_cementerio);
IMPORT_MAP(titulo_pueblo);
IMPORT_MAP(titulo_selva);
IMPORT_MAP(entra_piramide);
DECLARE_MUSIC(level_start);

extern UINT16 tiempo;
UINT8 parpadeo;
extern unsigned char level, mundo_actual, vidas;
void pausa(unsigned int time) BANKED;
unsigned char tile_num_1 [] = {97, 182, 116, 0}; //Tile donde empiezan los números
unsigned char tile_negro [] = {0, 177, 55, 0}; //Tile donde empiezan los números
unsigned char pos_y_titulo [] = {41, 30, 41, 8}; //Coordenada Y del sprite para el título del nivel
	
void print_texts(){
	UINT8 tile_n;
	tile_n = tile_num_1[mundo_actual];
	if (parpadeo == 0) {
		set_bkg_tile_xy(6,17, (mundo_actual + tile_n));
		set_bkg_tile_xy(7,17, (tile_n - 4));
		set_bkg_tile_xy(8,17, (level + tile_n));
		set_bkg_tile_xy(19,17, (vidas + tile_n - 1));
	}
	
	tile_n = tile_negro[mundo_actual];
	if (parpadeo == 15) {
		 set_bkg_tile_xy(6,17, (tile_n));
		 set_bkg_tile_xy(7,17, (tile_n));
         set_bkg_tile_xy(8,17, (tile_n));
	}
	parpadeo ++;
	
	if (parpadeo == 31) parpadeo = 0;

}



void START() {
	
	
	
	//Change Sprite Palettes
	OBP0_REG = PAL_DEF(0,0,3,2); //Cambia la paleta 0
	
	switch (mundo_actual){
		
		
				
		case 0:  
		InitScroll(BANK(titulo_cementerio), &titulo_cementerio, 0, 0);
		break;
		
		case 1: 
		InitScroll(BANK(titulo_pueblo), &titulo_pueblo, 0, 0);
		break;
		
		case 2: 
		InitScroll(BANK(titulo_selva), &titulo_selva, 0, 0);
		break;
	
		case 3: 
		InitScroll(BANK(entra_piramide), &entra_piramide, 0, 0);
		break;	
	
	}


	PlayMusic(level_start, 0);
	
	
	SpriteManagerAdd(SpriteTitulo, 0, pos_y_titulo[mundo_actual]);
	tiempo = 240;
	parpadeo = 0;
	
	

}





void UPDATE() {

	
	if (mundo_actual < 3) print_texts();
	
	
	tiempo --;
		
	if (tiempo == 0) {
		if (mundo_actual < 3)
			SetState(StateGame);
		else{ 
			StopMusic;
			pausa(230);
			SetState(StateCredits);
		}
	}
}