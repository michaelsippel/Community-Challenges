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

int Player::check_collision(Ball *ball) {
    
}
