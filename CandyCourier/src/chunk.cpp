
#include <oxygarum.h>
#include "chunk.h"

Chunk::Chunk()
{
	static int count = 0;
	this->mesh = NULL;
	this->obj = NULL;
	this->id = count++;
}

Chunk::~Chunk()
{
}

void Chunk::generate_mesh(void)
{
	int num_vertices = 0;
	int num_faces = 0;

	int x, y;

	// counting
	for(x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(y = 0; y < CHUNK_SIZE_Y; y++)
		{
			if(this->blocks[x][y] != NONE)
			{
				char *s;
				switch(this->blocks[x][y])
				{
					case STONE:
						s = "stone";
						break;
					case DIRT:
						s = "dirt";
						break;
					case GRASS:
						s = "grass";
						break;
				};
	//			printf("(%d|%d) is %s\n", x, y, s);
				num_faces++;
				num_vertices += 4;
			}
		}
	}

	//printf("have %d vertices on %d faces\n", num_vertices, num_faces);

	// allocating memory
	Vector3D *vertices = (Vector3D*) calloc(num_vertices, sizeof(Vector3D));
	Vector2D *texcoords = (Vector2D*) calloc(num_vertices, sizeof(Vector2D));
	Face    **faces    = (Face**) calloc(num_faces, sizeof(Face*));

	// generating vertices
	int block_ind = 0;
	for(x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(y = 0; y < CHUNK_SIZE_Y; y++)
		{
			if(this->blocks[x][y] != NONE)
			{
				int *indices = (int*) calloc(4, sizeof(int));
				indices[0] = block_ind*4 + 0;
				indices[1] = block_ind*4 + 1;
				indices[2] = block_ind*4 + 2;
				indices[3] = block_ind*4 + 3;

				vertices[indices[0]] = Vector3D(x-0.5f,y+0.5f, 0.0f);
				vertices[indices[1]] = Vector3D(x+0.5f,y+0.5f, 0.0f);
				vertices[indices[2]] = Vector3D(x+0.5f,y-0.5f, 0.0f);
				vertices[indices[3]] = Vector3D(x-0.5f,y-0.5f, 0.0f);

				gen_texcoords(&texcoords[indices[0]], texture_locations[this->blocks[x][y]][0], texture_locations[this->blocks[x][y]][1], TEXTURE_GRID_X,TEXTURE_GRID_Y);

				faces[block_ind] = new Face(4, indices, indices);
				block_ind ++;
			}
		}
	}

	this->mesh = new Mesh3D(num_vertices, vertices, num_vertices, texcoords, num_faces, faces);
	this->mesh->instance = new RenderInstance(mesh);

	this->obj = new Object3D();
	this->obj->mesh = mesh;
	this->obj->setFlag(OBJECT_RENDER_VBO);

	this->obj->material = new Material(Color(1.0f, 0.0f, 0.0f, 1.0f));
}

