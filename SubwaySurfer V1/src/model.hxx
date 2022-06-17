//
// INCOMPLETE
//

#pragma once

#include <ge211.hxx>
#include "vector"
#include <object.hxx>
#include <obstacle.hxx>
#include <coin.hxx>
#include <player.hxx>
#include <game_config.hxx>
#include <powerups.hxx>

// The settings of the game as well as its backend
class Model
{
public:
    // Constructors
    Model();
    Model(Game_config const& config);

    // Member functions
    // Starts game when ' ' is pressed
    void launch();
    // Updates everything each frame
    void on_frame(double dt);
    // Returns a random x-coord for one of the lanes
    int random_x();
    //
    bool collide(Obstacle const &obstacle) const;
    //
    std::vector<int> which_obstacle(std::vector<Obstacle>& a) const;
    //
    std::vector<Obstacle> remove_obstacle(std::vector<int> remove_obstacle_vec);


    // Member Variables
    // Standard configuration of Model provided by
    Game_config const config;
    // All of the obstacles, in no particular order.
    std::vector<Obstacle> obstacles;
    // All of the coins, in no particular order.
    std::vector<Coin> coins;
    // Starting position of the player.
    Player runner;
    // Beginning velocity when the game is started
    int start_y_velocity;
    // Current velocity of the level
    int y_velocity;
    // Coins collected
    int coins_collected;
    // Distance travelled
    int distance;
    // In-game timer (60 * (actual seconds passed))
    double time;
    // What level (how fast it will be going) you are on
    int level;
    // Vector that records which obstacles are collided with and removed
    std::vector<Obstacle> removed_obstacles;

    std::vector<Coin> removed_coins;

    std::vector<int> which_coins(std::vector<Coin> &c);

    void remove_coins(std::vector<int> remove_coin_vec);

    void coin_on_frame(double dt);

    Powerups active_powerup;

    void powerups_on_frame(double dt);

    int time_count_ff;
};
