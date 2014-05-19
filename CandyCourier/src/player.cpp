
#include <oxygarum.h>
#include "player.h"
#include "materials.h"

Player::Player()
{
	Vector3D *vertices = (Vector3D*) calloc(4, sizeof(Vector3D));
	Vector2D *texcoords = (Vector2D*) calloc(4, sizeof(Vector2D));
	Face **faces = (Face**) calloc(1, sizeof(Face*));

	vertices[0] = Vector3D(-0.5f, +0.5f, 0.0f);
	vertices[1] = Vector3D(+0.5f, +0.5f, 0.0f);
	vertices[2] = Vector3D(+0.5f, -0.5f, 0.0f);
	vertices[3] = Vector3D(-0.5f, -0.5f, 0.0f);
	gen_texcoords(texcoords, 0, 3, TEXTURE_GRID_X, TEXTURE_GRID_Y);

	int *indices = (int*) calloc(4, sizeof(int));
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	faces[0] = new Face(4, indices, indices);

	this->mesh = new Mesh3D(4, vertices, 4, texcoords, 1, faces);
	this->obj = new Object3D(Vector3D(0.0f, 8.0f, 0.0f));
	this->obj->mesh = this->mesh;

	this->obj->setFlag(OBJECT_TRANSPARENT);
}

Player::~Player()
{
}

void Player::update(float speed)
{
	this->obj->position.x += speed * vel;
}

void Player::move(int vel_)
{
	this->vel = vel_;
}

void Player::jump(int height)
{
	if(this->jump_count == 0)
	{
		this->jump_count = height;
	}
}

