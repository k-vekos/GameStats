#include "Circle.h"

#include <iostream>

Circle::Circle(int centerX, int centerY, int radius)
{
	center.first = centerX;
	center.second = centerY;
	this->radius = radius;
}

void Circle::print() const
{
	std::cout << "---Circle---" << std::endl;
	std::cout << "center=(" << center.first << ", " << center.second << ")" << std::endl;
	std::cout << "radius=" << radius << std::endl;
}
