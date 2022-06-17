//
// INCOMPLETE
//

//
// Created by kohda on 5/30/2022.
//

#include "ge211.hxx"
#include "player.hxx"
#include "object.hxx"
#include <typeinfo>

Player::Player()
: Player(0, 0)
{ }

Player::Player(double posx, double posy)
:
Object(posx, posy, 25, 50, false),
lives(3),
alive(true),
invincible(false),
magnetized(false),
ff(false)
{ }

void
Player::player_to(int x){
    if(!live){
        return;
    }
    else{
        hitbox_.x = x;
    }
}

bool
Player::check_alive() const{
    return (lives>0);
}
