
#include <oxygarum.h>
#include "player.h"
#include "materials.h"
#include "chunk.h"

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
	this->obj = new Object3D(Vector3D(1.00f, 15.0f, 0.0f));
	this->obj->mesh = this->mesh;

	this->obj->setFlag(OBJECT_TRANSPARENT);
}

Player::~Player()
{
}

void Player::update(float speed, Chunk **chunks)
{
	// check collisions
	int collide_left = 0;
	int collide_right = 0;
	int collide_top = 0;
	int collide_bottom = 0;

	Chunk *current_chunk = chunks[0];
	int cx = (int) (0.5+this->obj->position.x - current_chunk->id*CHUNK_SIZE_X);
	int cy = (int) (0.5+this->obj->position.y);

	if(cx > 0)
	{
		if(current_chunk->blocks[cx-1][cy] != NONE &&
			this->obj->position.x <= cx)
		{
			collide_left = 1;
		}
	}

	if(cx+1 < CHUNK_SIZE_X)
	{
		if(current_chunk->blocks[cx+1][cy] != NONE &&
			this->obj->position.x >= cx)
		{
			collide_right = 1;
		}
	}

	if(cy+1 < CHUNK_SIZE_Y)
	{
		if(current_chunk->blocks[cx][cy+1] != NONE &&
			this->obj->position.y >= cy)
		{
			collide_top = 1;
		}
	}

	if(cy > 0)
	{
		if(current_chunk->blocks[cx][cy-1] != NONE &&
			this->obj->position.y <= cy)
			
		{
			collide_bottom = 1;
		}
	}
	
	// gravitation
	this->vel_y -= 0.981 * speed;
	
	// stop if collide
	if( (collide_left && vel_x < 0.0f) ||
	    (collide_right && vel_x > 0.0f))
	{
		this->vel_x = 0.0f;
		this->obj->position.x = (float) cx;
	}

	if( (collide_top && vel_y > 0.0f) ||
	    (collide_bottom && vel_y < 0.0f))
	{
		this->vel_y = 0.0f;
		this->obj->position.y = (float) cy;
	}

	// move
	this->obj->position.x += speed * this->vel_x;
	this->obj->position.y += speed * this->vel_y;
}

void Player::move(float vel)
{
	this->vel_x = (float)vel;
}

void Player::jump(float vel)
{
	if(this->vel_y == 0.0f)
	{
		this->vel_y = vel;
	}
}

