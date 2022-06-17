//
// INCOMPLETE
//

#include "view.hxx"

// Key colors (make sure to update)
ge211::Color const player_normal {255, 255, 127};
ge211::Color const player_invincible {50, 150, 50}; // green = invincible
ge211::Color const invincibility_color {50, 150, 50}; // green = invincible
ge211::Color const player_magnet {220, 20, 60}; // red = magnet
ge211::Color const magnet_color {220, 20, 60}; // red = magnet
ge211::Color const coin_color {215,219,88}; // yellow
ge211::Color const obstacle_color {100, 100, 100};// grey = obstacle
ge211::Color const extra_lives_color{131,238,255}; // blue = extra lives
ge211::Color const ff_color{138, 44,226};
static std::string const bg_music_filename {"bg_music.mp3"};


View::View(Model const& m, ge211::Mixer& mixer)
        : model_(m),
          //player_sprite(ge211::Dims<int>{50, 50}, player_normal),
          player_sprite(ge211::Dims<int>{25, 50},
                        player_normal),
          invincible_sprite(ge211::Dims<int>{25, 50},player_invincible),
          magnet_sprite(ge211::Dims<int>{25,50}, player_magnet),
          obstacle_sprite(m.config.brick_dims(), obstacle_color),
          coin_sprite(20, coin_color),
          power_up_sprite(10,extra_lives_color),
          magnet_obstacle_sprite(10, magnet_color),
          invinc_obstacle_sprite(10, invincibility_color),
          lives_sprite(10,extra_lives_color),
          ff_sprite(ge211::Dims<int>{25,50}, ff_color),
          ff_obstacle_sprite(10, ff_color),
          view_mixer(mixer),
          background_sprite("background edited.png"),
          start_screen("start screen.png"),
          defeat_screen("defeat screen.png"),
          police_sprite("police car edited.png"),
          lightning_sprite("lightning.png"),
          text_layer(4)
{ }

