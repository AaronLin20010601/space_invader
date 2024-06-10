#include "spaceship.hpp"

//create the spaceship game entity
Spaceship::Spaceship()
{
    //initialize
    image = LoadTexture("Graphics/spaceship.png");
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    lastFireTime = 0.0;
    laserSound = LoadSound("Sounds/laser.ogg");
}

//destruct the spaceship entity when the game close
Spaceship::~Spaceship()
{
    UnloadTexture(image);
    UnloadSound(laserSound);
}

//draw the spaceship on the screen
void Spaceship::Draw()
{
    DrawTextureV(image, position, WHITE);
}

//move the spaceship torward to the left direction
void Spaceship::MoveLeft()
{
    position.x -= 7;
    if(position.x < 9) {
        position.x = 9;
    }
}

//move the spaceship torward to the right direction
void Spaceship::MoveRight()
{
    position.x += 7;
    if(position.x > GetScreenWidth() - image.width - 9) {
        position.x = GetScreenWidth() - image.width - 9;
    }
}

//shoot the laser out from player to attack the enemy
void Spaceship::FireLaser()
{
    if(GetTime() - lastFireTime >= 0.35) {
        lasers.push_back(Laser({position.x + image.width / 2 - 2, position.y}, -6));
        lastFireTime = GetTime();
        PlaySound(laserSound);
    }
}

//return the rectangle of spaceship object
Rectangle Spaceship::GetRect()
{
    return {position.x, position.y, float(image.width), float(image.height)};
}

//reset the position of the spaceship
void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear();
}