#include "model.hxx"
#include <catch.hxx>

Game_config const config;

TEST_CASE("example test")
{
    CHECK(1 + 1 == 2);
}

TEST_CASE("start with 3 lives. When they hit  obstacle they will lose 1 life, and obstacle will disappear."){
    Model m = Model();
    CHECK(m.runner.how_many_lives()==3);

    ge211::Rect<double> d = m.runner.hitbox_;
    Obstacle b = {d.x,d.y,d.width,d.height};

    CHECK(m.runner.check_collision(b)==true);

    Obstacle c = {d.x+3,d.y+3, d.width, d.height};
    CHECK(m.runner.check_collision(c)==true);

    Obstacle e = {d.x+d.width+100, d.y+d.height+100, d.width, d.height}; // won't collide
    CHECK(m.runner.check_collision(e)==false);

    Obstacle f = {d.x+d.width/2, d.y+d.width/2, d.width, d.height};
    CHECK(m.runner.check_collision(f)==true);

    m.obstacles.clear();
    m.obstacles.push_back(b);

    std::vector<int> check_b = m.which_obstacle(m.obstacles);
    m.remove_obstacle(check_b);
    CHECK(m.runner.how_many_lives()==2);
    CHECK(m.obstacles.size()==0);

    m.obstacles.push_back(c);
    std::vector<int> check_c = m.which_obstacle(m.obstacles);
    m.remove_obstacle(check_c);
    CHECK(m.runner.how_many_lives()==1);
    CHECK(m.obstacles.size()==0);

    m.obstacles.push_back(e);
    std::vector<int> check_e = m.which_obstacle(m.obstacles);
    m.remove_obstacle(check_e);
    CHECK(m.runner.how_many_lives()==1);
    CHECK(m.obstacles.size()==1);
    m.obstacles.clear();
    CHECK(m.obstacles.size()==0);

    m.obstacles.push_back(f);
    std::vector<int> check_f = m.which_obstacle(m.obstacles);
    m.remove_obstacle(check_f);
    CHECK(m.runner.how_many_lives()==0);
    CHECK(m.obstacles.size()==0);
}


TEST_CASE("when player has 0 lives, the game ends"){
    Model m = Model();
    CHECK(m.runner.how_many_lives()==3);
    CHECK(m.runner.check_alive());

    m.runner.minus_life();
    m.runner.minus_life();
    m.runner.minus_life();

    CHECK(m.runner.how_many_lives()==0);
    CHECK(!m.runner.check_alive());
}


TEST_CASE("When a player passes through a coin, they collect it"){
    Model m = Model();
    m.coins.clear();
    m.coins.push_back(Coin(m.runner.hitbox_.x, m.runner.hitbox_.y,10,10));

    CHECK(m.coins_collected==0);
    std::vector<int> c = m.which_coins(m.coins);
    m.remove_coins(c);
    CHECK(m.coins_collected==1);

}

TEST_CASE("When a player passes through an invincibility power-up, "
          "they can move through obstacles without losing lives for a set period of time."){
    Model m = Model();
    double dt = 0.01;
    m.active_powerup=Powerups(m.runner.hitbox_.x,m.runner.hitbox_.y-m
    .y_velocity*dt,10,
                                       10, false, true, false, false, true);
    CHECK(m.runner.how_many_lives()==3);
    m.powerups_on_frame(dt);
    CHECK(m.runner.check_invincible());


    m.obstacles.clear();
    Obstacle b = {m.runner.hitbox_.x,m.runner.hitbox_.y,m.runner.hitbox_.width,m.runner.hitbox_.height};
    m.obstacles.push_back(b);
    std::vector<int> check_b = m.which_obstacle(m.obstacles);
    m.remove_obstacle(check_b);

    CHECK(m.runner.how_many_lives()==3);
}

TEST_CASE("When the player passes through an extra-life powerup, they gain an additional life."){
    Model m = Model();
    double dt = 0.01;
    m.active_powerup=Powerups(m.runner.hitbox_.x,m.runner.hitbox_.y-m
    .y_velocity*dt,10,
                              10, false, false, true, false, true);
    CHECK(m.runner.how_many_lives()==3);
    m.powerups_on_frame(dt);
    CHECK(m.runner.how_many_lives()==4);
}

TEST_CASE("When the player passes through a fast forward powerup, the objects "
          "around them increase velocity"){
    Model m = Model();
    double dt = 0.01;
    m.active_powerup=Powerups(m.runner.hitbox_.x,m.runner.hitbox_.y-m
                                                                            .y_velocity*dt,10,
                              10, false, false, false, true, true);
    int check_velocity = m.y_velocity;
    m.powerups_on_frame(dt);
    CHECK(m.y_velocity == check_velocity * 4);
    CHECK(m.runner.check_invincible());
}

TEST_CASE("When the obstacle reaches the bottom, it will reappear at the top "
          "in one of three lane positions"){
    Model m = Model();
    Obstacle b = {0,805,50,50};

    m.obstacles.clear();
    m.obstacles.push_back(b);

    int dt = 1;
    m.on_frame(dt);

    int p1 = config.side_margin;
    int p2 = config.side_margin + config.brick_spacing
                           .width+config.brick_dims().width;
    int p3 = config.side_margin + 2 * (config.brick_spacing
                                                      .width+config
                                                      .brick_dims().width);
    CHECK(m.obstacles[0].hitbox_.y == 0);

    bool test = (m.obstacles[0].hitbox_.x == p1) || (m.obstacles[0].hitbox_.x ==
                                                     p2) || (m.obstacles[0]
                                                             .hitbox_.x == p3);

    CHECK(test);
}