#ifndef _MATERIALS_H
#define _MATERIALS_H

#include <oxygarum.h>

using namespace oxygarum;

enum block_type
{
	NONE,
	STONE,
	DIRT,
	GRASS,
	SKY1,
	SKY2,
	SKY3,
	SKY4,
	CHOCO,
	BONBON,
	GUMMI
};

#define TEXTURE_GRID_X 4
#define TEXTURE_GRID_Y 4

const int texture_locations[11][2] =
{
	{2, 2}, // NONE
	{0, 0}, // STONE
	{1, 0}, // DIRT
	{2, 0}, // GRASS
	// SKY
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	// CANDIES
	{0, 2},
	{1, 2},
	{2, 2}
};

void gen_texcoords(Vector2D *texcoords, int x, int y, int gx, int gy);

#endif

