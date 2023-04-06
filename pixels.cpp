//Author: satanisticmicrowave

#include "pixels.h"

pixels::pixels::pixels
{
    redraw();
}

pixels::pixels::~pixels()
{

}

void pixels::pixels::kill(dots::dot dot)
{

}

void pixels::pixels::create(dots::dot dot)
{

}

void pixels::pixels::scale(int scale)
{
    this->dot_scale = scale;
    this->redraw();
}

void pixels::pixels::redraw()
{
    setGeometry(EDITPANEL_WIDTH, 0, GAME_WIDTH-EDITPANEL_WIDTH, GAME_HEIGHT);
}

void pixels::pixels::move_camera(int x, int y)
{

}
