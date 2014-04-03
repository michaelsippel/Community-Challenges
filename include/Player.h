/* 
 * File:   Player.h
 * Author: micha
 *
 * Created on 1. April 2014, 17:32
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include "Ball.h"
#include <oxygarum.h>

class Player {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();
    
    void move(int v);
    
    int check_collision(Ball *ball);
    void update(float speed);
    float get_pos();
    float get_power();
    void use_power(float diff);
    
    object3d_t *racket;
    object3d_t *plasma1;
    object3d_t *plasma2;
    
private:
    float power;
    int score;
    
    float velocity;
};

#endif	/* PLAYER_H */

