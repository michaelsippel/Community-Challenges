/* 
 * File:   Ball.cpp
 * Author: micha
 * 
 * Created on 1. April 2014, 17:44
 */

#include "Ball.h"
#include <oxygarum.h>

Ball::Ball() {
    this->object = oxygarum_create_object3d();
    this->pos.x = 0;
    this->pos.y = 0;
    this->velocity.x = 0.01;
    this->velocity.y = 0;
}

Ball::Ball(const Ball& orig) {
}

Ball::~Ball() {
}

void Ball::update(float speed) {
    this->pos.x += this->velocity.x * speed;
    this->pos.y += this->velocity.y * speed;
    
    this->object->pos.x = this->pos.x;
    this->object->pos.z = this->pos.y;
}

vertex2d_t Ball::get_pos() {
    return this->pos;
}
