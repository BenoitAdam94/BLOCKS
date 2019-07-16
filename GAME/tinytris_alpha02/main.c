#include <gb/gb.h>
#include <stdio.h> // for debug only
#include <gb/console.h>

#include "gametile.h"
#include "game_bkg.h"
#include "bloc.c"


const char blankmap = {0x00};
const char bloctile[1] = {0x02};

struct GameCharacter bloc; // current bloc
struct GameCharacter next; // next bloc

UINT8 difficulty = 6; // 20=Easy 10=Normal 2=Hard



/* Setup Movemenent for BLOC */

void movethebloc(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spritids, x, y);
}




/* Displaying a BLOC */

// There is a main Issue here because the value isn't x = 0 and y = 0 at top left of the screen.

UBYTE xoffset = 8;
UBYTE yoffset = 16;

void setupbloc(){
    bloc.x = 64; // left (screen) 8 - left (playfield) = 24 // Final release value = 64
    bloc.y = 16; // top (screen) 16 - top (playfield)= 16 // Final release value = 8 or 16

    // load sprites for bloc
    set_sprite_tile(0, 6); // sprite object 0 using tile 4   (Max Sprite = 40 / 10 by scanline)
    bloc.spritids = 0;  // 1rst square of sprite uses sprite object 0
    
    movethebloc(&bloc, bloc.x, bloc.y);
}


void setupnext(){ // Displays the NEXT PIECE
    next.x = 136;
    next.y = 64;

    // load sprites for next bloc
    set_sprite_tile(4, 4);
    next.spritids = 4;

    movethebloc(&next, next.x, next.y);
}


UBYTE canplayermove(UINT8 newplayerx, UINT8 newplayery){ // Collision detection
    UINT8 indexTLy, indexTLx, result, index_TILE;

    indexTLx = (newplayerx - xoffset) / 8;
    indexTLy = (newplayery - yoffset) / 8;

    get_bkg_tiles(indexTLx, indexTLy, 1, 1, &index_TILE);
    
    result = index_TILE == blankmap; // {0x00}
    return result;
}


void transform_piece(UINT8 positionx, UINT8 positiony){  // Transforming bloc (sprites) into background
    UINT8 indexTLx, indexTLy;

    indexTLx = (positionx - xoffset ) / 8;
    indexTLy = (positiony - yoffset ) / 8;
    

    set_bkg_tiles(indexTLx,indexTLy,1,1,bloctile); // x - y - w - h - {0x02}
}

void linecheck(){
    UINT8 index_TILE[100], i, linefull,linefull2;

    for(i = 0; i < 10; i++){
        get_bkg_tiles(i+2, 17, 1, 1, &index_TILE[i]); // Row 1
        get_bkg_tiles(i+2, 16, 1, 1, &index_TILE[i+10]); // Row 2
    }
    
    
    linefull = 0; // Row 1 line
    if(index_TILE[0] && index_TILE[1] && index_TILE[2] && index_TILE[3] && index_TILE[4] && index_TILE[5] & index_TILE[6] & index_TILE[7] & index_TILE[8] & index_TILE[9]){
        linefull = 1;
    }

    linefull2 = 0; // Row 2 line
    if(index_TILE[10] && index_TILE[11] && index_TILE[12] && index_TILE[13] && index_TILE[14] && index_TILE[15] & index_TILE[16] & index_TILE[17] & index_TILE[18] & index_TILE[19]){
        linefull2 = 1;
    }
    
    if(linefull){ // Row 1 line
        set_bkg_tiles(2,17,10,1,blankmap); // 0x00
    }
  
    if(linefull2){ // Row 2 line
        set_bkg_tiles(2,16,10,1,0x00);
    }
}







/* Performant Delay ** Not Used Yet ** */

void performantdelay(UINT8 numloops){         // usage = performantdelay(128);
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
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










void main(){
    /* Sound Initialisation */

    sound_initialisation();

    /* Background Initialisation */

    set_bkg_data(0, 127, gametile);
    set_bkg_tiles(0, 0, 20, 19, game_bkg);
    SHOW_BKG;
    
    
    /* Sprite Initialisation */

    set_sprite_data(0, 127, gametile);
    
    SHOW_SPRITES;
    DISPLAY_ON;   
    
    /* Movements */

    UBYTE gameover = 0; // Not used yet
    UBYTE keydown = 0;
    UBYTE keys;
    UBYTE nextbloc_display = 0;
    UBYTE frame_skip = 0;
    
    setupbloc();    
    setupnext();

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
                    if(canplayermove(bloc.x-8,bloc.y)){
                        bloc.x -= 8;
                        sound_blocmove();
                    }
                    break;
                case J_RIGHT :
                    if(canplayermove(bloc.x+8,bloc.y)){
                        bloc.x += 8;
                        sound_blocmove();
                    }
                    break;
                case J_UP :
                    bloc.y += 144 - bloc.y;
                    sound_blocmove();
                    break;
                case J_DOWN :
                    if(canplayermove(bloc.x,bloc.y+8)){
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
                    if(nextbloc_display){
                        movethebloc(&next, 144, 160); // moving next bloc to invisible
                        nextbloc_display = 0;
                        }
                    else{
                        movethebloc(&next, 136, 64); // replacing next bloc to visible
                        nextbloc_display = 1;
                        }
                    break;
            }
        }
        wait_vbl_done();
        
        if (frame_skip == 0) {
            frame_skip = difficulty;

            if(canplayermove(bloc.x,bloc.y+8)){ // bloc falls from 8 units
                bloc.y += 8;
            }
            else{
                sound_blocdown();
                delay(128);
                transform_piece(bloc.x,bloc.y);
                linecheck();
                delay(128);
                setupbloc();
                }
            
        }
        else{
            frame_skip -= 1;
        }
        
        keydown = keys;

        movethebloc(&bloc,bloc.x,bloc.y);
        
    }
}