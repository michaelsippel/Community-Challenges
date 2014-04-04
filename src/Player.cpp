/* 
 * File:   Player.cpp
 * Author: micha
 * 
 * Created on 1. April 2014, 17:32
 */

#include "Player.h"
#include "Ball.h"
#include <oxygarum.h>
#include <math.h>

extern Ball *ball;

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
    switch (v) {
        case -1:
            this->velocity = -0.05;
            break;
        case 0:
            this->velocity = 0;
            break;
        case 1:
            this->velocity = 0.05;
            break;
    }
}

void Player::update(float speed) {
    if ((this->racket->pos.z < -12.0f && this->velocity < 0.0f) ||
            (this->racket->pos.z > 12.0f && this->velocity > 0.0f)) {
        this->velocity = 0.0f;
    }

    this->use_power(speed * 0.01);

    if (power > 0.0f) {
        this->racket->pos.z += this->velocity * speed;
        if (this->velocity != 0.0f) {
            this->use_power(-speed * 0.04);
        }
    }
}

void Player::use_power(float diff) {
    this->power += diff;

    if (this->power <= 0.0f) {
        this->power = 0.0f;
    }

    if (this->power > 100.0f) {
        this->power = 100.0f;
    }
}

float Player::get_power(void) {
    return this->power;
}

int Player::get_score(void) {
    return this->score;
}

void Player::score_up(void) {
    this->score++;
}

void Player::reset(void) {
    this->power = 100.0f;

}

int Player::has_ball(void) {
    return ball->get_player() == this ? 1 : 0;
}

int Player::check_collision(vertex2d_t ball_pos) {
    if (ball_pos.y > this->racket->pos.z - 4.0f &&
            ball_pos.y < this->racket->pos.z + 4.0f &&
            ball_pos.x > this->racket->pos.x - 1.0f &&
            ball_pos.x < this->racket->pos.x + 1.0f) {
        return 1;
    } else {
        return 0;
    }
}
