/* 
 * File:   Player.cpp
 * Author: micha
 * 
 * Created on 1. April 2014, 17:32
 */

#include "Player.h"
#include <oxygarum.h>

Player::Player() {
    this->racket = oxygarum_create_object3d();
    this->plasma1 = oxygarum_create_object3d();
    this->plasma2 = oxygarum_create_object3d();
    this->power = 100.0f;
    this->score = 0;
}

Player::Player(const Player& orig) {
}

Player::~Player() {
}

void Player::move(int v) {
    switch(v) {
        case -1:
            this->velocity = -0.03;
            break;
        case 0:
            this->velocity = 0;
            break;
        case 1:
            this->velocity = 0.03;
            break;
    }
}

void Player::update(float speed) {
    if( (this->racket->pos.z < -12.0f && this->velocity < 0.0f) ||
        (this->racket->pos.z >  12.0f && this->velocity > 0.0f))
    {
        this->velocity = 0.0f;
    }
    this->racket->pos.z += this->velocity * speed;
}

int Player::check_collision(Ball *ball) {
    
}