void
View::draw(ge211::Sprite_set& sprites)
{
    // TEXT
    ge211::Text_sprite::Builder coin_score_builder(sans18Font);
    coin_score_builder.color(ge211::Color(255, 255, 255));
    coin_score_builder << "Coins: " << round(model_.coins_collected);
    coins_collected_text_.reconfigure(coin_score_builder);
    sprites.add_sprite(coins_collected_text_, {0, 0}, text_layer);

    ge211::Text_sprite::Builder time_score_builder(sans18Font);
    time_score_builder.color(ge211::Color(255, 255, 255));
    time_score_builder << "Time passed: " << round(model_.time/60) << " seconds";
    time_travelled_text_.reconfigure(time_score_builder);
    sprites.add_sprite(time_travelled_text_, {0, 20}, text_layer);

    ge211::Text_sprite::Builder lives_score_builder(sans18Font);
    lives_score_builder.color(ge211::Color(255, 255, 255));
    lives_score_builder << "Lives: " << model_.runner.how_many_lives();
    lives_left_text.reconfigure(lives_score_builder);
    sprites.add_sprite(lives_left_text, {0, 40}, text_layer);

    ge211::Text_sprite::Builder dist_score_builder(sans18Font);
    dist_score_builder.color(ge211::Color(255, 255, 255));
    int a = model_.distance;
    dist_score_builder << "Distance: " << round(a/120);
    dist_travelled_text_.reconfigure(dist_score_builder);
    sprites.add_sprite(dist_travelled_text_, {0, 60}, text_layer);

    ge211::Text_sprite::Builder extra_life_text_builder(sans18Font);
    extra_life_text_builder.color(ge211::Color(131,238,255));
    extra_life_text_builder << "Blue = Extra life";
    extra_life_text.reconfigure(extra_life_text_builder);
    sprites.add_sprite(extra_life_text, {model_.config.scene_dims.width - 145, 0}, text_layer);

    ge211::Text_sprite::Builder invincibility_text_builder(sans18Font);
    invincibility_text_builder.color(ge211::Color(220, 20, 60));
    invincibility_text_builder << "Red = Magnet";
    invincibility_text.reconfigure(invincibility_text_builder);
    sprites.add_sprite(invincibility_text, {model_.config.scene_dims.width - 135, 20}, text_layer);

    ge211::Text_sprite::Builder magnet_text_builder(sans18Font);
    magnet_text_builder.color(ge211::Color(50, 150, 50));
    magnet_text_builder << "Green = Invincibility";
    magnet_text.reconfigure(magnet_text_builder);
    sprites.add_sprite(magnet_text, {model_.config.scene_dims.width - 175, 40}, text_layer);

    ge211::Text_sprite::Builder ff_text_builder(sans18Font);
    ff_text_builder.color(ge211::Color(138, 44, 226));
    ff_text_builder << "Purple = Fast forward";
    ff_text.reconfigure(ff_text_builder);
    sprites.add_sprite(ff_text, {model_.config.scene_dims.width - 175, 60}, text_layer);

    // ADD TEXT FOR DISTANCE, FF POWERUP

    // GAME
    ge211::Posn<int> p_player = {int(model_.runner.hitbox_.x),
                                 int(model_.runner.hitbox_.y)};

    if (model_.runner.check_ff()){
        sprites.add_sprite(ff_sprite, p_player);
    }

    else if (model_.runner.check_invincible()){
        sprites.add_sprite(invincible_sprite, p_player);
    }
    else if(model_.runner.check_magnetized()){
        sprites.add_sprite(magnet_sprite,p_player);
    }
    else {
        sprites.add_sprite(lightning_sprite, p_player);
    }

    for(Obstacle ii : model_.obstacles){
        ge211::Posn<int> p_b = {0,0};
        p_b.x = ii.hitbox_.x;
        p_b.y = ii.hitbox_.y;


        sprites.add_sprite(obstacle_sprite, p_b);
    }

    for (Coin ii: model_.coins){
        ge211::Posn<int> p_b = {0,0};
        p_b.x = ii.hitbox_.x-20;
        p_b.y = ii.hitbox_.y-20;
        sprites.add_sprite(coin_sprite, p_b+model_.config.brick_dims()/2, 2);
    }

    Powerups jj = model_.active_powerup;
    if (jj.in_play){
            Powerups ii = model_.active_powerup;
            ge211::Posn<int> p_b = {int(ii.hitbox_.x-10), int(ii.hitbox_.y-10)};
            if (ii.extra_life){
                sprites.add_sprite(power_up_sprite, p_b+model_.config.brick_dims()/2,3);
            }
            else if (ii.magnet){
                sprites.add_sprite(magnet_obstacle_sprite, p_b+model_.config.brick_dims()/2,3);
            }
            else if (ii.invincibility){
                sprites.add_sprite(invinc_obstacle_sprite, p_b+model_.config.brick_dims()/2,3);
            }
            else{
                sprites.add_sprite(ff_obstacle_sprite, p_b+model_.config.brick_dims()/2,3);
            }
        }

    // Add in background
    sprites.add_sprite(background_sprite, ge211::geometry::Posn<int> {0,0}, -1);
    sprites.add_sprite(police_sprite, ge211::geometry::Posn<int> {model_.config.scene_dims.width/2 - 32,
                                                                  model_.config.scene_dims.height - 110}, 5);
    sprites.add_sprite(police_sprite, ge211::geometry::Posn<int> {100,model_.config.scene_dims.height - 110}, 5);
    sprites.add_sprite(police_sprite, ge211::geometry::Posn<int> {350,model_.config.scene_dims.height - 110}, 5);

    // Start screen disappears when game starts
    if(!model_.runner.live){
        sprites.add_sprite(start_screen, ge211::geometry::Posn<int> {0, 150}, 5);
    }

    // Defeat screen appears when you die
    if(!model_.runner.check_alive()){
        sprites.add_sprite(defeat_screen, ge211::geometry::Posn<int> {0, 150}, 6);
    }
}

ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model_.config.scene_dims;
}

void View::load_audio(){ // REFERRED TO BEJEWELED GAME FOR HELP WITH AUDIO
    if (bg_music_.try_load(bg_music_filename, view_mixer)) {
            view_mixer.play_music(bg_music_, true);
        }
}

