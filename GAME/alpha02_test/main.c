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
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels

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

    UINT8 prev_keys = 0;
    UINT8 keys;
    UBYTE bottom = 0;
    UINT8 frame_skip = 0;

    
    while (bottom == 0){
        keys = joypad();

        if (frame_skip == 0) {
            if(keys & J_LEFT && !(prev_keys & J_LEFT)){
                bloc.x -= 8;
            }
               
            if(keys & J_RIGHT && !(prev_keys & J_RIGHT)){
                bloc.x += 8;
            }
            bloc.y += 8;
            frame_skip = 16;
        }
        else{
            frame_skip -= 1;
        }
        prev_keys = keys;    
        movegamecharacter(&bloc,bloc.x,bloc.y);

        wait_vbl_done();

        if(bloc.y >= 144){
        bottom = 1;
        }
    }
}