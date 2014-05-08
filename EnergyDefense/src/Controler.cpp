/* 
 * File:   Controler.cpp
 * Author: micha
 * 
 * Created on 1. April 2014, 17:39
 */

#include "Controler.h"
#include "Player.h"
#include "Ball.h"

extern Player *p1;
extern Ball *ball;

void controler_left(void) {
    p1->move(-1);
}
void controler_right(void) {
    p1->move(1);
}
void controler_up(void) {
    p1->move(0);
}

void controler_click_left(void) {
    ball->launch(p1);
}
void controler_click_right(void) {
    ball->stop(p1);
}
