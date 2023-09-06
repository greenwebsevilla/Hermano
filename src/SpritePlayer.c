#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "Keys.h"
#include "SpriteManager.h"

#include "Print.h"
#include "Sound.h"
#include "Music.h"
#include "Sprite.h"
#include "Scroll.h"

DECLARE_MUSIC(muerte);
DECLARE_MUSIC(stageclear);


void CreateBomb(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED;
void CreateCraneo(UINT16 x, INT16 y, INT16 p_facing_, UINT8 offset_y_) BANKED;

#define PLAYER_ENERGY_MAX 5
#define MAX_BOMBS 5

#define X_SPEED_MAX	88
#define X_SPEED_INC	12
#define X_FRICTION	4
#define GRAVITY	10
#define Y_SPEED_MAX	220
#define JUMP_INC	220

//Player variables
signed int aux1,aux2;
signed int cx1,cx2,cy1,cy2;
unsigned char pt1, pt2;
UINT32 p_x;
UINT32 p_y;
signed int p_vx, p_vy, platform_vy;
signed char gravity;
signed int jump_inc;
signed char p_facing;
unsigned char last_mirror_player, pausado;


//Animations
const UINT8 anim_player_walk[] = {4, 0, 1, 2, 1, 0};
const UINT8 anim_player_jump[] = {1, 3};
const UINT8 anim_player_quietor[] = {12, 1,1,1,1,1,1,1,1,8,1,1,1};
const UINT8 anim_player_tocado[] = {2, 4, 5};
const UINT8 anim_player_muerto[] = {1, 4};
const UINT8 anim_player_muerto2[] = {1, 7};
const UINT8 anim_player_tocado2[] = {2, 7, 5}; // THIS->tocado cuando ha disparado la cabeza
const UINT8 anim_player_disparando[] = {1, 6};

// DATOS NIVELES
extern unsigned char level, mundo_actual, vidas, has_key, bombas_disparadas, vidaextra;
extern unsigned char half_life, frame_counter, disparando, bombas, autoscroll;
extern signed char energy;
extern unsigned int x_player;
extern signed int y_player;
extern unsigned char bocabajo;
extern unsigned int y_eje_actual;
extern unsigned int x_checkpoint, y_checkpoint;

//Funciones extra

void UpdateHudLife() {
	for (INT8 i = 0; i < PLAYER_ENERGY_MAX; ++i)
		UPDATE_HUD_TILE(0 + i, 0, i < energy ? 30 : 29);
	
	if (has_key > 0) UPDATE_HUD_TILE(10, 0, 27);
}

void UpdateHudBombs() {
	for (UINT8 i = 0; i < MAX_BOMBS; ++i)
		UPDATE_HUD_TILE(19 - i, 0, i < bombas ? 31 : 28);
}

void check_2_points() {
	pt1 = GetScrollTile(cx1>>3, cy1>>3);
	pt2 = GetScrollTile(cx2>>3, cy2>>3);
}

void pausa(unsigned int time) BANKED{
	
	for (UINT8 j = 0; j < time; j++){
			
			__asm
			halt
			halt
			halt
			halt
			__endasm;

		}
		
}

void loss_energy(){
	
	PlayFx(CHANNEL_1, 8, 0x3b, 0x48, 0xb2, 0x17, 0x84);
	energy--;
	UpdateHudLife();
	
	if (energy < 1) {
		THIS->tocado = 1;
		vidas --;
		pausa(30);
		PlayMusic(muerte, 0);
		THIS->estado = 4;
		scroll_target = 0; //Quito el scroll al player
		p_vy = jump_inc; //rebote al morir
		THIS->contador_tiempo = 180;
	}else {
	
		THIS->tocado = 70;
		THIS->estado = 3;
		
	}
}

void rebote_tocado(){
	//rebote al recibir golpe
	p_vy = jump_inc>>1; 
	p_vx = -p_vx;
}

void animate_player(){
	
	switch (THIS->estado) {
		
		case 0:
		
			if (THIS->posee){
				//Walk / Idle
				if (p_vx != 0)
					SetSpriteAnim(THIS, anim_player_walk, 12u);
				else
					SetSpriteAnim(THIS, anim_player_quietor, 12);
				
			}else{
				//Jump
				SetSpriteAnim(THIS, anim_player_jump, 2);
			}
			
			break;
		
		case 1:
		case 3:
		
			if (THIS->tocado){
				//Parpadeando THIS->tocado
				if (disparando)
					SetSpriteAnim(THIS, anim_player_tocado2, 30u);
				else
					SetSpriteAnim(THIS, anim_player_tocado, 30u);
			
			}else{
				SetSpriteAnim(THIS, anim_player_disparando, 0);
			}	
			break;
			
		case 4: //Muerte
			if (disparando)
					SetSpriteAnim(THIS, anim_player_muerto2, 30u);
				else
			SetSpriteAnim(THIS, anim_player_muerto, 30u);
		break;
	
	}
}

void check_sprites_collisions() {
	
	UINT8 i;
	Sprite* spr;
	platform_vy = 0;
	SPRITEMANAGER_ITERATE(i, spr) {
		if (spr->estado < 99){
		// if (THIS->estado < 4){
			if(CheckCollision(THIS, spr)) {
				
				switch(spr->type){ 
				
					case SpriteBombaItem:
						PlayFx(CHANNEL_1, 8, 0x15, 0x8a, 0xff, 0x43, 0x85);
						SpriteManagerRemove(i);
						if (bombas < MAX_BOMBS) bombas ++;
						UpdateHudBombs();					
					break;
					
					case SpriteCorazonItem:
						PlayFx(CHANNEL_1, 8, 0x15, 0x8a, 0xff, 0x43, 0x85);
						SpriteManagerRemove(i);
						if (energy < PLAYER_ENERGY_MAX) energy ++;
						UpdateHudLife();					
					break;
					
					case SpriteLlave:
						PlayFx(CHANNEL_1, 8, 0x17, 0x4b, 0xda, 0x08, 0x87);
						SpriteManagerRemove(i);
						has_key = 1;
						UpdateHudLife();
					break;
					
					case SpriteRompible:
					case SpriteAntorcha:
						if (THIS->estado < 4){
							rebote_tocado();
						}
					case SpriteEnemyProyectile:
						if (THIS->tocado == 0){
							loss_energy();
						}
						spr->estado = 2;
					break;
					
					case SpriteEnemigo1:
					case SpriteEnemigo2:
					case SpriteEnemigo3:
					case SpriteZombie:
					case SpriteCalabaza:
					case SpriteCojon:
					case SpritePajaro:
					case SpriteTopo:
						if (THIS->estado < 2){ //estados 0 y 1
							rebote_tocado();
							loss_energy();
						}
					break;
					
					case SpritePlataforma:
			
							if (THIS->y >= spr->y - 25 && THIS->y < spr->y - 16){
								platform_vy = spr->vy;
								THIS->y = spr->y - 21;
								if (platform_vy < 0) THIS->y --;
								p_y = (UINT32)(THIS->y) << 6;
							}
											
					break;
					
					
					case SpriteCraneo:
			
							if (THIS->estado == 4){
								THIS->tocado = 0;
							}
											
					break;
					
					
				}
				
				
			}
		// }
		}
		
		
	}
	
	if (autoscroll) {
		if (THIS->y > scroll_y+160) {
			energy = 1;
			loss_energy();
		}
	}
	
}

void add_friction(){
	
	if (p_vx > 0) 
		p_vx -= X_FRICTION;
	else if (p_vx < 0)
		p_vx += X_FRICTION;
	
}

void add_gravity(){
	
	//gravity
	p_vy += gravity;
	
	//vertical speed limits
	if (p_vy > Y_SPEED_MAX) p_vy = Y_SPEED_MAX;
	if (p_vy < -Y_SPEED_MAX) p_vy = -Y_SPEED_MAX;
	
}


void check_gravity_axis(){
	
	if ((INT16)THIS->y > (INT16)y_eje_actual) { 
		gravity = -GRAVITY;
		jump_inc = JUMP_INC;
		bocabajo = 1;
	}else{
		gravity = GRAVITY;
		jump_inc = -JUMP_INC;
		bocabajo = 0;
	}	
	
}

//Funciones basicas

void START() {
	
	check_gravity_axis();
	
	THIS->posee = 1;
	p_vy = p_vx = 0;
	bombas = 0;
	p_facing = 1;
	last_mirror_player = 0;
	
	THIS->attr_add |= S_PALETTE;
	
	p_x = (UINT32)(THIS->x) << 6;
	p_y = (UINT32)(THIS->y) << 6;
	
	THIS->lim_y = 255;
	
	energy = PLAYER_ENERGY_MAX;
	
	THIS->estado = 0;
	THIS->tocado = 0;
	
	UpdateHudLife();
}

void UPDATE() {
	
	half_life = !half_life;
	frame_counter ++;
	
	if (THIS->estado < 4){ //TODO EL CONTROL Y MOVIMIENTOS SE ANULAN EN ESTADO 4 (MUERTE)

			//VERTICAL MOVEMENT 
			
			add_gravity();
			
			//Player Jump
			if (THIS->posee) {
				if (disparando == 0){
						if(KEY_TICKED(J_A)) {
						p_vy = jump_inc; 
						platform_vy = 0;
						PlayFx(CHANNEL_1, 5, 0x27, 0x82, 0x9f, 0x0b, 0xc6);
					}
				}
				if (has_key == 1){
					has_key ++;
					x_checkpoint = THIS->x;
					y_checkpoint = THIS->y;
				}
			}

			// if (platform_vy) p_vy = platform_vy;
			p_y += p_vy;
			THIS->y = (p_y >> 6);

			THIS->posee = 0;
			
			if (platform_vy != 0){
				THIS->posee = 1;
			}
			
			if (p_vy > 0){
				cx1 = THIS->x+2 ; cy1 = THIS->y + 24;
				cx2 = THIS->x+13 ; cy2 = cy1;
				check_2_points();	
				if (pt1 < MAX_TILE_TRASPASABLE || pt2 < MAX_TILE_TRASPASABLE) {
					if (bocabajo == 0){
						THIS->posee = 1;
						THIS->y &= 0xFFF8;
						p_y = (UINT32)(THIS->y) << 6;
					}
					p_vy = 0;
				}else{
					if(KEY_PRESSED(J_DOWN)){ //Aceleración al caer
						p_vy += 2;
						if(KEY_TICKED(J_DOWN)) PlayFx(CHANNEL_1, 5, 0x27, 0x0b, 0xc2, 0x73, 0x86);
					}
				}
			}
			
			
			if (p_vy < 0){
				cx1 = THIS->x+2; cy1 = THIS->y;
				cx2 = THIS->x+13; cy2 = cy1;
				check_2_points();
				if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					if (bocabajo == 1){
						THIS->posee = 1;
						THIS->y += 4;
						THIS->y &= 0xFFF8;
						p_y = (UINT32)(THIS->y) << 6;
					}
					p_vy = 0;
				}
				
			}

	
			//HORIZONTAL MOVEMENT 
			
			
			//Player Control
			if(disparando == 0){
				if(KEY_PRESSED(J_RIGHT)){
					
					p_facing = 1;
					p_vx += X_SPEED_INC;
					if (p_vx > X_SPEED_MAX) p_vx = X_SPEED_MAX;
					last_mirror_player = NO_MIRROR;
					
				}else if(KEY_PRESSED(J_LEFT)){
					
					p_facing = -1;
					p_vx -= X_SPEED_INC;
					if (p_vx < -X_SPEED_MAX) p_vx = -X_SPEED_MAX;
					last_mirror_player = V_MIRROR;
					
				}else{

					add_friction();

				}
			}else{

				add_friction();

			}
			
			THIS->mirror = last_mirror_player + bocabajo;
			
			//Horizontal collisions
			if (p_vx < 0){
				cx1 = THIS->x-1; cy1 = THIS->y + 2;
				cx2 = cx1; cy2 = THIS->y + 22;
				check_2_points();
				if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					p_vx = 0;
				}
			}
			
			if (p_vx > 0){
				cx1 = THIS->x+17; cy1 = THIS->y + 2;
				cx2 = cx1; cy2 = THIS->y + 22;
				check_2_points();
				if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					p_vx = 0;
				}
			}
			
			

			p_x += p_vx;
			
			
			//Limite izquierda
			if (p_x < (UINT32)(scroll_x + 4)<<6){
				p_x = (UINT32)(scroll_x + 4)<<6;
				p_vx = 0;
				
				// En la fase de auto-scroll comprueba si te estruja un bloque sólido.
				if (autoscroll){
				
					cx1 = THIS->x+10; cy1 = THIS->y;
					cx2 = THIS->x+10; cy2 = THIS->y + 16;
					check_2_points();
					if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					
						energy = 1;
						loss_energy();
						THIS->estado = 4;
					}
				}
			}
			
			//Limite derecha
			if (p_x > (UINT32)(scroll_x + 140)<<6){
				p_x = (UINT32)(scroll_x + 140)<<6;
				p_vx = 0;
			}
			
			THIS->x = (UINT16)(p_x >> 6);
			
			
			

			
			
			check_gravity_axis();
			
			if(KEY_PRESSED(J_UP) && KEY_TICKED(J_B)) {
			//Drop bombs
			if (bombas){
				if (bombas_disparadas == 0){
					
						bombas--;
						bombas_disparadas++;
						UpdateHudBombs();
						CreateBomb(THIS->x + 4, THIS->y + (bocabajo<<4), p_facing<<1, (jump_inc>>1), gravity);
					}
				}
			}else
			
			//Lanzar craneo
			if (THIS->estado != 3){
					
					if(KEY_TICKED(J_B) && THIS->estado == 0) {
						// p_vx = 0;
						THIS->estado = 1; //estado Disparando cabezas
						disparando = 1;
						CreateCraneo(THIS->x , THIS->y-2, p_facing, bocabajo<<3);
					}
					
			}


			if (THIS->tocado){
				if (THIS->estado < 4){
					THIS->tocado --;
					if (THIS->tocado == 0) THIS->estado = 0;
					
				}
			}
			
			//Detect EXIT - puerta
			if (has_key > 0){
				
				cx1 = THIS->x ; cy1 = THIS->y ;
				cx2 = THIS->x+8 ; cy2 = THIS->y ;
				check_2_points();
				if (pt1 == 39 || pt2 == 39) { 
					if(KEY_TICKED(J_UP)) { 
						has_key = 0;
						level ++;
						x_checkpoint = 0;
						y_checkpoint = 0;
						if (level == 2) {
							level = 0;
							vidaextra = 0;
							mundo_actual ++;
							// IR A FINAL SI PASAMOS TODOS LOS MUNDOS
							
							SetState(StateTituloNivel); 
						}else SetState(StateGame); 
						
						
						PlayMusic(stageclear, 0);
						pausa(240);
						WY_REG = 144;
						SHOW_WIN;
					}
				
				}
					
			}
			
			
			
			if (half_life == 0 || (autoscroll)) check_sprites_collisions();


	}else{ //IF ESTADO == 4
	
			add_gravity();

			p_y += p_vy;
			THIS->y = (p_y >> 6);
			
			
			THIS->tocado = 0;
	
	}
	
	//ANIMACIONES PLAYER
	animate_player();
	x_player = THIS->x;
	y_player = THIS->y;
	
	

	DPRINT_POS(0, 0);
	DPrintf("x:%d y:%d  ", p_vy, pt2);
	
	
	if(KEY_TICKED(J_START)) {
		PlayFx(CHANNEL_1, 8, 0x74, 0x0a, 0xf7, 0x72, 0x86);
		pausa(50);
		waitpad(J_START);
		PlayFx(CHANNEL_1, 8, 0x74, 0x0a, 0xf7, 0x72, 0x86);
	}
	
}

void DESTROY() {
	
	WY_REG = 144;
	SHOW_WIN;
	if (vidas > 0){
		SetState(StateTituloNivel);
	}else{
		SetState(StateGameOver);
	}
	
}


