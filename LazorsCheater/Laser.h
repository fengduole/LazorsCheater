#pragma once

#include "Point.h"

class Laser : public Point
{
public:
	Laser(void);
	
	bool operator==(Laser&);
	bool operator!=(Laser&);


public:
	int dx, dy;
};