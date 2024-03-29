#include <gb/gb.h>
#include <gb/rand.h> // Random generator
#include <stdio.h> // for debug only
#include <gb/console.h>


#include "gametile.h"
#include "game_bkg.h"
#include "bloc.c"

const UINT8 spritesize = 8;
const char blankmap[1] = {0x00};
const char bloctile[1] = {0x01};
UINT8 debug = 0; // for debug only
UINT8 difficulty = 15; // 20=Easy 10=Normal 2=Hard


const UINT8 MAX = 2, MIN = 1; // Not used yet, Number of different blocs for rand function

struct GameCharacter bloc; // current bloc
struct GameCharacter next; // next bloc
struct GameCharacter bar; // bar (test for now)

/* Collision detection */

UBYTE canplayermove(UINT8 newplayerx, UINT8 newplayery, UINT8 newplayerright){ // RIGHT / LEFT movements
    UINT16 tileindexTL;
    UINT8 indexTLy, indexTLx, result;

    if(newplayerright){
        indexTLx = (newplayerx) / 8;
    }
    else{
        indexTLx = (newplayerx - 8) / 8;
    }
    indexTLy = (newplayery) / 8;
    tileindexTL = 20 * indexTLy + indexTLx;
  
    /* DEBUG */
    // printf("%u %u\n",(UINT16)(newplayerx),(UINT16)(newplayery));
    // printf("%u %u %u\n\n",(UINT16)indexTLx,(UINT16)indexTLy,(UINT16)tileindexTL);   
    
    result = game_bkg[tileindexTL] == blankmap[0];
    return result;
}

UBYTE canplayermove_down(UINT8 newplayerx, UINT8 newplayery){ // BOTTOM behavior
    UINT16 tileindexTL;
    UINT8 indexTLy, indexTLx, result;
    UBYTE position;

    indexTLx = (newplayerx) / 8;
    indexTLy = (newplayery) / 8;
    tileindexTL = 20 * indexTLy + indexTLx;

    /* DEBUG */
    // printf("%u %u\n",(UINT16)(newplayerx),(UINT16)(newplayery)); printf("%u %u %u\n\n",(UINT16)indexTLx,(UINT16)indexTLy,(UINT16)tileindexTL);   
    
    result = game_bkg[tileindexTL] == blankmap[0];
    
    // 24 128 | 32 128
    // 24 136 | 32 136
    if(tileindexTL >= 343 && tileindexTL <= 351 && result == 1){ // tileindexTL >= 343 && tileindexTL <= 349
        // add blocks to backgrounds
        delay(128);
        position=tileindexTL-343;
        set_bkg_tiles(position+2,16,1,1,bloctile); // x y 1 sprite right 1 sprite down
        set_bkg_tiles(position+2,17,1,1,bloctile); // making 4 of these otherwise doesn't works well
        set_bkg_tiles(position+3,16,1,1,bloctile);
        set_bkg_tiles(position+3,17,1,1,bloctile);
        result = 0;
    }
    /* 
    if(tileindexTL >= 323 && tileindexTL <= 332 && result == 0){ // tileindexTL >= 343 && tileindexTL <= 349
        // add blocks to backgrounds
        delay(128);
        position=tileindexTL-323;
        set_bkg_tiles(position+2,14,1,1,bloctile); // x y 1 sprite right 1 sprite down
        set_bkg_tiles(position+2,15,1,1,bloctile); // making 4 of these otherwise doesn't works well
        set_bkg_tiles(position+3,14,1,1,bloctile);
        set_bkg_tiles(position+3,15,1,1,bloctile);
        result = 0;
    }*/



    return result;
}

void movethebloc(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x + spritesize, y);
    move_sprite(character->spritids[2], x, y + spritesize);
    move_sprite(character->spritids[3], x + spritesize, y + spritesize);
}

/* Not used Yet */

void movethebar(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x, y + 8);
    move_sprite(character->spritids[2], x, y + 16);
    move_sprite(character->spritids[3], x, y + 24);
}

/* Not used yet */

void setupbar(){
    bloc.x = 64;
    bloc.y = 16;
    bloc.width = 8;
    bloc.height = 32;

    // load sprites for bar
    set_sprite_tile(0, 4); // sprite object 0 using tile 4   (Max Sprite = 40 / 10 by scanline)
    bloc.spritids[0] = 0;  // 1rst square of sprite uses sprite object 0
    set_sprite_tile(1, 5);
    bloc.spritids[1] = 1;
    set_sprite_tile(2, 6);
    bloc.spritids[2] = 2;
    set_sprite_tile(3, 7);
    bloc.spritids[3] = 3;

    movethebar(&bloc, bloc.x, bloc.y);
}

/* Current bloc */

