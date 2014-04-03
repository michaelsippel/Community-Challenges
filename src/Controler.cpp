/* 
 * File:   Controler.cpp
 * Author: micha
 * 
 * Created on 1. April 2014, 17:39
 */

#include "Controler.h"
#include "Player.h"

extern Player *p1;

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
    printf("left!\n");
}
void controler_click_right(void) {
    printf("right!\n");
}
