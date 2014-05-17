#ifndef _CHUNK_H
#define _CHUNK_H

#include <oxygarum.h>
#include "materials.h"

using namespace oxygarum;

#define CHUNK_SIZE_X 8
#define CHUNK_SIZE_Y 8

class Chunk
{
	public:
		Chunk();
		~Chunk();
	
		enum block_type blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y];

	private:
		Mesh3D *mesh;
		Object3D *obj;
};

#endif

