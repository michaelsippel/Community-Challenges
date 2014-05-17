
#include <oxygarum.h>
#include "chunk.h"

Chunk::Chunk()
{
	this->mesh = NULL;
	this->obj = NULL;
}

Chunk::~Chunk()
{
}

void Chunk::generate_mesh(void)
{
	int num_vertices = 4;
	int num_faces = 1;

	Vector3D *vertices = (Vector3D*) calloc(num_vertices, sizeof(Vector3D));
	Face    **faces    = (Face**) calloc(num_faces, sizeof(Face*));

	vertices[0] = Vector3D(-1.0f, 1.0f, 0.0f);
	vertices[1] = Vector3D( 1.0f, 1.0f, 0.0f);
	vertices[2] = Vector3D( 1.0f,-1.0f, 0.0f);
	vertices[3] = Vector3D(-1.0f,-1.0f, 0.0f);

	int *indices = (int*) calloc(4, sizeof(int));
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	faces[0] = new Face(4, indices);

	this->mesh = new Mesh3D(num_vertices, vertices, num_faces, faces);
//	this->mesh->instance = new RenderInstance(mesh);

	this->obj = new Object3D();
	this->obj->mesh = mesh;
//	this->obj->setFlag(OBJECT_RENDER_VBO);

	this->obj->material = new Material(Color(1.0f, 0.0f, 0.0f, 1.0f));
}

