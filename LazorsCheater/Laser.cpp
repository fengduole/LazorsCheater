#include "Laser.h"

Laser::Laser(void)
	: Point()
{
	dx = dy = 0;
}


bool Laser::operator==(Laser& laser)
{
	if ((laser.x == x) && (laser.y == y) && 
		(laser.dx == dx) && (laser.dy == dy))
		return true;

	return false;
}


bool Laser::operator!=(Laser& laser)
{
	return !operator==(laser);
}