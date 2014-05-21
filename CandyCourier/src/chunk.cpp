
#include <oxygarum.h>
#include "chunk.h"

Chunk::Chunk()
{
	static int count = 0;
	this->mesh = NULL;
	this->obj = NULL;
	this->id = count++;

	this->vertices = (Vector3D*) calloc(4*16*16,sizeof(Vector3D));
	this->texcoords = (Vector2D*) calloc(4*16*16, sizeof(Vector2D));
	this->faces    = (Face**) calloc(16*16, sizeof(Face*));
}

Chunk::~Chunk()
{
}

void Chunk::generate_mesh(void)
{
	int num_vertices = 0;
	int num_faces = 0;

	int x, y;

	// generating vertices
	for(x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(y = 0; y < CHUNK_SIZE_Y; y++)
		{
			if(this->blocks[x][y] != NONE)
			{
				int *indices = (int*) calloc(4, sizeof(int));
				indices[0] = num_faces*4 + 0;
				indices[1] = num_faces*4 + 1;
				indices[2] = num_faces*4 + 2;
				indices[3] = num_faces*4 + 3;
				
				this->vertices[indices[0]] = Vector3D(x-0.5f,y+0.5f, 0.0f);
				this->vertices[indices[1]] = Vector3D(x+0.5f,y+0.5f, 0.0f);
				this->vertices[indices[2]] = Vector3D(x+0.5f,y-0.5f, 0.0f);
				this->vertices[indices[3]] = Vector3D(x-0.5f,y-0.5f, 0.0f);
				
				gen_texcoords(&this->texcoords[indices[0]], texture_locations[this->blocks[x][y]][0], texture_locations[this->blocks[x][y]][1], TEXTURE_GRID_X,TEXTURE_GRID_Y);
				
				this->faces[num_faces] = new Face(4, indices, indices);
				num_faces++;
				num_vertices+=4;
			}
		}
	}

	if(this->mesh == NULL)
	{
		this->mesh = new Mesh3D(num_vertices, vertices, num_vertices, texcoords, num_faces, faces);
		this->mesh->instance = new RenderInstance(mesh);
	}

	if(this->obj == NULL)
	{
		this->obj = new Object3D(Vector3D((float)this->id*(float)CHUNK_SIZE_X, 0.0f, 0.0f));
		this->obj->mesh = mesh;
		this->obj->setFlag(OBJECT_RENDER_VBO);
	}
}

