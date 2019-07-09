#include <gb/gb.h>
#include <stdio.h> // for debug only

#include "gametile.h"
#include "game_bkg.h"
#include "bloc.c"

UBYTE spritesize = 8;


struct GameCharacter bloc;

void movegamecharacter(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x + spritesize, y);
    move_sprite(character->spritids[2], x, y + spritesize);
    move_sprite(character->spritids[3], x + spritesize, y + spritesize);
}

void performantdelay(UINT8 numloops){         // usage = performantdelay(128);
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}

void setupbloc(){
    bloc.x = 64;
    bloc.y = 16;
    bloc.width = 16;
    bloc.height = 16;

    // load sprites for bloc
    set_sprite_tile(0, 4);
    bloc.spritids[0] = 0;
    set_sprite_tile(1, 5);
    bloc.spritids[1] = 1;
    set_sprite_tile(2, 6);
    bloc.spritids[2] = 2;
    set_sprite_tile(3, 7);
    bloc.spritids[3] = 3;

    movegamecharacter(&bloc, bloc.x, bloc.y);
}



void main(){
    /* Sound */
    
    // these registers must be in this specific order!
    NR52_REG = 0x8F;  // SOUND ON
    NR51_REG = 0xff;  // Channel usage (1111 1111 = all)
    NR50_REG = 0x20;  // Volume (MAX = 0x77, min = 0x00)

    /* Background */

    set_bkg_data(0, 127, gametile);
    set_bkg_tiles(0, 0, 20, 18, game_bkg);
    SHOW_BKG;
    DISPLAY_ON;
    
    /* Sprite */

    set_sprite_data(0, 127, gametile);
    setupbloc();

       SHOW_SPRITES;    

    /* Movements */

    UBYTE keydown = 0;
    UBYTE keyrepeat = 0;
    UINT8 keys;
    UBYTE bottom = 0;
    UINT8 frame_skip = 0;
    UINT8 difficulty = 20; // 20=Easy 10=Normal 2=Hard
      
    while (bottom == 0){
        keys = joypad();

        if(!keydown){
            switch(keys){
                case J_LEFT :
                    bloc.x -= 8;
                    break;
                case J_RIGHT :
                    bloc.x += 8;
                    break;
                case J_START :
                    NR10_REG = 0x16;
                    NR11_REG = 0x40;
                    NR12_REG = 0x73;
                    NR13_REG = 0x00;
                    NR14_REG = 0xC3;
                    delay(512);
                    waitpad(J_START);
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
        movegamecharacter(&bloc,bloc.x,bloc.y);

        wait_vbl_done();

        if(bloc.y >= 144){
        bottom = 1;
        }
    }
}