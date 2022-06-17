//
// INCOMPLETE
//

//
// Created by kohda on 5/30/2022.
//

#ifndef GAME_PLAYER_HXX
#define GAME_PLAYER_HXX

#include "ge211.hxx"
#include "object.hxx"
// Player is controlled by the user, it's position is determined by the
// x-coord of the mouse cursor
// The member variables for the player are how many lives it has, whether it
// "alive", and whether it has any special statuses currently
// (i.e. invicibility)
class Player : public Object{

public:
    // Constructors
    Player();
    Player(double posx, double posy);

    // Member functions
    // Updates player's x-coord based on input
    void player_to(int x);

    // Checks whether player has more than 0 lives
    bool check_alive() const;

//    // Checks whether object is an obstacle
//    bool check_is_obstacle(object &obj) const;
//
//    // Checks whether object is a coin
//    bool check_is_coin(object &obj) const;
//
//    // Checks whether object is an invincibility power-up
//    bool check_is_invincibility(object &obj) const;
    // ADD OTHER POWER UP CHECKS

    // Getter functions
    int how_many_lives() const{
        return lives;
    }

    bool check_magnetized() const {
        return magnetized;
    }

     bool check_invincible() const {
        return invincible;
    }

    bool check_ff() const{
        return ff;
    }

    // Update functions
    void make_dead(){
        alive = false;
    }
    void minus_life(){
        lives -= 1;
    }

    void add_life(){
        lives+=1;
    }

    void set_invincible(bool b){
        invincible=b;
    }

    void set_magnetized(bool b){
        magnetized=b;
    }

    void set_ff(bool b){
        ff = b;
    }

private:
    // Member variables
    int lives;
    bool alive;
    bool invincible;
    bool magnetized;
    bool ff;
};

#endif //GAME_PLAYER_HXX
