#ifndef _MATERIALS_H
#define _MATERIALS_H

#include <oxygarum.h>

using namespace oxygarum;

enum block_type
{
	NONE,
	STONE,
	DIRT,
	GRASS
};

#define TEXTURE_GRID_X 4
#define TEXTURE_GRID_Y 4

const int texture_locations[4][2] =
{
	{2, 2}, // NONE
	{0, 0}, // STONE
	{1, 0}, // DIRT
	{2, 0}  // GRASS
};

void gen_texcoords(Vector2D *texcoords, int x, int y, int gx, int gy);

#endif

