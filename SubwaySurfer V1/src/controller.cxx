//
// INCOMPLETE
//

#include "controller.hxx"
#include "player.hxx"

Controller::Controller()
        : view_(model_, mixer())
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('q')) {
        quit();
    }

    if (key == ge211::Key::code(' ')) {
        // game starts paused, only when ' ' is pressed then the game goes 'live'
        // model_.launch();
        model_.runner.live = true;
        model_.launch();

        // REFERRED TO BEJEWELED GAME FOR HELP WITH AUDIO
        if (view_.view_mixer.get_music_state()==ge211::Mixer::State::detached) {
            view_.load_audio();
        }
    }
}

void
Controller::on_mouse_move(ge211::Posn<int> pos){
    // move the player to the mouse's position (ONLY WHEN HE IS LIVE)
    // model.paddle_to(position.x-model.config.paddle_dims.width/2);

    // ACCOUNTS FOR OFFSET OF OBJECT COORDINATES
    model_.runner.player_to(pos.x-model_.runner.hitbox_.width/2);
}

void
Controller::on_frame(double dt)
{
    // REFERRED TO BEJEWELED GAME FOR HELP WITH AUDIO
    if (!model_.runner.check_alive() && view_.view_mixer.get_music_state()!=ge211::Mixer::State::detached){
        view_.view_mixer.pause_music(ge211::Duration(0));
    }
    model_.on_frame(dt);
}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}