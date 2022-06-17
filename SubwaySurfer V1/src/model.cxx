//
// INCOMPLETE
//

#include "model.hxx"
#include <iostream>

Model::Model()
: Model(Game_config())
{ }

Model::Model(Game_config const& config)
        : config(config),
          runner((config.scene_dims.width/2)-12.5, config.scene_dims.height-config
          .bottom_margin-50),
          start_y_velocity(200),
          y_velocity(0),
          coins_collected(0),
          distance(0),
          time(0),
          level(1),
          active_powerup(Powerups(0,0,10,
                                  10, false, false, false, false, false)),
          time_count_ff(0)
{
    ge211::Posn<int> p = {config.side_margin, config.top_margin};

    // Want to initialize two cols of bricks (on the outer cols)
    std::vector<int> outer(2,1);
    outer[1] = config.brick_cols;

    for(int ii = 1; ii <= config.brick_rows; ii++){
        p.x = config.side_margin;

        for(int jj = 1; jj <= config.brick_cols; jj++ ){
            if(jj == 1 || jj == config.brick_cols){ // Add obstacles
                obstacles.push_back(Obstacle(p.x, p.y,
                                             config.brick_dims().width,
                                             config.brick_dims().height));
            }
            else{ // add COINS
                coins.push_back(Coin(p.x, p.y,
                                         config.brick_dims().width,
                                         config.brick_dims().height));
            }
            p.x += config.brick_spacing.width+config.brick_dims().width;
        }

        p.y += config.brick_spacing.height+config.brick_dims().height;
    }
}

void
Model::launch(){
    y_velocity = start_y_velocity;
}

int
Model::random_x(){
    // Get random value between 0 and (rightmost col - 1)
    ge211::Random_source<int> rand_col(0, config.brick_cols-1);
    int rand = rand_col.next();

    int p = config.side_margin;
    p += rand*(config.brick_spacing.width+config.brick_dims().width);
    return p;
}

std::vector<int> // which obstacles the player collided with
Model::which_obstacle(std::vector<Obstacle>& a) const{
    std::vector<int> remove_obstacle_vec;
    for(size_t ii=0; ii<a.size(); ii++){
        if(runner.check_collision(a[ii])){
            remove_obstacle_vec.push_back(ii); // ADD TO VECTOR, DON'T TERMINATE EARLY
        }
    }
    return remove_obstacle_vec;
}

std::vector<Obstacle> // remove obstacle if player collided, minus life
Model::remove_obstacle(std::vector<int> remove_obstacle_vec) {
    std::vector<Obstacle> removed;
    for (size_t ii=0; ii<remove_obstacle_vec.size(); ii++){
        int remove = remove_obstacle_vec[ii];
        removed.push_back(obstacles[remove]);
        obstacles[remove] = obstacles.back();
        obstacles.pop_back();
    }

    if (!runner.check_invincible() && remove_obstacle_vec.size() != 0){ // only
        // subtract life if not invincible
        runner.minus_life();
    }

    return removed;
}


std::vector<int>
Model:: which_coins(std::vector<Coin>& c){
    std::vector<int> remove_coin_vec;
    for(size_t ii=0; ii<c.size(); ii++){
        if(runner.check_collision(c[ii])){
            remove_coin_vec.push_back(ii); // ADD TO VECTOR, DON'T TERMINATE EARLY
        }
    }
    return remove_coin_vec;
} // which coins the player collided with

void // remove all collided coins from main coins vector and move to removed_coins vector, add coin score
Model:: remove_coins(std::vector<int> remove_coin_vec){
    for (size_t ii=0; ii<remove_coin_vec.size(); ii++){
        int remove = remove_coin_vec[ii];
        removed_coins.push_back(coins[remove]);
        coins[remove] = coins.back();
        coins.pop_back();
        coins_collected++;
    }
}

void Model:: coin_on_frame(double dt){
    std::vector<Coin> temp_coin;
    temp_coin = coins;
    for (Coin &c: temp_coin){
        c.next(dt, y_velocity);
    }

    if (which_coins(temp_coin).size()!=0){
        remove_coins(which_coins(temp_coin));
    }

    // coins that player never collided with
    for (Coin &c: coins){
        Coin temp_coin = c;
        temp_coin.next(dt, y_velocity);

        // WHEN OFF THE SCREEN, MOVE BACK UP AT A RANDOM X
        if(temp_coin.hitbox_.y > config.scene_dims.height){
            c.hitbox_.y = 0;
            c.hitbox_.x = random_x();
            if (runner.check_magnetized()){ // if magnet, then automatically collect coins as they go off screen
                coins_collected++;
            }
        }

        else{
            c.next(dt, y_velocity);
        }
    }

    // coins that player already collided
    for (unsigned int ii=0; ii<removed_coins.size(); ii++){
        Coin temp_coin = removed_coins[ii];
        temp_coin.next(dt, y_velocity);

        // WHEN OFF THE SCREEN, MOVE BACK UP AT A RANDOM X
        if(temp_coin.hitbox_.y > config.scene_dims.height){
            temp_coin.hitbox_.y = 0;
            temp_coin.hitbox_.x = random_x();
            coins.push_back(temp_coin); // add back to bricks
            removed_coins[ii]= removed_coins.back(); // remove from removed_bricks
            removed_coins.pop_back();
        }
        else{
            removed_coins[ii].next(dt, y_velocity); // keep updating the bricks
        }
    }
}

