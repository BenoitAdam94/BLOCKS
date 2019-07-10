#include <gb/gb.h>
#include <gb/rand.h> // Random generator
#include <stdio.h> // for debug only


#include "gametile.h"
#include "game_bkg.h"
#include "bloc.c"

const UBYTE spritesize = 8;
UBYTE debug = 1; // for debug only

UINT8 playerlocation[2];

const UINT8 MAX = 2, MIN = 1; // Number of different blocs

struct GameCharacter bloc;
struct GameCharacter next;
struct GameCharacter bar;


void movethebloc(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x + spritesize, y);
    move_sprite(character->spritids[2], x, y + spritesize);
    move_sprite(character->spritids[3], x + spritesize, y + spritesize);
}

void movethebar(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x, y + 8);
    move_sprite(character->spritids[2], x, y + 16);
    move_sprite(character->spritids[3], x, y + 24);
}

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

void setupbloc(){
    bloc.x = 64;
    bloc.y = 16;
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



void performantdelay(UINT8 numloops){         // usage = performantdelay(128);
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}





void main(){
    /* Sound */
    
    NR52_REG = 0x8F;  // SOUND ON
    NR51_REG = 0xff;  // Channel usage (1111 1111 = all)
    NR50_REG = 0x20;  // Volume (MAX = 0x77, min = 0x00)

    /* Background */

    set_bkg_data(0, 127, gametile);
    set_bkg_tiles(0, 0, 20, 18, game_bkg);
    SHOW_BKG;
    
    
    /* Sprite */

    set_sprite_data(0, 127, gametile);
    
    SHOW_SPRITES;
    DISPLAY_ON;   
    
    /* Movements */

    UBYTE bottom = 0; // Technicaly this is gameover
    UBYTE gameover = 0;
    UBYTE keydown = 0;
    UBYTE keyrepeat = 0;
    UINT8 keys;
    UBYTE select = 0;
    UINT8 frame_skip = 0;
    UINT8 difficulty = 20; // 20=Easy 10=Normal 2=Hard   
    
    setupbar();    
    setupnext();
    bottom == 0;

    while (1){   
        keys = joypad();
        
        if(!keydown){
            switch(keys){
                case J_A :
                    // rotatebloc();
                case J_LEFT :
                    bloc.x -= 8;
                    sound_blocmove();
                    break;
                case J_RIGHT :
                    bloc.x += 8;
                    sound_blocmove();
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


        if (frame_skip == 0) {
            bloc.y += 8;
            frame_skip = difficulty;
        }
        else{
            frame_skip -= 1;
        }
        

        keydown = keys;

        movethebloc(&bloc,bloc.x,bloc.y);
        movethebar(&bloc,bloc.x,bloc.y);

        wait_vbl_done();

        if(bloc.y >= 144){
        setupbloc();
        }
        
    }
}