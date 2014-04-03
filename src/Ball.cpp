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
    this->velocity.x = 0.05;
    this->velocity.y = 0.01;
}

Ball::Ball(const Ball& orig) {
}

Ball::~Ball() {
}

void Ball::update(float speed) {
    this->pos.x += this->velocity.x * speed;
    this->pos.y += this->velocity.y * speed;
    
    if(this->pos.x > 20.0f || this->pos.x < -20.0f) {
        this->velocity.x *= -1.0f;
    }
    
    if(this->pos.y > 15.0f || this->pos.y < -15.0f) {
        this->velocity.y *= -1.0f;
    }
    
    this->object->pos.x = this->pos.x;
    this->object->pos.z = this->pos.y;
}

vertex2d_t Ball::get_pos() {
    return this->pos;
}
