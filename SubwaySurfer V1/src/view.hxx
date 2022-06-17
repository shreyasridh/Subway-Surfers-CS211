//
// INCOMPLETE
//

#pragma once

#include "model.hxx"

class Model;

class View
{
public:
    explicit View(Model const& model, ge211::Mixer& mixer);

    // Member functions
    void draw(ge211::Sprite_set& set);
    ge211::Dims<int> initial_window_dimensions() const;
    void load_audio();

    // Member variables
    Model const& model_;

    ge211::Rectangle_sprite const player_sprite;
    ge211::Rectangle_sprite const invincible_sprite;
    ge211::Rectangle_sprite const magnet_sprite;
    ge211::Rectangle_sprite const obstacle_sprite;
    ge211::Circle_sprite const coin_sprite;
    ge211::Circle_sprite const power_up_sprite;
    ge211::Circle_sprite const magnet_obstacle_sprite;
    ge211::Circle_sprite const invinc_obstacle_sprite;
    ge211::Circle_sprite const lives_sprite;

    ge211::Rectangle_sprite const ff_sprite;
    ge211::Circle_sprite const ff_obstacle_sprite;

    ge211::Mixer& view_mixer;
    ge211::Music_track bg_music_;

    // Imported images
    ge211::Image_sprite const background_sprite;
    ge211::Image_sprite const start_screen;
    ge211::Image_sprite const defeat_screen;
    ge211::Image_sprite const police_sprite;
    ge211::Image_sprite const lightning_sprite;

    // Text and font stuff
    ge211::Font sans18Font {"sans.ttf", 18};
    ge211::Text_sprite time_travelled_text_;
    ge211::Text_sprite coins_collected_text_;
    ge211::Text_sprite lives_left_text;
    ge211::Text_sprite dist_travelled_text_;
    ge211::Text_sprite extra_life_text;
    ge211::Text_sprite invincibility_text;
    ge211::Text_sprite magnet_text;
    ge211::Text_sprite ff_text;
    int text_layer;

private:
};
