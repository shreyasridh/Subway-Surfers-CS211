#include "powerups.hxx"

Powerups::Powerups(double posx, double posy, double width, double height,
                   bool m, bool i, bool l, bool f, bool p)
        : Object(posx, posy, width, height, true),
        magnet(m),
        invincibility(i),
        extra_life(l),
        in_play(p),
        ff(f)
{ }
