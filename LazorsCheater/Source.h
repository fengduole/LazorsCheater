#pragma once

#include <iostream>

#include "Laser.h"

class Source
{
public:
	Source(void);

public:
	bool isLight;
	bool laser[3][3];
};