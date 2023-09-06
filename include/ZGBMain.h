#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateMenu)\
_STATE(StateTituloNivel)\
_STATE(StateGameOver)\
_STATE(StateGame)\
_STATE(StateIntro)\
_STATE(StateCredits)\
STATE_DEF_END

/*
1 - Parca
2 - Murcielago
3 - Mano
4 - Zombi
5 - Llave
6 - Calabaza
7 - Cojon
8 - Flecha IN
9 - Pajaro
10 - Topo

*/


#define SPRITES \
_SPRITE_DMG(SpriteBombaItem, bomba_item)\
_SPRITE_DMG(SpriteRompible, rompibles)\
_SPRITE_DMG(SpriteEnemigo1, parca)\
_SPRITE_DMG(SpriteEnemigo2, murcielago)\
_SPRITE_DMG(SpriteEnemigo3, mano)\
_SPRITE_DMG(SpriteZombie, zombie)\
_SPRITE_DMG(SpriteLlave, llave)\
_SPRITE_DMG(SpriteCalabaza, calabaza)\
_SPRITE_DMG(SpriteCojon, bola)\
_SPRITE_DMG(SpriteFlechaIn, flecha_in)\
_SPRITE_DMG(SpritePajaro, pajarraco)\
_SPRITE_DMG(SpriteTopo, topo)\
_SPRITE_DMG(SpritePlataforma, platform)\
_SPRITE_DMG(Sprite1up, item_1up)\
_SPRITE_DMG(SpriteAntorcha, antorcha)\
_SPRITE_DMG(SpriteCorazonItem, corazon_item)\
_SPRITE_DMG(SpritePlayer, calavera)\
_SPRITE_DMG(SpriteCraneo, craneo)\
_SPRITE_DMG(SpriteBomba, bomba)\
_SPRITE_DMG(SpriteEnemyProyectile, enemy_bullet)\
_SPRITE_DMG(SpritePress, press)\
_SPRITE_DMG(SpriteOjillos, ojos)\
_SPRITE_DMG(SpriteSoul, soul)\
_SPRITE_DMG(SpriteTitulo, titulos)\
_SPRITE_DMG(SpriteAutoScroll, enemy_bullet)\
_SPRITE_DMG(SpriteAutoScrollCredits, enemy_bullet)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif


#define MAX_TILE_SOLIDO	32
#define MAX_TILE_TRASPASABLE 39
