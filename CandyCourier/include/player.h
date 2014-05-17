#ifndef _PLAYER_H
#define _PLAYER_H

#include <oxygarum.h>

using namespace oxygarum;

class Player
{
	public:
		Player();
		~Player();

		void update(float speed);
		void move(int vel_);
		void jump(int height);

		Mesh3D *mesh;
		Object3D *obj;

	private:
		int vel;
		int jump_count;
};

#endif

