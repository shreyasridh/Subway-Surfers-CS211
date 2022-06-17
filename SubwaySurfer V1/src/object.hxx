//
// INCOMPLETE
//

//
// Created by kohda on 5/30/2022.
//

#ifndef GAME_OBJECT_HXX
#define GAME_OBJECT_HXX

#include "ge211.hxx"

// Represents all in-game objects (player, coin, obstacle, etc.), they are
// comprised of a "hitbox" that determines where and how big they are.
// This "hitbox" also determines when they collide with another object.
// Additionally, a member variable field "live" decides whether it is in play
// currently, this means different things for different objects
struct Object{
    // Constructors
    Object(double posx, double posy, double width, double height, bool is_it);
    Object();

    // Member variable
    // This is the 'hittable' area of an object
    ge211::Rect<double> hitbox_;
    bool live;

    // Member functions
    // Checks collisions with any other objects
    bool check_collision(Object &obj) const;
    // Moves object down one frame
    void next(double dt, int vel);



};

#endif //GAME_OBJECT_HXX
