#ifndef SUBWAYSURFERS_POWERUPS_HXX
#define SUBWAYSURFERS_POWERUPS_HXX

#include "ge211.hxx"
#include "object.hxx"

class Powerups : public Object{
public:
    // Constructors
    Powerups(double posx, double posy, double width, double height, bool m,
             bool i, bool l, bool ff, bool p);

    bool magnet;
    bool invincibility;
    bool extra_life;
    bool in_play;
    bool ff;
};
#endif //SUBWAYSURFERS_COIN_HXX
