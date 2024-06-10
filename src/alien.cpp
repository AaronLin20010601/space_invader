#include "alien.hpp"

Texture2D Alien::alienImages[3] = {};

//create alien entity
Alien::Alien(int type, Vector2 position)
{
    //initialize
    this -> type = type;
    this -> position = position;

    if(alienImages[type - 1].id == 0) {
        switch(type) {
            case 1:
                alienImages[0] = LoadTexture("Graphics/alien_1.png");
                break;
            case 2:
                alienImages[1] = LoadTexture("Graphics/alien_2.png");
                break;
            case 3:
                alienImages[2] = LoadTexture("Graphics/alien_3.png");
                break;
            default:
                alienImages[0] = LoadTexture("Graphics/alien_1.png");
                break;
        }
    }
}

//update the alien movement
void Alien::Update(int direction)
{
    position.x += direction;
}

//draw the alien on the screen
void Alien::Draw()
{
    DrawTextureV(alienImages[type - 1], position, WHITE);
}

//get the type of alien
int Alien::GetType()
{
    return type;
}

//unload the images of aliens
void Alien::UnloadImages()
{
    for(int i = 0; i < 4; i++) {
        UnloadTexture(alienImages[i]);
    }
}

//return the rectangle of alien object
Rectangle Alien::GetRect()
{
    return {position.x, position.y, float(alienImages[type - 1].width), float(alienImages[type - 1].height)};
}