#include "mysteryship.hpp"

//create mystery ship entity
MysteryShip::MysteryShip()
{
    image = LoadTexture("Graphics/mystery.png");
    alive = false;
}

//destruct mystery ship entity
MysteryShip::~MysteryShip()
{
    UnloadTexture(image);
}

//update the movement of mystery ship
void MysteryShip::Update()
{
    if(alive) {
        position.x += speed;
        if(position.x > GetScreenWidth() - image.width - 9 || position.x < 9) {
            alive = false;
        }
    }
}

//draw the mystery ship on the screen
void MysteryShip::Draw()
{
    if(alive) {
        DrawTextureV(image, position, WHITE);
    }
}

//spawn the mystery ship in the game
void MysteryShip::Spawn()
{
    position.y = 90;
    int side = GetRandomValue(0,1);

    if(side == 0) {
        position.x = 9;
        speed = 3;
    }
    else {
        position.x = GetScreenWidth() - image.width - 9;
        speed = -3;
    }

    alive = true;
}

//return the rectangle of mystery ship object
Rectangle MysteryShip::GetRect()
{
    if(alive) {
        return {position.x, position.y, float(image.width), float(image.height)};
    }
    else {
        return {position.x, position.y, 0, 0};
    }
}
