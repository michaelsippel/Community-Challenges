
#include <oxygarum.h>
#include "player.h"

Player::Player()
{
	this->mesh = new Mesh3D();
	this->obj = new Object3D();
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

