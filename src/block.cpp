#include "block.hpp"

//create block entity
Block::Block(Vector2 position)
{
    //initialize
    this -> position = position;
}

//draw the block on the screen
void Block::Draw()
{
    DrawRectangle(position.x, position.y, 3, 3, {243,216,63,255});
}

//return the rectangle of block object
Rectangle Block::GetRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 3;
    rect.height = 3;
    return rect;
}
