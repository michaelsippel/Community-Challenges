#ifndef _CHUNK_H
#define _CHUNK_H

#include <oxygarum.h>
#include "materials.h"

using namespace oxygarum;

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16

class Chunk
{
	public:
		Chunk();
		~Chunk();
	
		enum block_type blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y];
		void generate_mesh(void);

		Mesh3D *mesh;
		Vector3D *vertices;
		Vector2D *texcoords;
		Face **faces;
		Object3D *obj;

		int id;
};

#endif

