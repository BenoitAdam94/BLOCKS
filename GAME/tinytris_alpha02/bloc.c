#include <gb/gb.h>

//generical character structure: id, position, graphics
struct GameCharacter {
	UBYTE spritids; // all characters use 4 sprites
	UINT8 x;
	UINT8 y;
};