void setupbloc(){
    bloc.x = 24; // added 8 - "O" is the main tile of the sprite
    bloc.y = 16; // default = 8 or 16
    bloc.width = 16;
    bloc.height = 16;

    // load sprites for bloc
    set_sprite_tile(0, 4); // sprite object 0 using tile 4   (Max Sprite = 40 / 10 by scanline)
    bloc.spritids[0] = 0;  // 1rst square of sprite uses sprite object 0
    set_sprite_tile(1, 5);
    bloc.spritids[1] = 1;
    set_sprite_tile(2, 6);
    bloc.spritids[2] = 2;
    set_sprite_tile(3, 7);
    bloc.spritids[3] = 3;

    movethebloc(&bloc, bloc.x, bloc.y);
}

/* Next Bloc */

void setupnext(){
    next.x = 136;
    next.y = 64;
    next.width = 16;
    next.height = 16;    

    // load sprites for next bloc
    set_sprite_tile(4, 4);
    next.spritids[0] = 4;
    set_sprite_tile(5, 5);
    next.spritids[1] = 5;
    set_sprite_tile(6, 6);
    next.spritids[2] = 6;
    set_sprite_tile(7, 7);
    next.spritids[3] = 7;

    movethebloc(&next, next.x, next.y);
}


/* Function about Sounds */

void sound_initialisation(){
    NR52_REG = 0x8F;  // SOUND ON
    NR51_REG = 0xff;  // Channel usage (1111 1111 = all)
    NR50_REG = 0x20;  // Volume (MAX = 0x77, min = 0x00)
}

void sound_pause(){
    NR41_REG = 0x00;
    NR42_REG = 0xE1;
    NR43_REG = 0x22;
    NR44_REG = 0xC3;
    NR51_REG = 0x88U;
}

void sound_blocmove(){
    NR10_REG = 0x04U;
    NR11_REG = 0xFEU;
    NR12_REG = 0xA1U;
    NR13_REG = 0x8FU;
    NR14_REG = 0x86U;
    NR51_REG = 0xF7;
}

void sound_blocdown(){
    NR10_REG = 0x03U;
    NR11_REG = 0xFDU;
    NR12_REG = 0xA0U;
    NR13_REG = 0x8EU;
    NR14_REG = 0x87U;
    NR51_REG = 0xE6;
}

void sound_rotate(){
    NR10_REG = 0x73U;
    NR11_REG = 0xADU;
    NR12_REG = 0xA7U;
    NR13_REG = 0x8AU;
    NR14_REG = 0x86U;
    NR51_REG = 0xD5;
}

/* Performant Delay */

void performantdelay(UINT8 numloops){         // usage = performantdelay(128);
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}




void main(){
    /* Sound Initialisation */

    sound_initialisation();

    /* Background Initialisation */

    set_bkg_data(0, 127, gametile);
    set_bkg_tiles(0, 0, 20, 19, game_bkg);
    SHOW_BKG;
    
    
    /* Sprite */

    set_sprite_data(0, 127, gametile);
    
    SHOW_SPRITES;
    DISPLAY_ON;   
    
    /* Movements */

    UBYTE bottom = 0;
    UBYTE gameover = 0;
    UBYTE keydown = 0;
    UBYTE keyrepeat = 0;
    UBYTE keys;
    UBYTE select = 0;
    UBYTE frame_skip = 0;
    UBYTE newbloc = 0; // useless
       
    
    setupbloc();    
    setupnext();
    bottom == 0;

    while (1){   
        keys = joypad();
        
        
        if(!keydown){
            switch(keys){
                case J_A :
                    // rotateblocright();
                    sound_rotate();
                    gotoxy(4, 5);

                    break;
                case J_B :
                    // rotateblocleft();
                    sound_rotate();
                    break;
                case J_LEFT :
                    if(canplayermove(bloc.x-8,bloc.y,0)){
                        bloc.x -= 8;
                        sound_blocmove();
                    }
                    break;
                case J_RIGHT :
                    if(canplayermove(bloc.x+8,bloc.y,1)){
                        bloc.x += 8;
                        sound_blocmove();
                    }
                    break;
                case J_UP :
                    bloc.y += 144 - bloc.y;
                    sound_blocmove();
                    break;
                case J_DOWN :
                    if(canplayermove_down(bloc.x,bloc.y-8)){
                        bloc.y += 8;
                        sound_blocmove();
                    }
                    break;
                case J_START :
                    sound_pause();
                    delay(512);
                    waitpad(J_START);
                    sound_pause();
                    break;
                case J_SELECT : // Add / remove Next Bloc display !! Doesn't work on pause.
                    if(!select){
                        movethebloc(&next, 144, 160); // moving next bloc to invisible
                        select = 1;
                        }
                    else{
                        movethebloc(&next, 136, 64); // replacing next bloc to visible
                        select = 0;
                        }
                    break;
            }
        }
        wait_vbl_done();

        if (frame_skip == 0) {
            
            frame_skip = difficulty;
            if(canplayermove_down(bloc.x,bloc.y-8)){
                    bloc.y += 8;
            }
                else{
                    sound_blocdown();
                    setupbloc();
                    }
        }
        else{
            frame_skip -= 1;
        }
        
        keydown = keys;

        movethebloc(&bloc,bloc.x,bloc.y);
        // movethebar(&bloc,bloc.x,bloc.y);
        

        
    }
}