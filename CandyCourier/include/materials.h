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

const Vector2D texture_locations[4] =
{
	Vector2D(2, 2), // NONE
	Vector2D(0, 0), // STONE
	Vector2D(1, 0), // DIRT
	Vector2D(2, 0)  // GRASS
};

#endif

