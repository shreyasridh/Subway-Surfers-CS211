//
// Created by kohda on 5/30/2022.
//

#include "ge211.hxx"
#include "object.hxx"

Object::Object()
: Object(0, 0, 0, 0, false)
{ }

Object::Object(double x, double y, double width, double height, bool is_it)
: hitbox_(ge211::Rect<double>(x, y, width, height)),
live(is_it)
{ }

bool
Object::check_collision(Object &obj) const{
    // NOT
    // top of reference object below bottom of external object
    // bottom of reference object above the top of external object
    bool hits_bottom =  !(hitbox_.y>obj.hitbox_.y+obj.hitbox_.height ||
            hitbox_.y+hitbox_.height < obj.hitbox_.y);

    // NOT
    // right of reference object to the left of the left of external object
    // left of reference object to the right of the right of external object
    bool hits_side = !(hitbox_.x + hitbox_.width < obj.hitbox_.x || hitbox_.x >
            obj.hitbox_.x+ obj.hitbox_.width);

    return (hits_bottom && hits_side);
}

void
Object::next(double dt, int vel){
    hitbox_.y += vel * dt;
}
