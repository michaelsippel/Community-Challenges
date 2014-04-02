/* 
 * File:   Controler.h
 * Author: micha
 *
 * Created on 1. April 2014, 17:39
 */

#ifndef CONTROLER_H
#define	CONTROLER_H

class Controler {
public:
    Controler();
    Controler(const Controler& orig);
    virtual ~Controler();
    
    void left();
    void left_up();
    void right();
    void right_up();
    
private:
    
};

#endif	/* CONTROLER_H */

