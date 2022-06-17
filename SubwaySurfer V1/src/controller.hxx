#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    // Constructor
    Controller();
    // Keyboard inputs
    void on_key(ge211::Key) override;
    // Updates the game view each frame
    void on_frame(double dt) override;
    // Mouse cursor inputs
    void on_mouse_move(ge211::Posn<int>) override;

    ge211::Dims<int> initial_window_dimensions() const override;

protected:
    void draw(ge211::Sprite_set& set) override;

private:
    Model model_;
    View view_;
};