void Model:: powerups_on_frame(double dt){
    if (active_powerup.in_play){ // check if particular powerup is active
            Powerups temp_p = active_powerup;
            temp_p.next(dt, y_velocity);
            if (runner.check_collision(temp_p)){
                if (temp_p.invincibility){
                    runner.set_invincible(true);
                }
                else if(temp_p.magnet){
                    runner.set_magnetized(true);

                }
                else if (temp_p.extra_life){
                    runner.add_life();
                }
                else if (temp_p.ff){
                    time_count_ff=0;
                    y_velocity=y_velocity*4;
                    runner.set_invincible(true);
                    runner.set_ff(true);
                }
                active_powerup.in_play=false; // collided --> therefore, remove from screen
            }
            else{
                active_powerup.next(dt,y_velocity);
            }
    }
}

void // calls coin_on_frame + powerups_on_frame, implements on_frame for obstacles
Model::on_frame(double dt){
    // Create temp obstacles that scout one frame ahead
    std::vector<Obstacle> temp_obstacles;
    temp_obstacles = obstacles;
    for(Obstacle &obstacle : temp_obstacles) {
        obstacle.next(dt, y_velocity);
    }

    if(which_obstacle(temp_obstacles).size() != 0){
        std::vector<Obstacle> b = remove_obstacle(which_obstacle(temp_obstacles));
        for (Obstacle i : b){
            removed_obstacles.push_back(i); // add to variable containing all removed bricks
        }
    }

    if(runner.how_many_lives() <= 0){
        runner.make_dead();
        runner.live = false;
        return;
    }

    // MOVING BRICKS DOWN SCREEN
    // HIT BOTTOM OF SCREEN
    for(Obstacle &obstacle : obstacles){

        Obstacle temp_obstacle = obstacle;
        temp_obstacle.next(dt, y_velocity);

        // WHEN OFF THE SCREEN, MOVE BACK UP AT A RANDOM X
        if(temp_obstacle.hitbox_.y > config.scene_dims.height){
            obstacle.hitbox_.y = 0;
            obstacle.hitbox_.x = random_x();
        }

        else{
            obstacle.next(dt, y_velocity);
        }
    }

    // PLAYER ALREADY COLLIDED
    for (unsigned int ii=0; ii<removed_obstacles.size(); ii++){
        Obstacle temp_obstacle = removed_obstacles[ii];
        temp_obstacle.next(dt, y_velocity);

        // WHEN OFF THE SCREEN, MOVE BACK UP AT A RANDOM X
        if(temp_obstacle.hitbox_.y > config.scene_dims.height){
            temp_obstacle.hitbox_.y = 0;
            temp_obstacle.hitbox_.x = random_x();
            obstacles.push_back(temp_obstacle); // add back to bricks
            removed_obstacles[ii]= removed_obstacles.back(); // remove from removed_bricks
            removed_obstacles.pop_back();
        }
        else{
            removed_obstacles[ii].next(dt, y_velocity); // keep updating the bricks
        }
    }

    if (int(time) % 600 ==0){ //activate power-ups
        runner.set_magnetized(false);
        runner.set_invincible(false);
        ge211::Random_source<int> rand(0, 3); // is this INCLUSIVE or EXCLUSIVE
        int rand_ind = rand.next();
        if (rand_ind == 0){
            active_powerup= Powerups(0,0,config.brick_dims().width,
                                     config.brick_dims().height, true, false, false, false, true);
        }
        else if(rand_ind==1){
            active_powerup= Powerups(0,0,config.brick_dims().width,
                                     config.brick_dims().height, false, true, false, false, true);
        }
        else if (rand_ind==2){
            active_powerup= Powerups(0,0,config.brick_dims().width,
                                     config.brick_dims().height, false, false, true, false, true);
        }
        else{
            active_powerup= Powerups(0,0,config.brick_dims().width,
                                     config.brick_dims().height, false, false, false, true, true);
        }
    }

    if (runner.check_ff()){ // separate deactivation for fast forward
        time_count_ff++;
        if (time_count_ff>45){
            runner.set_ff(false);
            runner.set_invincible(false);
            y_velocity=(y_velocity/4);
        }
    }
    coin_on_frame(dt);

    powerups_on_frame(dt);

    if(runner.live) {
        time +=1;
        distance+= time*y_velocity/(60*20);
        int temp_time = time;
        int divider = 60*2;
        if (y_velocity < 1200){
            if((temp_time % divider) == 0){
                y_velocity += 10;
                level += 1;

            }
        }
    }
}