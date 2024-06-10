#include "laser.hpp"

//create laser beam entity
Laser::Laser(Vector2 position, int speed)
{
    //initialize
    this -> position = position;
    this -> speed = speed;
    active = true;
}

//update the movement and position of laser beam
void Laser::Update()
{
    position.y += speed;
    if(active) {
        if(position.y > GetScreenHeight() - 100 || position.y < 25) {
            active = false;
        }
    }
}

//draw the laser on the screen
void Laser::Draw()
{
    if(active) {
        DrawRectangle(position.x, position.y, 4, 15, {243,216,63,255});
    }
}

//return the rectangle of laser object
Rectangle Laser::GetRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 4;
    rect.height = 15;
    return rect;
}
